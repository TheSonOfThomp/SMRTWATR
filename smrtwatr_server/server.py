import logging
import sys, os
sys.path.append('tornado')
import tornado.websocket
import tornado.httpserver
import tornado.ioloop
import tornado.template
import traceback
from itertools import cycle

loader = tornado.template.Loader(os.path.join(os.path.join(os.path.realpath(__file__) + '/../'), 'templates'))

class Game(object):
    def __init__(self):
        self.players = []
        self.state = self.add_player
        self.winner = None
        self.openPlayers = ["1", "2", "3", "4"]
        self.grid = None

    def add_player(self, player):
        self.players.append(player)
        self.broadcast('Player added')
        if len(self.players) == 4:
            self.start_game()

    def start_game(self):
        self.grid = [[None, None, None],
                     [None, None, None],
                     [None, None, None]]
        self.broadcast('Game started')
        # This creates a generator which cycles over the elements in a list
        self.turn_order = cycle(self.players)
        self.next_player = self.turn_order.next()
        self.winner = None
        self.broadcast('New game')

    def make_move(self, player, x, y):
        if player != self.next_player:
            player.socket.write_message('ERR: Out of turn!')
            return
        if self.grid[y][x] != None:
            player.socket.write_message('ERR: Space occupied')
            return
        self.grid[y][x] = player.symbol
        self.broadcast('%s played at %s, %s' % (player.symbol, x, y))
        self.check_winner()
        self.next_player = self.turn_order.next()
        self.broadcast("It is Player " + self.next_player.symbol + "'s turn")

    def broadcast(self, message):
        try:
            for player in self.players:
                if player.socket:
                    player.socket.write_message(message)
        except:
            traceback.print_exc()
    
    def check_winner(self):
        def all_same(symbol, set):
            set = map(lambda _: _ == symbol, set)
            return all(set)

        for player in self.players:
            for y in xrange(0, 3):
                if all_same(player.symbol, self.grid[y]):
                    self.winner = player.symbol
            for x in xrange(0, 3):
                if all_same(player.symbol, [self.grid[0][x],
                                            self.grid[1][x],
                                            self.grid[2][x]]):
                    self.winner = player.symbol
            if all_same(player.symbol, [self.grid[0][0],
                                        self.grid[1][1],
                                        self.grid[2][2]]):
                    self.winner = player.symbol
            if all_same(player.symbol, [self.grid[0][2],
                                        self.grid[1][1],
                                        self.grid[2][0]]):
                    self.winner = player.symbol
        if self.winner:
            self.broadcast(self.winner + ' wins!')
            self.start_game()

class Player(object):
    def __init__(self, symbol, game):
        self.symbol = symbol
        self.socket = None
        self.game = game
        self.callbacks = {}

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

    def make_move(self, x, y):
        self.game.make_move(self, x, y)

class PlayerHandler(tornado.web.RequestHandler):
    def __init__(self, *args, **kwargs):
        self.player = kwargs.pop('player')
        self.template = kwargs.pop('template')
        super(PlayerHandler, self).__init__(*args, **kwargs)

    def get(self):
        self.write(loader.load(self.template).generate(player=self.player))
        #self.player.socket.write_message("Player joined")
        if self.player.symbol in game.openPlayers :
            for waiter in GameWebSocket.waiters:
                try:
                    waiter.write_message('Player added')
                except:
                    logging.error("Error sending message", exc_info=True)

            self.player.add()
            game.openPlayers.remove(self.player.symbol)

class PlayerWebSocket(tornado.websocket.WebSocketHandler):
    def __init__(self, *args, **kwargs):
        self.player = kwargs.pop('player')
        self.player.socket = self
        super(PlayerWebSocket, self).__init__(*args, **kwargs)

    #def open(self):
    #   self.receive_message(self.on_message)
    
    def on_message(self, message):
        if self.player.callbacks.get(message, None):
            self.player.callbacks[message]()
        # Keep receiving messages
        # self.receive_message(self.on_message)

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

settings = {'static_path': os.path.join(os.path.realpath(__file__ + '/../'), 'web-socket-js')}

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
     (r'/play/er4/ws',   PlayerWebSocket, {'player': player4})],
    **settings)

if __name__ == '__main__':
    http_server = tornado.httpserver.HTTPServer(application)
    http_server.listen(9999)
    tornado.ioloop.IOLoop.instance().start()
