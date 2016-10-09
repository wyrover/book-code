/**
 * Created by king on 15-5-8.
 *
 * ch09.path_normalize.js
 */
console.info("------   path normalize()   ------");
console.info();
var path = require('path'); // TODO: 引入路径处理模块
/**
 * 测试 path.normalize() 函数方法
 * @type {string}
 */
var path_a = "/home//king";
console.info('path.normalize("/home//king") is : ' + path.normalize(path_a));
console.info();
var path_b = "/home//king/";
console.info('path.normalize("/home//king/") is : ' + path.normalize(path_b));
console.info();
var path_c = "/home///king//";
console.info('path.normalize("/home///king//") is : ' + path.normalize(path_c));
console.info();
var path_d = "/home/king/.";
console.info('path.normalize("/home/king/.") is : ' + path.normalize(path_d));
console.info();
var path_e = "/home/king//.";
console.info('path.normalize("/home/king//.") is : ' + path.normalize(path_e));
console.info();
var path_f = "/home/king//.abc";
console.info('path.normalize("/home/king//.abc") is : ' + path.normalize(path_f));
console.info();
var path_g = "/home/king//..";
console.info('path.normalize("/home/king//..") is : ' + path.normalize(path_g));
console.info();
var path_h = "/home/king//..abc";
console.info('path.normalize("/home/king//..abc") is : ' + path.normalize(path_h));
console.info();
var path_i = "/home/king/../..";
console.info('path.normalize("/home/king/../..") is : ' + path.normalize(path_i));
console.info();
var path_j = "/home/king//..//..";
console.info('path.normalize("/home/king//..//..") is : ' + path.normalize(path_j));
console.info();