/**
 * Created by king on 15-2-11.
 *
 * ch03.syn-asyn.js
 */
function sleep(milliSeconds) {
    var startTime = new Date().getTime();
    while (new Date().getTime() < startTime + milliSeconds);
};
function synAsyn() {
    for (var i=0; i<5; i++)
    {
        console.info("before FOR: " + new Date());
        setTimeout(function(){
            console.info("in FOR: " + new Date());
        }, 3000); //睡眠2秒，然后再进行一下次for循环打印
        console.info("after FOR: " + new Date());
    }
}
synAsyn();