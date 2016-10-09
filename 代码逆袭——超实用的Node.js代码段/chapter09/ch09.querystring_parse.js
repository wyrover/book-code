/**
 * Created by king on 15-5-13.
 *
 * ch09.querystring_parse.js
 */
console.info("------   querystring parse()   ------");
console.info();
var url = require('url');// TODO: 引入路径处理模块
var querystring = require('querystring');// TODO: 引入路径处理模块
/**
 * 测试 url.parse() 函数方法
 * @type {string}
 */
var queryUrl = "http://localhost:6666/index?name=king&gender=male&level=high";
console.info('typeof queryUrl is : ' + typeof url.parse(queryUrl));
console.info();
console.info('querystring.parse("queryUrl") is : ');
var url_parse = url.parse(queryUrl).query;
/**
 * 测试 querystring.parse() 函数方法
 * @type {*}
 */
var querystring_parse = querystring.parse(url_parse);
console.info(querystring_parse);
console.info();