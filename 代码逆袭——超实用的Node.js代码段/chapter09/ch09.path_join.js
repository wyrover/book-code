/**
 * Created by king on 15-5-8.
 *
 * ch09.path_join.js
 */
console.info("------   path join()   ------");
console.info();
var path = require('path'); // TODO: 引入路径处理模块
/**
 * 测试 path.join() 函数方法
 * @type {string}
 */
var path_a = "home";
var path_b = "king";
var path_c = "webstorm.js";
console.info('path.join("home","king","webstorm.js") is : ' + path.join(path_a, path_b, path_c));
console.info();
var path_d = "home";
var path_e = "king";
console.info('path.join("home",{},"king") is : ' + path.join(path_d, {}, path_e));
console.info();