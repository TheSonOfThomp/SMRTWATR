<?php
  /*
   * This file is part of quizzyBuilder.
   *
   * quizzyBuilder is free software: you can redistribute it and/or modify
   * it under the terms of the GNU Affero General Public License as
   * published by the Free Software Foundation, either version 3 of
   * the License, or (at your option) any later version.
   *
   * quizzyBuilder is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU Affero General Public License for more details.
   *
   * You should have received a copy of the GNU Affero General Public
   * License along with quizzyBuilder. If not, see <http://www.gnu.org/licenses/>.
   */
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
	<head>
		<title>Quiz Builder</title>
    <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=utf-8">
    <!--don't even give IE8 the option for wonky-mode.-->
    <META http-equiv="X-UA-Compatible" content="IE=8"/>

    <script type="text/javascript" src="lib/jquery-1.4.2.js"></script>
    <!-- <script type="text/javascript" src="../quizzy/lib/jquery.loading.js"></script>-->
    <script type="text/javascript" src="lib/jquery-ui-1.7.2.custom.min.js"></script>
    <script type="text/javascript" src="quizzyBuilder.js" charset="utf-8"></script>
    <link rel="stylesheet" type="text/css" href="quizzyBuilder.css" charset="utf-8">

    <!-- overflow:hidden in IE is currently breaking mask calcs :( -->
    <!--[if IE]>
      <style type="text/css">
        .loading-masked { overflow: visible; }
      </style>
    <![endif]-->
    <!--[if lte IE 6]>
      <style type="text/css">
        #load_exclusive {opacity: 50;}
      </style>
    <![endif]-->

<?php

  $AUTOLOAD_SMRT_QUIZ = true;
  $quiz_file_path = '../quizzy/quizzes/testQuiz.xml';
  $filename = substr($quiz_file_path, 18);
  //$fileText = substr($filename, 0, length($filename - 4));

  //check for upload error, make sure it was an xml file
  if ($AUTOLOAD_SMRT_QUIZ) {
    //now we know we have the uploaded file in $_FILES['file']['tmp_name'] so go ahead and open it up.
    $quiz_XML= simplexml_load_file($quiz_file_path);
    $quiz = $quiz_XML->quiz;
  }
  else if (isset($_FILES['file'])) {
    if ($_FILES['file']['error'] > 0)
    {
      echo '<script type="text/javascript">alert("Error opening uploaded file! Please try again.");</script>';
    }
    elseif ($_FILES['file']['type'] != 'text/xml')
    {
      echo '<script type="text/javascript">alert("Uploaded file was not an XML file. Please select the quiz XML file to upload.");</script>';
    }
    else {
      //now we know we have the uploaded file in $_FILES['file']['tmp_name'] so go ahead and open it up.
    	$quiz_XML= simplexml_load_file($_FILES['file']['tmp_name']);
    	$quiz = $quiz_XML->quiz;
?>
<script type="text/javascript">
  loadedQuiz = true;
  numQuestions = <?php echo count($quiz->question) - 1; ?>;
  numRanges = <?php echo count($quiz->grading->range) - 1; ?>;
</script>
<?php
    }
  }
?>

	</head>
	<body>
    <div id="load_exclusive"></div>
	  <div id="content">
      <h1 style="text-align: center; ">Quiz Editor</h2>
      <form method="post" action="saveQuiz.php" id="quizzyBuilder" target="_blank">
        <div id="note">
          <p>You can now re-arrange questions and their options by simply clicking and dragging the element. You can also load up a previously created quiz by clicking 'Load Quiz' below.</p>
          <p>If you encounter any problems, please report all issues to our <a href="https://sourceforge.net/apps/trac/quizzy/wiki/WikiStart">Trac</a>.</p>
        </div>
        <?php
          include 'addQuizInfo.php';
          //include 'addGrading.php';

          echo '<ul id="questions_container" class="dragging_container">';
          $to_add_q = isset($quiz) ? count($quiz->question) : 3;
          for ($i = 0; $i < $to_add_q; ++$i) {
            include 'addQuestion.php';
          }
          echo '</ul>';
        ?>
        <div class="hide_all_cont hide_all" id="hide_all">[Hide All]</div>
        <div class="hider_all hide_all" id="hide_all_quests" style="margin-top:45px;">[Hide All Questions]</div>
        <div class="main button" style="width:305px; float:left;">
          <h2 style="cursor:pointer;" id="quest_add">Add another Question</h2>
        </div>
        <div class="main button" style="width:300px; clear:both; float: right; margin: 100px 0px 100px 0px;">
          <h2 style="cursor:pointer;" id="save">Overwrite <?php echo $filename; ?></h2>
        </div>
        <input class="hidden" name="quiz_file_path" value="<?php echo $quiz_file_path; ?>" />
      </form>

      <form method="post" action="index.php" id="load_form" enctype="multipart/form-data">
        <div class="main button" style="width: 143px; float: left; margin: 100px 0px 100px 0px;">
          <h2 style="cursor:pointer;" id="load">Load Quiz</h2>
        </div>

        <div class="main" id="load_input">
          <div class="group_title">File to Load</div>
          <input type="file" name="file" id="file" />
          <div class="sect">
            <div class="float_right"><h2 style="cursor:pointer;" id="load_load">Load Quiz</h2></div>
            <div class="float_left"><h2 style="cursor:pointer;" id="load_cancel">Cancel</h2></div>
          </div>
        </div>
      </form>

      <script type="text/javascript">
        console.log('<?php echo $filename ?>');
      </script>
      <div style="clear: both; margin-top: 100px;">
        <p>The fields in this form are not validated. Make sure your input makes sense.</p>
        <p><a href="../index.php">Return to quizzy main</a></p>
      </div>

    </div>
  </body>
</html>
