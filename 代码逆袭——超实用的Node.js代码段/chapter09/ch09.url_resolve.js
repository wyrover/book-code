/**
 * Created by king on 15-5-13.
 *
 * ch09.url_resolve.js
 */
console.info("------   url parse()   ------");
console.info();
var url = require('url');// TODO: 引入路径处理模块
/**
 * 测试 url.resolve() 函数方法
 * @type {string}
 */
console.info(url.resolve('/one/two/three', 'four'));
console.info(url.resolve('/one/two/three/', 'four'));
console.info(url.resolve('/one/two/three', '/four'));
console.info(url.resolve('/one/two/three/', '/four'));
console.info(url.resolve('http://www.example.com/one', 'two'));
console.info(url.resolve('http://www.example.com/one', '/two'));
console.info(url.resolve('http://www.example.com/one/', 'two'));
console.info(url.resolve('http://www.example.com/one/', '/two'));