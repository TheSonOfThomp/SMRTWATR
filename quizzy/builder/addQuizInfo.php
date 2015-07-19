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
  //get stuff passed in get or use default empties
  $title_txt = isset($quiz) ? addElem($quiz->title) : '';
  $title_pic_src = isset($quiz) ? addElem($quiz->img['src']) : '';
  $title_pic_alt = isset($quiz) ? addElem($quiz->img['alt']) : '';
  $desc_txt = isset($quiz) ? addElem($quiz->description->text) : '';
  $desc_pic_src = isset($quiz) ? addElem($quiz->description->img['src']) : '';
  $desc_pic_alt = isset($quiz) ? addElem($quiz->description->img['alt']) : '';

?>

<div class="main" id="quiz_data">
  <?php addHider('quiz_data'); ?>
  <div class="sect_head_cont"><div class="sect_head">Quiz </div><div id="quiz_data_hval" class="hval_cont">Data</div></div>
  <div class="sect">
    <div class="group">
     <!--  <?php addPic('quiz', 'Quiz picture', $title_pic_src, $title_pic_alt, 'float_right'); ?> -->
      <div class="group_title">Title<input type="text" name="data_txt"  id="data_txt" class="hval" value="<?php echo $title_txt; ?>"></div>
    </div>
    <div class="group">
     <!--  <?php addPic('quiz_desc', 'Description picture', $desc_pic_src, $desc_pic_alt, 'float_right'); ?> -->
      <p class="group_title">Description</p><textarea rows="3" cols="64" name="data_desc" id="data_desc"><?php echo $desc_txt; ?></textarea>
    </div>
  </div>
</div>
