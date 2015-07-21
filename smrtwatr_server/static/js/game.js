WebSocket.__swfLocation = "/static/WebSocketMain.swf";

function update_grid() {
    $('#grid').load(location.pathname + '/grid');
}

function update_quizbtm() {
    $('#quizbtm').load(location.pathname + '/quizbtm');
}

$(document).ready(function() {
    ws = new WebSocket('ws://' + location.hostname + ':' + location.port + location.pathname + '/ws')

    gws = new WebSocket('ws://' + location.hostname + ':' + location.port + '/game/ws')

    ws.onmessage = function(msg) {
        if (msg.data.indexOf('Player Removed') == 0) {
            // Ignore
            console.log('Ignoring Message');
        }
        else if (msg.data.indexOf('pi:') == 0) {
            update_grid();
        }
        else {
            update_grid();
        }
        

        console.log('ws: ' + msg.data);
        $('#messages').prepend(msg.data + '<br/>');
        // toast new players
    }

    gws.onmessage = function(msg) {
        console.log('gws: ' + msg.data);
        if (msg.data === 'END') {
            window.location.assign('/');
        }
        else if (msg.data.indexOf('Player Removed') == 0) {
            // Ignore
            console.log('Ignoring Message');
        }
    	else if (msg.data.indexOf('pi:') == 0) {
            update_quizbtm()

        }
        else {
            update_quizbtm()
            // toast other players' scores
    	}

    	console.log(msg);
    }

    update_grid();
    update_quizbtm();

    $('#radialTimer').hide();
});


// ANIMATION TIMING VARIABLES //
// How long it takes for the wrong answers to slide up 
var optSlideTime = 500;
// How long it takes or the DYK to fade in
var dykFadeTime = 300;

