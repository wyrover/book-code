/**
 * Created by king on 15-2-12.
 *
 * ch02.module-exports-compare-main.js
 */
console.log();
var obj = require('./ch02.module-exports-compare-obj.js');
var user = new obj('king', 26);
user.userinfo();
console.log(user.tag);
console.log();