/**
 * Created by king on 15-6-24.
 *
 * ch13.util_isRegExp.js
 */
console.info("------   util isRegExp()   ------");
console.info();
var util = require('util'); // TODO: 引入常用工具（Util）模块
/**
 * return value is true
 */
console.log(util.inspect(util.isRegExp(/some regexp/)));
/**
 * return value is true
 */
console.log(util.inspect(util.isRegExp(new RegExp('another regexp'))));
/**
 * return value is false
 */
console.log(util.inspect(util.isRegExp({})));