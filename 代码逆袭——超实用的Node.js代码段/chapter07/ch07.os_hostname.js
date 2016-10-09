/**
 * Created by king on 15-4-14.
 *
 * ch07.os_hostname.js
 */
console.info("------   OS hostname()   ------");
console.info();
var os = require('os');
var hostname = os.hostname();
console.info('操作系统主机名: ' + hostname);
console.info();
console.info("------   OS hostname()   ------");