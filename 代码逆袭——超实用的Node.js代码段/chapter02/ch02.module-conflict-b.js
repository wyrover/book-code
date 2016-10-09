/**
 * Created by king on 15-2-12.
 *
 * ch02.module-conflict-b.js
 */
console.log('b starting');
exports.done = false;
var a = require('./ch02.module-conflict-a.js');
console.log('in b, a.done = %j', a.done);
exports.done = true;
console.log('b done');