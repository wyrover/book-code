/**
 * Created by king on 15-2-12.
 *
 * ch02.module-conflict-main.js
 */
console.log();
console.log('main starting');
var a = require('./ch02.module-conflict-a.js');
var b = require('./ch02.module-conflict-b.js');
console.log('in main, a.done=%j, b.done=%j', a.done, b.done);
console.log();