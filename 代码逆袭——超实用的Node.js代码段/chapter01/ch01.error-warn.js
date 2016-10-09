/**
 * Created by KING on 2014/12/23.
 *
 * ch01.error-warn.js
 */
var fs = require('fs');
var file = 'chapter01/error-warn.txt';
var encoding = 'UTF-8';
fs.readFile(file, encoding, function(err, data) {
    if(err) {
        console.error("error - \n %s", err);
        console.warn("warn - \n %s", err);
    } else {
        console.log(data);
    }
});