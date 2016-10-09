/**
 * Created by king on 15-6-24.
 *
 * ch13.util_format.js
 */
console.info("------   util format()   ------");
console.info();
var util = require('util'); // TODO: 引入常用工具（Util）模块
/**
 * return value is 'foo:%s'
 */
console.info(util.format('%s:%s', 'foo'));
/**
 * return value is 'foo:bar baz'
 */
console.info(util.format('%s:%s', 'foo', 'bar', 'baz'));
/**
 * return value is '1 2 3'
 */
console.info(util.format(1, 2, 3));