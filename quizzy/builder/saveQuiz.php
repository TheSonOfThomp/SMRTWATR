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

	/*
	 * DESCRIPTION:
	 * 	 passed all data from the quizBuilder in GET to export as an xml file to the user.
	 *
	 */


  //ALL this function does is return the string value of the corresponding variable
  //in GET. it's useful to keep me from doing $_POST('val') which I've done several times
  // already.
  function getv($val){
    return strval($_POST[$val]);
  }

  //sees if the indicated $val is set in _GET. like above, this is for my protection
  function gotv($val){
    return isset($_POST[$val]);
  }

  //will see if the user set a picture for the indicated $sel and assigns the proper
  //tags to $parent.
  function addImg($sel, &$parent) {
    if($_POST[$sel.'_pic_src'] != '') {
      $img = $parent->addChild('img');
      $img->addAttribute('src', getv($sel.'_pic_src'));
      if($_POST[$sel.'_pic_alt'] != '')
        $img->addAttribute('alt', getv($sel.'_pic_alt'));
    }
  }

  //get a barebones quiz going
  $xmlstr = <<<XML
<?xml version="1.0" encoding="utf-8"?>
<quizzes><quiz></quiz></quizzes>
XML;
  $xml = new SimpleXMLElement($xmlstr);
  $quiz = $xml->quiz[0];

  if (isset($_POST['quiz_file_path'])) {
    $quiz_file_path = $_POST['quiz_file_path'];
  } 
  else {
    $quiz_file_path = '../quizzy/quizzes/textQuiz_.xml';
  }

  $QUIZ_FILE = fopen($quiz_file_path, 'c');
  

  //data
  $quiz->addChild('title', getv('data_txt'));
  addImg('quiz', $quiz);
  //if a description tag is needed
  if(gotv('data_desc') || gotv('data_pic_src'))
  {
    $desc = $quiz->addChild('description');

    if(isset($_POST['data_desc']))
      $desc->addChild('text', getv('data_desc'));

    //addImg('quiz_desc', $desc);
  }



  //grade range
  // $grading = $quiz->addChild('grading');
  // $i = 0;
  // $cond = 'gr0_start';
  // while(gotv($cond)){
  //   $grsel = 'gr'.$i;

  //   if(getv($grsel.'_start') != '')
  //   {
  //     $range = $grading->addChild('range');
  //     $range->addAttribute('start', getv($grsel.'_start'));
  //     $range->addAttribute('end', getv($grsel.'_end'));
  //     $range->addChild('grade', getv($grsel.'_grade'));
  //     $range->addChild('rank', getv($grsel.'_txt'));

  //     //addImg($grsel, $range);
  //   }

  //   //move on if there is another grade to process
  //   ++$i;
  //   $cond = 'gr'.$i.'_start';
  // }



  //Iterate thru questions
  $i = 0;
  $qcond = 'q0_txt';
  while(gotv($qcond)) {
    //echo $i.'<br/>';
    $qsel = 'q'.$i;

    //if the text field for this question is empty, don't add the question
    if(getv($qsel.'_txt') != '' || getv($qsel.'_pic_src') != '')
    {
      $quest = $quiz->addChild('question');
      $quest->addChild('text', getv($qsel.'_txt'));
      $type = getv($qsel.'_type');
      switch ($type) {
        case 'Radio Buttons':
        	$type = 'radio';
          break;
        case 'Checkboxes':
        	$type = 'checkbox';
          break;
      }
      $quest->addAttribute('type', $type);
      //addImg($qsel, $quest);


      //Iterate thru options
      $j = 0;
      $ocond = 'q'.$i.'_o0_txt';
      while(gotv($ocond)){
        $osel = 'q'.$i.'_o'.$j;

        //if the text field is empty, don't add the question
        if(getv($osel.'_txt') != '' || getv($qsel . '_pic_src') != '')
        {
          $opt = $quest->addChild('option');
          $opt->addChild('text', getv($osel.'_txt'));
          $opt->addChild('score', getv($osel.'_score'));
          //addImg($osel, $opt);

          //explanation
          // $exp = $opt->addChild('explanation');
          // $exp->addChild('text', getv($osel.'_exp'));
          // addImg($osel.'_exp', $exp);
        }

        //move on if there's another
        ++$j;
        $ocond = 'q'.$i.'_o'.$j.'_txt';
      }
    }

    $quest->addChild('dyk', getv($qsel.'_dyk'));

    //move on if there's another
    ++$i;
    $qcond = 'q'.$i.'_txt';
  }

  //this header will make the browser handle the file properly.
  header("Content-Type: text/xml; charset=utf-8");
  $dom = dom_import_simplexml($xml)->ownerDocument;
  $dom = new DOMDocument("1.0");
  $dom->preserveWhiteSpace = false;
  $dom->formatOutput = true;
  $dom->loadXML($xml->asXML());
  fwrite($QUIZ_FILE, $dom->saveXML());
  fclose($QUIZ_FILE);
  echo $dom->saveXML();

?>