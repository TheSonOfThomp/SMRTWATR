import logging
import sys, os
import os.path
sys.path.append('tornado')
import tornado.websocket
import tornado.httpserver
import tornado.ioloop
import tornado.template
import traceback
from threading import Timer
from itertools import cycle

loader = tornado.template.Loader(os.path.join(os.path.join(os.path.realpath(__file__) + '/../'), 'templates'))

questions = [
    {'q': 'What is the best Taylor Swift song?', 'a': ['Our Song', 'Red', 'Teardrops on My Guitar', 'Speak Now']},
    {'q': 'Best Female Canadian Soccer Player?', 'a': ['Tancredi', 'Chapman', 'Schmidt', 'Fleming']},
    {'q': 'Best Season?', 'a': ['Fall', 'Winter', 'Spring', 'Summer']}
]

answers = [
    'Speak Now',
    'Chapman',
    'Fall'
]
    
def gamebroadcast(message):
    for waiter in GameWebSocket.waiters:
        try:
            waiter.write_message(message)
        except:
            logging.error("Error sending message", exc_info=True)

class Game(object):
    def __init__(self):
        self.players = []
        self.state = self.add_player
        self.winner = None
        self.openPlayers = ["1", "2", "3", "4"]
        self.grid = None
        self.rightAnswer = ''

    def add_player(self, player):
        self.players.append(player)
        self.broadcast('Player added')
        if len(self.players) == 4:
            self.start_game()

    def quiz_splash(self):
        self.grid = 'start'
        self.broadcast('starting quiz')

    def start_question(self, *args):
        for player in self.players:
            player.correct = None
            player.guess = ''
        i = args[0]
        self.rightAnswer = answers[i]
        self.grid = questions[i]
        self.broadcast('new question')

    def end_question(self):
        for player in self.players:
            if player.guess == '':
                self.make_guess(player, '')

    def start_game(self):
        self.winner = None
        t0 = Timer(0.0, self.quiz_splash)
        t1 = Timer(5.0, self.start_question, [0])
        t2 = Timer(20.0, self.end_question)
        t3 = Timer(25.0, self.start_question, [1])
        t4 = Timer(40.0, self.end_question)
        t5 = Timer(45.0, self.start_question, [2])
        t6 = Timer(60.0, self.end_question)
        t7 = Timer(65.0, self.check_winner)
        t0.start()
        t1.start()
        t2.start()
        t3.start()
        t4.start()
        t5.start()
        t6.start()
        t7.start()
        

    def make_guess(self, player, answer):
        player.guess = answer
        if answer == self.rightAnswer :
            player.correct = True
            player.score += 10
            gamebroadcast('Update: Player ' + player.symbol + ' got it right and now has ' + str(player.score) + ' points')
            player.socket.write_message('You Are Right!')
        else :
            player.correct = False
            gamebroadcast('Update: Player ' + player.symbol + ' got it wrong and remains at ' + str(player.score) + ' points')
            player.socket.write_message('You Are Wrong!')
            player.socket.write_message('The right answer was ' + self.rightAnswer)

    def broadcast(self, message):
        try:
            for player in self.players:
                if player.socket:
                    player.socket.write_message(message)
        except:
            traceback.print_exc()
    
    def check_winner(self):
        score = 0
        winner = 'nobody. Because everyone scored 0:'
        for player in self.players:
            self.broadcast('Player ' + player.symbol + ' score: ' + str(player.score))
            if player.score == score :
                winner = winner + ' and Player' + player.symbol
            elif player.score > score :
                winner = 'Player' + player.symbol
                score = player.score
            player.score = 0
        self.broadcast('Winner is ' + winner)
        self.grid = None

class Player(object):
    def __init__(self, symbol, game):
        self.symbol = symbol
        self.socket = None
        self.game = game
        self.callbacks = {}
        self.score = 0
        self.guess = ''

    def add(self):
        self.game.add_player(self)

    def forget(self):
        self.callbacks = {}

    def remember(self, callback, *args, **kwargs):
        def doit():
            callback(*args, **kwargs)
        cid = str(id(doit))
        self.callbacks[cid] = doit
        return cid

    def make_guess(self, answer):
        self.game.make_guess(self, answer)

class PlayerHandler(tornado.web.RequestHandler):
    def __init__(self, *args, **kwargs):
        self.player = kwargs.pop('player')
        self.template = kwargs.pop('template')
        super(PlayerHandler, self).__init__(*args, **kwargs)

    def get(self):
        self.write(loader.load(self.template).generate(player=self.player))
        if self.player.symbol in game.openPlayers :
            gamebroadcast('Player added')
            self.player.add()
            game.openPlayers.remove(self.player.symbol)

class PlayerWebSocket(tornado.websocket.WebSocketHandler):
    def __init__(self, *args, **kwargs):
        self.player = kwargs.pop('player')
        self.player.socket = self
        super(PlayerWebSocket, self).__init__(*args, **kwargs)
    
    def on_message(self, message):
        if self.player.callbacks.get(message, None):
            self.player.callbacks[message]()

    def on_close(self):
        game.players.remove(self.player)
        game.openPlayers.append(self.player.symbol)
        game.openPlayers.sort()
        game.broadcast("Player Removed")
        gamebroadcast("Player Removed")

class GameWebSocket(tornado.websocket.WebSocketHandler):
    waiters = set()

    def __init__(self, *args, **kwargs):
        self.game = kwargs.pop('game')
        self.game.socket = self
        super(GameWebSocket, self).__init__(*args, **kwargs)

    def open(self):
        GameWebSocket.waiters.add(self)

    def on_close(self):
        GameWebSocket.waiters.remove(self)

    def on_message(self, message):
        self.game.start_game()


class SplashHandler(tornado.web.RequestHandler):
    def __init__(self, *args, **kwargs):
        self.template = kwargs.pop('template')
        super(SplashHandler, self).__init__(*args, **kwargs)

    def get(self):
        self.write(loader.load(self.template).generate(openPlayers=game.openPlayers))

game = Game()
player1 = Player('1', game)
player2 = Player('2', game)
player3 = Player('3', game)
player4 = Player('4', game)

application = tornado.web.Application(
    [
     (r"/",              SplashHandler,   {'template': 'splash.html'}),
     (r"/openplayers",   SplashHandler,   {'template': 'openPlayers.html'}),
     (r"/game/ws",       GameWebSocket,   {'game': game}),
     (r'/play/er1',      PlayerHandler,   {'player': player1,
                                    'template': 'player.html'}),
     (r'/play/er1/grid', PlayerHandler,   {'player': player1,
                                    'template': 'grid.html'}),
     (r'/play/er1/ws',   PlayerWebSocket, {'player': player1}),
     (r'/play/er2',      PlayerHandler,   {'player': player2,
                                    'template': 'player.html'}),
     (r'/play/er2/grid', PlayerHandler,   {'player': player2,
                                    'template': 'grid.html'}),
     (r'/play/er2/ws',   PlayerWebSocket, {'player': player2}),
     (r'/play/er3',      PlayerHandler,   {'player': player3,
                                    'template': 'player.html'}),
     (r'/play/er3/grid', PlayerHandler,   {'player': player3,
                                    'template': 'grid.html'}),
     (r'/play/er3/ws',   PlayerWebSocket, {'player': player3}),
     (r'/play/er4',      PlayerHandler,   {'player': player4,
                                    'template': 'player.html'}),
     (r'/play/er4/grid', PlayerHandler,   {'player': player4,
                                    'template': 'grid.html'}),
     (r'/play/er4/ws',   PlayerWebSocket, {'player': player4})
    ],
    template_path=os.path.join(os.path.dirname(__file__), "templates"),
    static_path=os.path.join(os.path.dirname(__file__), "static")
)


if __name__ == '__main__':
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(9999)
    tornado.ioloop.IOLoop.instance().start()
