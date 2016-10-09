/**
 * Created by king on 15-6-24.
 *
 * ch13.util_isDate.js
 */
console.info("------   util isDate()   ------");
console.info();
var util = require('util'); // TODO: 引入常用工具（Util）模块
/**
 * return value is true
 */
console.log(util.inspect(util.isDate(new Date())));
/**
 * return value is false
 */
console.log(util.inspect(util.isDate(Date())));
/**
 * return value is false
 */
console.log(util.inspect(util.isDate({})));