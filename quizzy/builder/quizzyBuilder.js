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

var showSpeed = 'def';
var hideSpeed = 'slow';
var numQuestions = 2;
var numOpts = new Array();
var numRanges = 4;
var loadedQuiz = false;

// When the document is ready get things goin
$(document).ready(function() {
	// all loading stuff is triggered by ajax
	//$.loading.onAjax = true;

	// make sortable stuff sortable
	$('#grades_container').sortable({ cursor: 'n-resize', handle: '.sect_head_cont'});
  $('#questions_container').sortable({ cursor: 'n-resize', handle: '.sect_head_cont:first'});

	// add click handler for adding quizRanges and Questions
	$('#grading_add').click(addRange);
	$('#quest_add').click(addQuest);

	// click handler for saving quizzes
	$('#save').click(save);

	// click handlers for hiding stuff
	$('#hide_all').click(function() { hideAll('#hide_all', '', ''); });
	$('#hide_all_grades').click(function() { hideAll('#hide_all_grades', '.grade_range', ' Grades');	});
  $('#hide_all_quests').click(function() { hideAll('#hide_all_quests', '.question', ' Questions');  });

  // load actions
  setupLoader();

	 // init the numOpts array if it isn't already
   if(!loadedQuiz) {
		 for (var i = 0; i < 3; i++) {
		 	 numOpts[i] = 2;
		 }
   }
});

function setupLoader() {
  // click handler on the first load button
  $('#load').click(showLoad);

  // hide the load input container, then set its height and position
  $('#load_input')
    .css('top', ($(window).height() - $('#load_input').height())/2  + 'px')
    .css('left', ($(window).width() - $('#load_input').width())/2  + 'px');
}

function showLoad() {
  // scroll the window up to the top
  $('html, body').animate({scrollTop: 0}, 1000, function () {
    // then fade in the background and the container
    $('#load_input').fadeIn(showSpeed);
    $('#load_exclusive').fadeIn(showSpeed);
  });

  // add click handlers to the second load button and the cancel button
  $('#load_load').click( function() { $('#load_form').submit(); } );
  $('#load_cancel').click( hideLoad );
}

function hideLoad() {
  // fade out the background and the container
  $('#load_input').fadeOut(hideSpeed);
  $('#load_exclusive').fadeOut(hideSpeed);
}

function hideAll(id, container, text) {
	// remove all click handlers
	$(id).unbind();

	// grab all of the grade_ranges and get their ids into an array
	var hiders = $( container + ' .hide' ).map(getId).get();

	// hide all of them
	for (var i in hiders) {
		// need to get rid of that '_hider' bit on the end of the id.
		hiders[i] = hiders[i].replace('_hider', '');
		hide(hiders[i]);
	}

  // then fade out the hider
	$(id).fadeOut(hideSpeed, function() {
		// change click handler
		$(id).click(function () {
			showAll(id, container, text);
		});

    // change the hider's text, change its class show it and change its click handeler
    $(id).html('[Show All' + text + ']').removeClass('hide').addClass('show').fadeIn(showSpeed);
	});
}

function showAll(id, container, text) {
	// remove all click handlers
	$(id).unbind();

	// grab all of the grade_ranges and get their ids into an array
	var showers = $( container + ' .show' ).map(getId).get();

	// hide all of them
	for (var i in showers) {
		// need to get rid of that '_hider' bit on the end of the id.
		showers[i] = showers[i].replace('_hider', '');
		show(showers[i]);
	}

  // then fade out the hider
	$(id).fadeOut(hideSpeed, function() {
		// change click handler
		$(id).click(function () {
			hideAll(id, container, text);
		});

    // change the hider's text, change its class show it and change its click handeler
    $(id).html('[Hide All' + text + ']').removeClass('show').addClass('hide').fadeIn(showSpeed);
	});
}

function addRange() {
	$.loading();
	$.get('addRange.php', {range_no: numRanges + 1}, function(data) {
		// add to end of grade ranges
		$('#grades_container').append(data);

		// increment number of ranges
		numRanges++;
	});
}

function addQuest() {
	//$.loading();

  numQuestions++;
	$.get('addQuestion.php', {quest_no: numQuestions}, function(data) {
		// add that question to the questions list
		$('#questions_container').append(data);

		// init its options entry in that array
		numOpts[numQuestions] = 3;
	});
}

function addOpt(toQuestion){
	$.loading();
  numOpts[toQuestion]++;
	$.get('addOption.php', {quest_no: toQuestion, opt_no: numOpts[toQuestion]}, function(data) {
		// add it to the list of options
		$('#q' + toQuestion + '_container').append(data);
	});
}


function hide(id) {
	// Don't do anything if it's already hidden
	if ($('#' + id + '_hider:first').hasClass('show'))
		return;

	// Remove all click handlers
	$('#' + id + '_hider:first').unbind();
   // grab all the children divs of id where their class is not sect_head and slide them up
	$('#' + id + ' > .sect').slideUp(hideSpeed, function(){
    // get old hval
    var oldHval = $('#' + id + '_hval').html();
		// fade out the id_hval, change its contents, and fade it back in
		$('#' + id + '_hval').fadeOut(hideSpeed, function() {
			// get the value to use from id .hval
		  $(this).html("'" + $('#' + id + ' .hval').attr('value') + "'");
		  $(this).fadeIn(showSpeed);
		});

    // then fade out the hider
		$('#' + id + '_hider').fadeOut(hideSpeed, function() {
			// change click handler
			$('#' + id + '_hider:first').click(function () {
				show(id, oldHval);
			});

      // change the hider's text, change its class show it and change its click handeler
      $('#' + id + '_hider').html('[Show]').removeClass('hide').addClass('show').fadeIn(showSpeed);
		});
	});
}

function show(id, oldHval) {
	// Don't do anything if it's already shown
	if ($('#' + id + '_hider:first').hasClass('hide'))
		return;

	// remove all click handlers
	$('#' + id + '_hider:first').unbind();

   // grab all the children divs of id where their class is not sect_head and slide them up
  $('#' + id + ' > .sect').slideDown(hideSpeed, function(){
      // fade out the id_hval, change its contents, and fade it back in
      $('#' + id + '_hval').fadeOut(hideSpeed, function() {
        // get the value to use from id > .hval
        $(this).html(oldHval);
        $(this).fadeIn(showSpeed);
      });

    // then fade out the hider
    $('#' + id + '_hider').fadeOut(hideSpeed, function() {
    	// change its click handeler
			$('#' + id + '_hider:first').click(function () {
      	hide(id);
			});
      // change the hider's text, change its class show it
      $('#' + id + '_hider').html('[Hide]').removeClass('show').addClass('hide').fadeIn(showSpeed);
    });
  });
}

function addDeleter(id) {
	// add click handler
	$('#' + id + '_del').click(function() {
		// make sure they do want to
		var answer = confirm('Are you sure you want to delete this? You cannot undo this.');

		if(answer) {
			// remove it
			$('#' + id).slideUp(hideSpeed, function() {
				$('#' + id).remove();
			});
		}
	});
}


function save() {
	// essentially, we need to loop through all the quiz ranges, questions, and the questions' options
	// setting names for everything. then we can submit the form.

	// grab all of the grade_ranges and get their ids into an array
	var gradeStarts = $('.grade_range').map(getId).get();
	for (var i in gradeStarts) {
		// set name for the fields in this grade_range.
		// range start
		$('#' + gradeStarts[i] + ' .grade_start').attr('name', 'gr' + i + '_start');
		// range end
		$('#' + gradeStarts[i] + ' .grade_end').attr('name', 'gr' + i + '_end');
		// range letter
		$('#' + gradeStarts[i] + ' .grade_letter').attr('name', 'gr' + i + '_grade');
		// range rank
		$('#' + gradeStarts[i] + ' .grade_rank').attr('name', 'gr' + i + '_txt');
		// pic src
		$('#' + gradeStarts[i] + ' .pic_src').attr('name', 'gr' + i + '_pic_src');
		// pic alt
		$('#' + gradeStarts[i] + ' .pic_alt').attr('name', 'gr' + i + '_pic_alt');
	}

	// grab all of the questions and get their ids into an array
	var questions = $('.question').map(getId).get();
	for(var i in questions) {
		// set the name for the fields
		// question text
		$('#' + questions[i] + ' .quest_txt').attr('name', 'q' + i + '_txt');
		// question type
		$('#' + questions[i] + ' .quest_type').attr('name', 'q' + i + '_type');

		// pic src
		$('#' + questions[i] + ' .quest_pic_src').attr('name', 'q' + i + '_pic_src');
		// pic alt
		$('#' + questions[i] + ' .quest_pic_alt').attr('name', 'q' + i + '_pic_alt');
		$('#' + questions[i] + ' .quest_dyk').attr('name', 'q' + i + '_dyk');

		// get all the options for this question and get their ids into an array
		var opts = $('#' + questions[i] + ' .quest_opt').map(getId).get();
		for(var j in opts) {
			// set all the names
			var sel = '#' + questions[i] + ' #' + opts[j];
			var name = 'q' + i + '_o' + j;
			// option text
			$(sel + ' .opt_txt').attr('name', name + '_txt');
			// option score value
			$(sel + ' .opt_score').attr('name', name + '_score');
			// pic src
			$(sel + ' .opt_pic_src').attr('name', name + '_pic_src');
			// pic alt
			$(sel + ' .opt_pic_alt').attr('name', name + '_pic_alt');
			// range rank
			$(sel + ' .opt_exp').attr('name', name + '_exp');
			// pic src
			$(sel + ' .opt_exp_pic_src').attr('name', name + '_opt_pic_src');
			// pic alt
			$(sel + ' .opt_exp_pic_alt').attr('name', name + '_opt_pic_alt');
		}
	}

	// everything is properly named now. Just need to submit the form.
	$('#quizzyBuilder').submit();
}

function getId() {
	return $(this).attr('id');
}