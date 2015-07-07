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
  $main_id = 'q' . $quest_no;
  $quest_pic_src = isset($quiz) ? addElem($quiz->question[$quest_no]->img['src']) : '';
  $quest_pic_alt = isset($quiz) ? addElem($quiz->question[$quest_no]->img['alt']) : '';
  $quest_txt = isset($quiz) ? addElem($quiz->question[$quest_no]->text) : '';
  $quest_dyk = isset($quiz) ? addElem($quiz->question[$quest_no]->dyk) : '';
?>

<div class="main question" id="<?php echo $main_id; ?>">
  <?php addHider($main_id, TRUE); ?>
  <div class="sect_head_cont"><div class="sect_head">↕ Question </div><div id="<?php echo $main_id; ?>_hval" class="hval_cont">Data</div></div>
  <div class="sect">
    <div class="group">
     <!--  <?php addPic('', 'Question picture', $quest_pic_src, $quest_pic_alt, 'float_right', quest_pic); ?> -->
      <div class="group_title">Question Text
        <textarea rows="3" cols="64" class="hval quest_txt"><?php echo $quest_txt; ?></textarea>
      </div>
    </div>
    <div class="group">
      <ul id="<?php echo $main_id; ?>_container" class="dragging_container">
        <?php
          $to_add = isset($quiz) ? count($quiz->question[$quest_no]->option) : 4;
          for ($o=0; $o < $to_add; $o++) {
            include 'addOption.php';
          }
          if(isset($quiz)) {
?>
      <script type="text/javascript">
        numOpts[<?php echo $quest_no; ?>] = <?php echo $to_add - 1; ?>;
      </script>
<?php
          }
        ?>
      </ul>
      <br/>
      <div class="group_title">Did You Know: <textarea rows="3" cols="82" class="hval quest_dyk"><?php echo $quest_dyk; ?></textarea></div>
      <div class="hider_all hide_all" id="hide_all_<?php echo $main_id; ?>_opts">[Hide All Options]</div>
      <div class="sub_sect button" style="width:275px;">
        <h2 style="cursor:pointer;" id="<?php echo $main_id; ?>_opt_add">Add another option</h2>
      </div>
      <script type="text/javascript">
        $('#<?php echo $main_id; ?>_opt_add').click(function () {
            addOpt(<?php echo $quest_no; ?>);
        });
        $('#<?php echo $main_id; ?>_container').sortable({ cursor: 'n-resize', handle: '.sect_head_cont'});
        $('#hide_all_<?php echo $main_id; ?>_opts').click(function() { hideAll('#hide_all_<?php echo $main_id; ?>_opts', '#<?php echo $main_id; ?>_container ', ' Options'); });
      </script>
    </div>
  </div>
</div>
