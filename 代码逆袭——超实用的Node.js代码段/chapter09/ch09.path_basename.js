/**
 * Created by king on 15-5-11.
 *
 * ch09.path_basename.js
 */
console.info("------   path basename()   ------");
console.info();
var path = require('path'); // TODO: 引入路径处理模块
/**
 * 测试 path.basename() 函数方法
 * @type {string}
 */
var path_a = "test";
var path_b = "test.txt";
console.info('path.resolve("test","test.txt") is : ');
var path_resolve = path.resolve(path_a, path_b);
console.info(path_resolve);
console.info();
console.info('path.extname("test.txt") is : ');
var path_extname = path.extname(path_resolve);
console.info(path_extname);
console.info();
/**
 * 测试 path.basename("test.txt", ".txt") 方法
 */
console.info('path.basename("test.txt", ".txt") is : ');
var path_basename = path.basename(path_resolve, path_extname);
console.info(path_basename);
console.info();
/**
 * 测试 path.basename("test.txt", "st.txt") 方法
 */
console.info('path.basename("test.txt", "st.txt") is : ');
var path_basename_st = path.basename(path_resolve, "st.txt");
console.info(path_basename_st);
console.info();
/**
 * 测试 path.basename("test.txt", "test.txt") 方法
 */
console.info('path.basename("test.txt", "test.txt") is : ');
var path_basename_test = path.basename(path_resolve, "test.txt");
console.info("variable path_basename_test is : " + path_basename_test);
console.info();