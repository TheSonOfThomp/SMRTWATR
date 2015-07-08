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

  include_once 'util.php';

  $quest_no = isset($i) ? $i : $_GET['quest_no'];
  $opt_no = isset($o) ? $o : $_GET['opt_no'];
  $opt_xml = $quiz->question[$quest_no]->option[$opt_no];
  $opt_txt = isset($quiz) ? addElem($opt_xml->text) : '';
  $opt_pic_src = isset($quiz) ? addElem($opt_xml->img['src']) : '';
  $opt_pic_alt = isset($quiz) ? addElem($opt_xml->img['alt']) : '';
  $opt_score = isset($quiz) ? addElem($opt_xml->score) : '';
  $exp_txt = isset($quiz) ? addElem($opt_xml->explanation->text) : '';
  $exp_pic_src = isset($quiz) ? addElem($opt_xml->explanation->img['src']) : '';
  $exp_pic_alt = isset($quiz) ? addElem($opt_xml->explanation->img['alt']) : '';

  $main_opt_id = 'q' . $quest_no . '_o' . $opt_no;
?>
  <li class="sub_sect quest_opt" id="<?php echo $main_opt_id; ?>">
    <?php addHider($main_opt_id, TRUE); ?>
    <div class="sect_head_cont"><div class="sect_head">↕ Option </div><div id="<?php echo $main_opt_id; ?>_hval" class="hval_cont">Data</div></div>
    <div class="sect">
      <div class="group">
       <!--  <?php addPic('', 'Option picture', $opt_pic_src, $opt_pic_alt, 'float_right', 'opt_pic'); ?> -->
        <div class="group_title">Option Text</div>
        <input type="text" class="hval opt_txt float_left" value="<?php echo $opt_txt; ?>">
        <div class="correct_opt float_right">Correct: <input type="checkbox" class="short_input opt_score" value="1" <?php if (intval($opt_score)) echo "checked"; ?>></div>
      </div>
      <div class="group">
        <!-- <?php addPic('', 'Explanation picture', $exp_pic_src, $exp_pic_alt, 'float_right', 'opt_exp_pic'); ?> -->
        <div class="group_title">Explanation Text <textarea rows="3" cols="64" class="opt_exp"><?php echo $exp_txt; ?></textarea></div>
      </div>
    </div>
  </li>
