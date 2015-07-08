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

  function addPic($name, $title_text, $src = '', $alt = '', $wrapper_class = '', $txt_class='') {
    $class = empty($wrapper_class) ? '' :  ' class="pic ' . $wrapper_class . '"';
    $src_name = empty($name) ? '' : ' name="' . $name . '_pic_src" id="data_' . $name . '_src"';
    $alt_name = empty($name) ? '' : ' name="' . $name . '_pic_alt" id="data_' . $name . '_alt"';
    $title = empty($title_text) ? '' : '<p class="group_title">' . $title_text . '</p>';?>
    <div <?php echo $class; ?>>
      <?php echo $title; ?>
      <p>Src: <input type="text"<?php echo $src_name; ?> class="pic_src <?php echo $txt_class; ?>_src" value="<?php echo $src; ?>"></p>
      <p>Alt: &nbsp;<input type="text"<?php echo $alt_name; ?> class="pic_alt <?php echo $txt_class; ?>_alt" value="<?php echo $alt; ?>"></p>
    </div>
<?php
  }


  function addHider($id_name, $show_del = FALSE) {
?>
    <div class="hider hide" id="<?php echo $id_name;?>_hider">[Hide]</div>
<?php
    if ($show_del) {
?>
    <div class="delete" id="<?php echo $id_name; ?>_del">[Delete]</div>
    <script type="text/javascript">addDeleter('<?php echo $id_name; ?>');</script>
<?php } ?>
    <script type="text/javascript">
      $('#<?php echo $id_name; ?>_hider:first').click(function () {
        hide('<?php echo $id_name; ?>');
      });
    </script>
<?php
  }

  //returns the text of the specified element or '' if it is not set.
  function addElem($what) {
    if (isset($what))
      return $what;
    else
      return '';
  }

?>
