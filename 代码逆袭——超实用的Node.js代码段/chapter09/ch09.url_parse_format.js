/**
 * Created by king on 15-5-13.
 *
 * ch09.url_parse_format.js
 */
console.info("------   url parse()   ------");
console.info();
var url = require('url');// TODO: 引入路径处理模块
/**
 * 测试 url.parse() 函数方法
 * @type {string}
 */
var queryUrl = "http://localhost:6666/index?name=king&gender=male&level=high";
console.info('typeof queryUrl is : ' + typeof url.parse(queryUrl));
console.info();
console.info('url.parse("queryUrl") is : ');
var url_parse = url.parse(queryUrl);    // TODO: 调用url.parse()方法
console.info(url_parse);
console.info();
/**
 * 测试 url.format() 函数方法
 */
console.info('url.format("url_parse") is : ');
var url_format = url.format(url_parse); // TODO: 调用url.format()方法
console.info(url_format);
console.info();