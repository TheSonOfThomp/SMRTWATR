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
?>

<div class="main" id="quiz_grading">
  <?php addHider('quiz_grading'); ?>
  <div class="sect_head_cont">Grading</div>
  <div class="sect">
    <ul id="grades_container" class="dragging_container">
      <?php
        $to_add_r = isset($quiz) ? count($quiz->grading->range) : 5;
        for ($i=0; $i < $to_add_r; $i++) {
          include 'addRange.php';
        }
      ?>
    </ul>
    <div class="hider_all hide_all" id="hide_all_grades">[Hide All Grades]</div>
    <div class="sub_sect" style="width:275px;">
      <h2 style="cursor:pointer;" id="grading_add">Add another range</h2>
    </div>
  </div>
</div>