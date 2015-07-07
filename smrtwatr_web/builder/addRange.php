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
	 * 	 used to add a graderange to quizzyBuilder
	 *
	 * PARAMETERS passed in GET:
	 */

  include_once 'util.php';

  //this is kind of an ugly hack, but it works.
  $range_no = isset($i) ? $i : $_GET['range_no'];
  $grade_pic_src = isset($quiz) ? addElem($quiz->grading->range[$range_no]->img['src']) : '';
  $grade_pic_alt = isset($quiz) ? addElem($quiz->grading->range[$range_no]->img['alt']) : '';
  $range_start = isset($quiz) ? addElem($quiz->grading->range[$range_no]['start']) : '';
  $range_end = isset($quiz) ? addElem($quiz->grading->range[$range_no]['end']) : '';
  $rank_name = isset($quiz) ? addElem($quiz->grading->range[$range_no]->rank) : '';
  $rank_letter = isset($quiz) ? addElem($quiz->grading->range[$range_no]->grade) : '';
  $main_id = 'grade_range' . $range_no;
?>
  <li class="grade_range sub_sect" id="<?php echo $main_id; ?>">
    <?php addHider($main_id, TRUE); ?>
    <div class="sect_head_cont"><div class="sect_head">↕ Grade Range </div><div id="<?php echo $main_id; ?>_hval" class="hval_cont">Data</div></div>
    <div class="sect">
      <div class="group">
        <?php addPic('', 'Rank picture', $grade_pic_src, $grade_pic_alt, 'float_right'); ?>
        <div>If user's final grade is between <input type="text" class="short_input grade_start" value="<?php echo $range_start; ?>">% and <input type="text" class="short_input grade_end" value="<?php echo $range_end; ?>">%,</div>
        <div>give letter grade <input type="text" class="short_input grade_letter" value="<?php echo $rank_letter; ?>"> and assign the following rank:</div>
        <div><input type="text" class="hval grade_rank" value="<?php echo $rank_name; ?>"></div>
      </div>
    </div>
  </li>
