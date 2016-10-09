/**
 * Created by king on 15-5-11.
 *
 * ch09.path_extname.js
 */
console.info("------   path extname()   ------");
console.info();
var path = require('path'); // TODO: 引入路径处理模块
/**
 * 测试 path.extname() 函数方法
 * @type {string}
 */
var path_a = "test";
var path_b = "test.txt";
var path_resolve = path.resolve(path_a, path_b);
console.info('path.resolve("test","test.txt") is : ');
console.info(path_resolve);
console.info();
console.info('path.extname("test.txt") is : ');
console.info(path.extname(path_resolve));
console.info();