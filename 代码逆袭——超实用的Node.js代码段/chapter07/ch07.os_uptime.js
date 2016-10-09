/**
 * Created by king on 15-4-15.
 *
 * ch07.os_uptime.js
 */
console.info("------   OS uptime()   ------");
console.info();
var os = require('os');
var uptime = os.uptime();
console.info('操作系统已经运行了: ' + uptime + ' seconds.');
console.info();
var sTime = convertTime(uptime);
console.info('操作系统已经运行了: ' + sTime);
console.info();
console.info("------   OS uptime()   ------");
/**
 * Format time to Hours/Minutes/Seconds
 * @param uptime
 * @returns {string}
 */
function convertTime(uptime) {
    var runTime;
    var hour;
    var minute;
    var second;
    hour = parseInt(uptime / 3600);
    minute = parseInt((uptime - hour * 3600) / 60);
    second = uptime - hour * 3600 - minute * 60;
    runTime = hour + ' hours ' + minute + ' minutes ' + second + ' seconds.';
    return runTime;
}