/**
 * Created by king on 15-3-22.
 *
 * ch05.process-stderr.js
 */
console.info("------   Process stderr   ------");
console.info();
var fs = require('fs');
var file = 'err.txt';
var encoding = 'UTF-8';
fs.readFile(file, encoding, function(err, data) {
    if(err) {
        setTimeout(function() {
            process.stderr.write('err: ' + err + '\n');  // TODO: 打印输出标准错误流
        }, 1000);
    } else {
        console.log(data);
    }
});
console.info();
console.info("------   Process stderr   ------");