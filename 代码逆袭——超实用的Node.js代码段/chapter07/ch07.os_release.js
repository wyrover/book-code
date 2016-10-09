/**
 * Created by king on 15-4-15.
 *
 * ch07.os_release.js
 */
console.info("------   OS release()   ------");
console.info();
var os = require('os');
var type = os.type();
console.info('操作系统类型: ' + type);
console.info();
var release = os.release();
console.info('操作系统发行版本: ' + release);
console.info();
console.info("------   OS release()   ------");