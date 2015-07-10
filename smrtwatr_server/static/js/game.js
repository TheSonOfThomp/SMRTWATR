WebSocket.__swfLocation = "/static/WebSocketMain.swf";

function update_grid() {
    $('#grid').load(location.pathname + '/grid');
}

$(document).ready(function() {
    ws = new WebSocket('ws://' + location.hostname + ':' + location.port + location.pathname + '/ws')

    gws = new WebSocket('ws://' + location.hostname + ':' + location.port + '/game/ws')

    ws.onmessage = function(msg) {
        update_grid();
        parse_msg(msg);
        console.log('ws: ' + msg.data);
        $('#messages').prepend(msg.data + '<br/>');
        // toast new players
    }

    gws.onmessage = function(msg) {
        console.log('gws: ' + msg.data);
    	if (msg.data.lastIndexOf('Update', 0) === 0) {
            update_grid();
    		parse_msg(msg)
            // toast other players' scores
    		$('#messages').prepend(msg.data + '<br/>');
    	}
    	console.log(msg);
    }

    update_grid();
});

function parse_msg(msg) {
    switch (msg.data) {
        case 'Player added': 
        	//alert all players
            break;
        case 'new question':

            break;
        default: 
        	break;
    }
}

// ANIMATION TIMING VARIABLES //

// How long it takes for the wrong answers to slide up 
var optSlideTime = 500;
// How long it takes or the DYK to fade in
var dykFadeTime = 300;

