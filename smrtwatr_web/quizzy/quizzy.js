//NOTE: QuizzyConfig.js MUST be included BEFORE this file!

//current quiz state
var quizFile = "testQuiz.xml";
var quizIndex = 0;
var curQuestion = -1;
var score = 0;

//this is set by jquery when the user clicks on one of the radio buttons
var selOpt = 0;
//these are set when the explantion data is dropped into its div
var correctOpt = -1;
var addScore = 0;
var bestScore = 0;
var optValues;
//these are set at other times by dropped in php code from ajax calls
var totalQuestions = -1;
var quizWidth = -1;
var quizHeight = -1;

var questionTimeout = 0;

//When the quiz is ready, start loading up the quiz
$('#quizzy_frame').ready(function() {
	$.loading.pulse = loadingPulse;
	$.loading.align = loadingAlign;
	$.loading.onAjax = true;	//don't change this!
	//$.loading.delay = loadingDelay;
	//reset all the variables
	quizIndex = "0";
	curQuestion = 0;
	score = 0;
	selOpt = 0;
	correctOpt = -1;
	addScore = 0;
	// quizWidth & quizHeight re effectively the window sizes
	quizWidth = $(window).width(); //$('#quizzy').width();
	quizHeight = $(window).height(); //$('#quizzy').height();
	
	console.log("quizFile: " + quizFile);
	startQuiz();
});

//requests a quiz setup from the server
// The server will also send 3 random integers corresponding to the quiz questions to be asked
function startQuiz()
{
	//make sure that there's a quiz that is selected
	if(quizIndex < 0)
		return;
		
	//unbind the click events for this button
	$(this).unbind();
	
	//globals were already set when the user clicked on the radio buttons
	
	//fade out quiz options
	$('.quizzy_quiz_b').fadeOut(fadeSpeed);
	
	//parameters passed in GET:
	//  _GET['quizFile']       xml file to open
	//  _GET['quizIndex']      index of requested quiz in xml file
	$.get('quizzy/serveQuiz.php', {quizFile: quizFile, quizIndex: quizIndex}, function(data){
		
		//put up throbber
		//$('#quizzy').loading(true);
		
		//we got our quiz datas, just dump them into the correct div
		$('#quizzy_quiz').html(data);
		
		//we also got a totalQuestions set, need to resize a few divs.
		$('.quizzy_q').width(quizWidth);
		$('#quizzy_c').width((totalQuestions + 3) * quizWidth);
		$('#quizzy_quiz').width((totalQuestions + 2) * quizWidth);
		$('.quizzy_title').width(quizWidth);
		
		//now request the next question
		requestNextQuestion();
	});
}

//requests a question from the server
function requestNextQuestion()
{		

	//parameters passed in GET:
	//  _GET["quizFile"]       xml file to open
	//  _GET["quizIndex"]      index of requested quiz in xml file
	//  _GET["questNo"]        question to return [first question is number 0]
	//  _GET['score']          score the player currently has (needed for serving last page)
	$.get('quizzy/serveQuestion.php', {quizFile: quizFile, quizIndex: quizIndex, questNo: curQuestion, score: score}, function(data){
		
		$('.quizzy_title span').html(curQuestion + 1);

		//dump the recieved data into the correct question div
		$("#quizzy_q" + curQuestion).html(data);
		
		//add the click & touchstart event to the option buttons for iOS
		if(/iP(hone|ad)/.test(window.navigator.userAgent)) {
			$('#quizzy_q' + curQuestion +' .quizzy_q_opt').bind('touchstart', function(){}, false);
		};
		//as soon as the user picks an option, we check the answer
		$('#quizzy_q' + curQuestion +' .quizzy_q_opt').click(checkQuestion);


		// document.addEventListener('touchstart', function(e) {
		//     e.preventDefault();
		//     var touch = e.touches[0];
		//     alert(touch.pageX + " - " + touch.pageY);
		// }, false);


				//add click handlers so that when a user clicks on any first option, it sets selOpt to 0
				//and if they click on any 2nd option, it sets selOpt to 1, etc.
				// $('.quizzy_q_opt').click(function (){
				// 	//the user clicked on one of the options
				// 	//get the id
				// 	var thisId = $(this).attr('id');
					
				// 	//hack out the index and set selOpt to it
				// 	selOpt = thisId.substring(thisId.lastIndexOf("opt") + 3) * 1;
					
				// 	//make sure that the radio button is selected
				// 	$('#quizzy_q'+curQuestion+'_opt'+selOpt+'_b').attr("checked", "checked");
				// });
		

				// $('#quizzy_q' + curQuestion + '_foot_nxt').click(function (){
				// 	$('#quizzy').loading(true);   
				// 	$(this).unbind();
				// 	requestNextQuestion();
				// });
		
		//slide quizzy_c to the right if we're on question 0, quizzy_q_c otherwise
		var scrollSel = (curQuestion == 0) ? '#quizzy_c' : '#quizzy_q_c';
		var scrollAmt = (-quizWidth * (curQuestion));

		$(scrollSel).animate({left: scrollAmt + "px"}, slideSpeed, animateStyle, function(){
			//uncheck the last question's buttons
			$('.quizzy_q_opt_b').attr('checked', false);
		});

			//reset the radial timer after the question is loaded
			resetTimer(MAX_QUESTION_TIME);

			//if this is not the last question, then we set a timeout
			if (curQuestion < 3) {
				//If the user takes too long, then we skip the question
				questionTimeout = setTimeout(function() {
					forfeitQuestion()
				}, toMS(MAX_QUESTION_TIME));
			}else killTimer();
	});
}

function checkQuestion()
{ 
	//Cancel the question timeout timer
	clearTimeout(questionTimeout);
	
	//unbind the listeners from the opts
	$('#quizzy_q' + curQuestion +' .quizzy_q_opt').unbind();
	$(this).unbind();	

	//the user has selOpt selected on question curQuestion
	//on the quizIndex'th quiz in quizFile

	//get the id
	var thisId = $(this).attr('id');
	//hack out the index and set selOpt to it
	selOpt = thisId.substring(thisId.lastIndexOf("opt") + 3) * 1;

	//get the explanation for this option, it will set the correctOpt variable
	//parameters passed in GET:
	//  _GET['quizFile']       xml file to open
	//  _GET['quizIndex']      index of requested quiz in xml file
	//  _GET['questNo']        question to return (first is 1)
	//	_GET['selOpt']				 the option for which to retrieve the explanation
	$.get('quizzy/serveExplanation.php',  {quizFile: quizFile, quizIndex: quizIndex, questNo: curQuestion, selOpt: selOpt}, function(data) {

			//reset the radial timer to read the explanation
			resetTimer(MAX_CHECK_TIME);

			//and timeout after that time
			setTimeout(function(){
				curQuestion++;
				requestNextQuestion();
			}, toMS(MAX_CHECK_TIME));

		//have the data returned by that ajax query, set the proper div info
		$('#quizzy_q' + curQuestion + '_exp').html(data);
		//that should have set the correctOpt and addScore variables

		//add to score
		score += addScore;
		
				//determine if this question has partial credit
				var partialCredit = false;
				for(var i in optValues)
					if(optValues[i] != 0 && optValues[i] != bestScore)
						partialCredit = true;
			
				//show the values
				for( i in optValues ) {
					
					//if the question no partial credit, use an X or a ✓ to indicate correctness
					var toWrite = optValues[i];
					if(!partialCredit)
						toWrite = (optValues[i] == bestScore) ? '✓' : 'X'; //Use glyficons 
					
					//if it was best score, use quizzy_opt_best
					//in between best and worst, use quizzy_opt_mid
					//or the worst, use quizzy_opt_worst
					var useClass = 'quizzy_opt_worst';
					if(optValues[i] == bestScore)
						useClass = 'quizzy_opt_best';
					if(optValues[i] > 0 && optValues[i] < bestScore)
						useClass = 'quizzy_opt_mid';
					
					//Set checks & Xs after the question
					//$('#quizzy_q' + curQuestion + '_opt' + i + '_val').html('<span class="' + useClass + '">' + toWrite + '</span>');
					
				}
				//show checks & Xs after the question
				//$('.quizzy_q_opt_val').fadeIn(fadeSpeed);

		//debugger;

		//Turn the correct answer green
		$('#quizzy_q' + curQuestion + '_opt' + correctOpt).addClass('correct');
		//Turn the selected answer red if it's wrong
		if (selOpt !== correctOpt) {
			$('#quizzy_q' + curQuestion + '_opt' + selOpt).addClass('incorrect');
		}

		//scroll up all but the selected answer and the best answer
		var notCorrectSel = '[id!=quizzy_q' + curQuestion + '_opt' + correctOpt + ']';
		var notPickedSel = '[id!=quizzy_q' + curQuestion + '_opt' + selOpt + ']';
		if(addScore == bestScore)
			notCorrectSel = '';

		//Hide the wrong answers
		$('.quizzy_q_opt' + notCorrectSel + notPickedSel).slideUp(slideSpeed);
		//fade in explanation
		$('#quizzy_q' + curQuestion + '_exp').fadeIn(fadeSpeed);


		//debugger;
	});
}

function forfeitQuestion(){
	console.log('----TIMING OUT----');

	//Cancel the question timeout timer
	clearTimeout(questionTimeout);
	//reset the radial timer to read the explanation
	resetTimer(MAX_CHECK_TIME);
	
	//since no option was selected,
	selOpt = -1;

	$.get('quizzy/serveExplanation.php',  {quizFile: quizFile, quizIndex: quizIndex, questNo: curQuestion, selOpt: selOpt}, function(data) {
		//have the data returned by that ajax query, set the proper div info
		$('#quizzy_q' + curQuestion + '_exp').html(data);
		//that should have set the correctOpt and addScore variables

		//Turn the correct answer green
		$('#quizzy_q' + curQuestion + '_opt' + correctOpt).addClass('correct');

		//Select all but the correctOption & hide them
		var notCorrectSel = '[id!=quizzy_q' + curQuestion + '_opt' + correctOpt + ']';
		$('.quizzy_q_opt' + notCorrectSel).slideUp(slideSpeed);

		$('#quizzy_q' + curQuestion + '_exp').fadeIn(fadeSpeed);

		setTimeout(function(){
			curQuestion++;
			requestNextQuestion();
		}, toMS(MAX_CHECK_TIME));
	});
}

// -- DEPRECATED -- //
function restartQuizzy()
{
	//figure out how much of the animation is in scrolling the questions back
	var firstRatio = curQuestion / (curQuestion + 1);
	//and how much is in scrolling the big container over
	var secondRatio = 1.0 - firstRatio;
	
	//reset all the state variables
	curQuestion = -1;
	score = 0;
	selOpt = 0;
	correctOpt = -1;
	addScore = 0;
	
	startQuiz();

	//unselect any selected quiz
	$('.quizzy_quiz_opt').attr('checked', false);
	//hide all the descriptions
	$('.quizzy_quiz_desc').hide();
	
	//scroll the quizzy_q_c back to the start
	$('#quizzy_q_c').animate({left: "0px"}, firstRatio * restartSpeed, animateStyle, function(){
		
		//scroll the quizzy_c back to the start
		$('#quizzy_c').animate({left: "0px"}, secondRatio * restartSpeed, animateStyle, function(){	
		}); //quizzy_c
	}); //quizzy_q_c
}

// --- DEPRECATED --- //
function showQuizList() {
	//load the quiz list
	//the buttons have onClick events so they're handled up there
	$.get('quizzy/serveQuizzes.php', function(data){

		console.log("Load Quiz List");
			
		$('#quizzy_load').html(data);
		//hide the descriptions
		$('.quizzy_quiz_desc').hide();
		
		//add a click event to the radio buttons' label
		$('.quizzy_quiz_lbl').click(function () {
			//the user clicked on one of the options
			//get the id
			var thisId = $(this).attr('id');

			//hack out the index and set selOpt to it
			var selQuiz = thisId.substring(thisId.lastIndexOf("lbl") + 3) * 1;
			
			//make sure that the radio button is selected
			$('#quizzy_quiz_opt'+selQuiz).click();
		});
		
		//add another click event handler to the radio buttons
		$('.quizzy_quiz_opt').click(function() {
			//the user clicked on one of the options
			//get the id
			var thisId = $(this).attr('id');

			//hack out the index and set selOpt to it
			var selQuiz = thisId.substring(thisId.lastIndexOf("opt") + 3) * 1;
			
			//slide up all other descriptions while sliding down the correct one
			$('.quizzy_quiz_desc[id!=quizzy_quiz_desc'+selQuiz+']').slideUp(slideSpeed, function() {
				$('#quizzy_quiz_desc' + selQuiz).slideDown(slideSpeed);
			});
		});

		//set the click event on the submit button
		$('#quizzy_start_b').click(startQuiz);
	});
}
