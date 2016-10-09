/**
 * Created by king on 15-2-12.
 *
 * ch02.module-conflict-a.js
 */
console.log('a starting');
exports.done = false;
var b = require('./ch02.module-conflict-b.js');
console.log('in a, b.done = %j', b.done);
exports.done = true;
console.log('a done');