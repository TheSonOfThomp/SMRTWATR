$(document).ready(function(){
	$('#alert-row').hide();
});

function N_A_ () {
	$('#alert').html('SMRT WΛTR is still in Beta. We haven\'t implememted this yet!')
	$('#alert-row').fadeIn(300);
	setTimeout(function(){
		$('#alert-row').fadeOut(300)
	}, 1750);
}