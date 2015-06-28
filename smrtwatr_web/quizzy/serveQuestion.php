<?php
  include 'quizzyConfig.php';
	/*
	 * DESCRIPTION: 
	 * 	 used in the AJAX of the core quizzy php. serves up ONE question of the
	 *   requested quiz. 
	 * 
	 * PARAMETERS passed in GET:
	 *  _GET['quizFile']       xml file to open
	 *  _GET['quizIndex']      index of requested quiz in xml file
	 *  _GET['questNo']        question to return (first is 0)
	 *  _GET['score']          score the player currently has (needed for serving last page)
	 *   
	 */
	$quizFile = $cwd.'/'.$quizFolder.'/'.$_GET['quizFile'];
	$quizIndex = intval($_GET['quizIndex']);
  $questNo = intval($_GET['questNo']);
  $score = intval($_GET['score']);
	?>

  <script type="text/javascript">
  console.log('----REQUESTED QUESTION----');
  console.log('questNo: ' + '<?php echo $questNo; ?>');
  console.log('MAX_QUESTIONS: ' + '<?php echo $MAX_QUESTIONS; ?>')
  </script>

  <?php

	//load up the quiz
	include 'quizzyXML.php';
	$quiz = loadQuiz($quizFile, $quizIndex);	
  
  //represents where this quiz's pictures should be found
  $picDir = 'quizzy/'.$quizFolder.'/'.$picFolder.'/';
	
  //see if the requested question exists
  while($questNo >= count($quiz->question)) {
    $questNo--; // Iterate down until the question exists (this is not likely to be an issue)
  }
  ?>

  <?php
  // SCORING FUNCTIONS GO HERE //
  if ($questNo >= $MAX_QUESTIONS) {
    $questNo == $MAX_QUESTIONS;
    include 'endgame.php';
    return;
  }

  // QUESTION FETCHING //
	//get the requested question
  $quest=$quiz->question[$questNo];
?>

<div class="quizzy_q_body">
  <?php if(isset( $quest->img )) { ?>
	  <img src="<?php echo $picDir.$quest->img['src'];?>" alt="<?php echo $quest->img['alt'];?>" >
  <?php } ?>
	<p><?php echo $quest->text; ?></p>
</div>
<div class="quizzy_q_opts">
		
<?php
	//loop through all of the options for this question
  $oi = 0;
	foreach($quest->option as $opt)
	{
    $oiletter = intToLetter($oi);
?>
		<div class="quizzy_q_opt noselect animate_all" id="quizzy_q<?php echo $questNo; ?>_opt<?php echo $oi; ?>">
			<!-- input type=radio class=quizzy_q_opt id=quizzy_q[qi]_opt[oi] name=quizzy_q[qi] -->
      <script type="text/javascript">
      </script>
      <span class="quizzy_q_opt_letter" id="quizzy_q<?php echo $questNo; ?>_opt<?php echo $oiletter; ?>_b"><?php echo $oiletter; ?></span>
			<input type="radio" name="quizzy_q<?php echo $questNo; ?>" class="quizzy_q_opt_b" id="quizzy_q<?php echo $questNo; ?>_opt<?php echo $oi; ?>_b">
			<label>
				<?php echo $opt->text; ?>
        <?php if(isset($opt->img)) { ?>
          <img src="<?php echo $picDir.$opt->img['src'];?>" alt="<?php echo $opt->img['alt'];?>" >
        <?php } ?>
        <span class="quizzy_q_opt_val" id="quizzy_q<?php echo $questNo ?>_opt<?php echo $oi; ?>_val"></span>
			</label>
		</div>
		
<?php
		//finish loop
		$oi++;
	}
	//end list
?>
</div><!--options-->
<div class="quizzy_q_exp" id="quizzy_q<?php echo $questNo ?>_exp"></div>



<!-- 
<div class="quizzy_q_foot">
	<input type="submit" class="quizzy_q_foot_b" id="quizzy_q<?php echo $questNo ?>_foot_chk" value="Check Answer">
	<input type="submit" class="quizzy_q_foot_b" id="quizzy_q<?php echo $questNo ?>_foot_nxt" value="Next">				
</div> -->