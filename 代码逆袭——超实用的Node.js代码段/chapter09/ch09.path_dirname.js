/**
 * Created by king on 15-5-11.
 *
 * ch09.path_dirname.js
 */
console.info("------   path dirname()   ------");
console.info();
var path = require('path'); // TODO: 引入路径处理模块
/**
 * 测试 path.dirname() 函数方法
 * @type {string}
 */
var path_a = "test";
var path_b = "test.txt";
var path_resolve = path.resolve(path_a, path_b);
console.info('path.resolve("test","test.txt") is : ');
console.info(path_resolve);
console.info();
console.info("The file test.txt's dirname is : ");
console.info(path.dirname(path_resolve));
console.info();