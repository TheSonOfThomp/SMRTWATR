// GAMEPLAY TIMER FUNCTIONS //
//Based on the codepen by David M.
//http://codepen.io/wasser/
// rAF - https://raw.githubusercontent.com/darius/requestAnimationFrame/master/requestAnimationFrame.min.js
var timerT = 0;
"use strict";
if (!Date.now) Date.now = function() {
  return (new Date).getTime()
};
(function() {
  var n = ["webkit", "moz"];
  for (var e = 0; e < n.length && !window.requestAnimationFrame; ++e) {
    var i = n[e];
    window.requestAnimationFrame = window[i + "RequestAnimationFrame"];
    window.cancelAnimationFrame = window[i + "CancelAnimationFrame"] || window[i + "CancelRequestAnimationFrame"]
  }
  if (/iP(ad|hone|od).*OS 6/.test(window.navigator.userAgent) || !window.requestAnimationFrame || !window.cancelAnimationFrame) {
    var a = 0;
    window.requestAnimationFrame = function(n) {
      var e = Date.now();
      var i = Math.max(a + 16, e);
      return setTimeout(function() {
        n(a = i)
      }, i - e)
    };
    window.cancelAnimationFrame = clearTimeout
  }
})();

  // It's the final countdown
  // Don't use this function unless you're sure there is no timer set yet
  // Otherwise use resetTimer()
function setTimer(time) {
    'use strict';
  var totalTime = time-1,
      currentTime = totalTime,
      percentTime = null,
      percentLin = null,
      timerId = null,
      timerText = document.querySelector('.text'),
      timerCircle = document.querySelector('.circle');

    timerId = function() {
      if (currentTime === -1) {
        return;
      }
      timerText.textContent = currentTime;
      percentTime = Math.round((currentTime / totalTime) * 100);
      timerCircle.style.strokeDashoffset = percentTime - 100;

      var linearP = Math.round((currentTime / (totalTime+5)) * 100);

      timerT = setTimeout(function() {
        timerText.textContent = currentTime;
        currentTime -= 1;
        incrementPbar(linearP);
        requestAnimationFrame(timerId);
      }, 1000);
    };
    timerId();
}

//AdamT
function killTimer(){
  clearTimeout(timerT);
}

//AdamT
function resetTimer(time){
  if(timerT)
    killTimer();
  setTimer(time);
}

function showRadialTimer(){
  $('#linearTimer').hide();
  $('#radialTimer').show();
}

function showLinearTimer(){
  $('#linearTimer').show();
  $('#radialTimer').hide();
}

function incrementPbar(percent){
  $('#bottom-progress-bar').css('width', percent.toString() + "%");
}