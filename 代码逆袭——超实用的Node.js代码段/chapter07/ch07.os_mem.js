/**
 * Created by king on 15-4-15.
 *
 * ch07.os_mem.js
 */
console.info("------   OS mem()   ------");
console.info();
var os = require('os'); // TODO: 引入OS模块
var totalmem = os.totalmem();
console.info('操作系统内存总量为: ' + totalmem + ' byte.');
console.info('操作系统内存总量为: ' + parseInt(totalmem / 1024 / 1024) + ' Mb.');
console.info();
var freemem = os.freemem();
console.info('操作系统空闲内存为: ' + freemem + ' byte.');
console.info('操作系统空闲内存为: ' + parseInt(freemem / 1024 / 1024) + ' Mb.');
console.info();
console.info("------   OS mem()   ------");