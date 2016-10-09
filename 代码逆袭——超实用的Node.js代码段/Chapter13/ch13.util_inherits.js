/**
 * Created by king on 15-6-24.
 *
 * ch13.util_inherits.js
 */
console.info("------   util inherits()   ------");
console.info();
var util = require('util'); // TODO: 引入常用工具（Util）模块
/**
 * 定义原型基类 Base
 * @constructor
 */
function Base() {
    this.name = 'base';
    this.year = 2015;
    this.sayHello = function() {
        console.log('Hello ' + this.name + ',' + 'this is ' + this.year + '.');
    };
}
/**
 * 定义基类 Base 的函数方法 showName()
 */
Base.prototype.showName = function() {
    console.log(this.name);
};
/**
 * 定义基类 Base 的函数方法 showYear()
 */
Base.prototype.showYear = function() {
    console.log(this.year);
};
/**
 * 定义原型子类 Child
 * @constructor
 */
function Child() {
    this.name = 'child';
}
/**
 * 调用util.inherits()函数方法实现原型对象继承
 */
util.inherits(Child, Base);
/**
 * 定义基类 Base 对象
 * @type {Base}
 */
var objBase = new Base();
objBase.showName();
objBase.showYear();
objBase.sayHello();
console.log(objBase);
/**
 * 定义子类 Child 对象
 * @type {Child}
 */
var objChild = new Child();
objChild.showName();
objChild.showYear();
//objChild.sayHello();
console.log(objChild);