/**
 * Created by king on 15-5-9.
 *
 * ch09.path_resolve.js
 */
console.info("------   path resolve()   ------");
console.info();
var path = require('path'); // TODO: 引入路径处理模块
/**
 * 测试 path.resolve() 函数方法
 * @type {string}
 */
console.info("Current working directory : ");
console.info(process.cwd());
console.info();
var path_a = "ch09.path_resolve.js";
console.info('path.resolve("ch09.path_resolve.js") is : ');
console.info(path.resolve(path_a));
console.info();
var path_b = "test";
var path_c = "test.txt";
console.info('path.resolve("test","test.txt") is : ');
console.info(path.resolve(path_b, path_c));
console.info();