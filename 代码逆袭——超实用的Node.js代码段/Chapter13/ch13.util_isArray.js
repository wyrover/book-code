/**
 * Created by king on 15-6-24.
 *
 * ch13.util_isArray.js
 */
console.info("------   util isArray()   ------");
console.info();
var util = require('util'); // TODO: 引入常用工具（Util）模块
/**
 * return value is true
 */
console.log(util.inspect(util.isArray([])));
/**
 * return value is true
 */
console.log(util.inspect(util.isArray(new Array)));
/**
 * return value is false
 */
console.log(util.inspect(util.isArray({})));