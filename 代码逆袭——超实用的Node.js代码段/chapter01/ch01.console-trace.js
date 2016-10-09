/**
 * Created by KING on 2015/1/7.
 *
 * ch01.console-trace.js
 */
console.trace();
console.trace("start");
var fs = require('fs');
var file = 'chapter01/not-found.txt';
var encoding = 'UTF-8';
fs.readFile(file, encoding, function(err, data) {
    console.trace("fs-start");
    if(err) {
        // console.error("error - \n %s", err);
        // console.warn("warn - \n %s", err);
    } else {
        // console.log(data);
    }
    console.trace("fs-end");
});
console.trace("end");