/**
 * Created by king on 15-4-14.
 *
 * ch07.os_tmpdir.js
 */
console.info("------   OS tmpdir()   ------");
console.info();
var os = require('os');
var type = os.type();
var tmpdir = os.tmpdir();
console.info('当前' + type + '系统临时目录: ' + tmpdir);
console.info();
console.info("------   OS tmpdir()   ------");