  <?php
  {
    //see what the max possible score for the quiz was
    $maxPossible = 0;
    foreach($quiz->question as $quest) {
      $thisBest = 0;
      foreach($quest->option as $opt) {
        if(intval($opt->score) > $thisBest)
          $thisBest = $opt->score;
      }
      $maxPossible += intval($thisBest);
      $maxPossible = $MAX_QUESTIONS;
    }
?>
    <div class="quizzy_result">
      <h1><?php echo $endQuizMessage; ?></h1>
      <p>You scored <span class="quizzy_result_score"><?php echo $score;?></span> out of 
      <span class="quizzy_result_max"><?php echo $maxPossible;?></span> possible points!</p>
    
<?php
  
  //figure out a percentage score, then use the grading information in the xml data to put some more stuff up
  $percentage = ($score / $maxPossible) * 100;
  
  //find the correct scoreRange
  $scoreRange = NULL;
  foreach($quiz->grading->range as $range) {
    //take care of 0 boundary case in easiest way possible.
    if(intval($range['start']) == 0) $range['start'] = -1;
    if(intval($range['start']) < $percentage && intval($range['end']) >= $percentage) {
      $scoreRange = $range;
      break;
    }
  }
  
?>
    
      <p>Grade: <span class="quizzy_result_grade quizzy_result_grade_<?php echo $scoreRange->grade; ?>"><?php echo $scoreRange->grade; ?></span> (<?php printf('%.1f%%', $percentage); ?>)</p>
      <?php if(isset($scoreRange->img)) { ?>
        <div class="quizzy_result_img"><img src="<?php echo $picDir . $scoreRange->img['src'];?>" alt="<?php echo $scoreRange->img['alt'];?>" ></div>
      <?php } ?>
      <p class="quizzy_result_rank quizzy_result_rank_<?php echo $scoreRange->rank; ?>"><?php echo $scoreRange->rank; ?></p>
      <div class="quizzy_result_foot"><input type="submit" onclick="restartQuizzy();" value="Do a different Quiz"></div>
    </div>
<?php
    return;
  }