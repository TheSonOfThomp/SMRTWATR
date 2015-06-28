// GAMEPLAY TIMER FUNCTIONS //
//By David M.
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
function setTimer(time) {
    'use strict';
  var totalTime = time,
      currentTime = totalTime,
      percentTime = null,
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

      timerT = setTimeout(function() {
        timerText.textContent = currentTime;
        currentTime -= 1;
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

