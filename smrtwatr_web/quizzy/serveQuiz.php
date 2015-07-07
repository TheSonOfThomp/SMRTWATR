<?php
  include 'quizzyConfig.php';
	/*
	 * DESCRIPTION: 
	 * 	 used in the AJAX of the core quizzy php. serves the div framework of
	 *   of the quiz (i.e. puts enough question divs in there, sets proper ids etc.
	 * 
	 * PARAMETERS passed in GET:
	 *  _GET['quizFile']       xml file to open
	 *  _GET['quizIndex']      index of requested quiz in xml file
	 *   
	 */
	$quizFile = $cwd.'/'.$quizFolder.'/'.$_GET['quizFile'];
	$quizIndex = intval($_GET['quizIndex']);
		
	//load up the quiz
	include 'quizzyXML.php';
	$quiz = loadQuiz($quizFile, $quizIndex);	
	
	//find the number of questions and dump it to a javascript variable
	$totalQuestions = count($quiz->question);
?>
  
  <script language="JavaScript">
    totalQuestions = <?php echo $totalQuestions; ?>;
  </script>

<?php	
	//and the name of the quiz
	$quizTitle = $quiz->title;
	
	//return that (insane) div structure
	//make the containers
?>

	<div class="quizzy_title">Question <span>1</span></div>
	<div id="quizzy_q_c">

	<?php
		//make a div for the max # of questions
		for($qi=0; $qi < $MAX_QUESTIONS + 1; $qi++)
		{
	?>
	
		<div class="quizzy_q" id="quizzy_q<?php echo $qi; ?>">&nbsp;</div>
	
	<?php
			//end the loop
		}
		//close the containers
	?>
	
</div>