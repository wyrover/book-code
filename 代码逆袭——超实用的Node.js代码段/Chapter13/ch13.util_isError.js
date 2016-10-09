/**
 * Created by king on 15-6-24.
 *
 * ch13.util_isError.js
 */
console.info("------   util isError()   ------");
console.info();
var util = require('util'); // TODO: 引入常用工具（Util）模块
/**
 * return value is true
 */
console.log(util.inspect(util.isError(new Error())));
/**
 * return value is true
 */
console.log(util.inspect(util.isError(new TypeError())));
/**
 * return value is false
 */
console.log(util.inspect(util.isError({
    name: 'Error',
    message: 'an error occurred'
})));