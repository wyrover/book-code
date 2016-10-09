/**
 * Created by king on 15-2-12.
 *
 * ch02.module-counter-main.js
 */
console.log();
var counter = require('./ch02.module-counter-obj.js');
console.log('第一次调用模块[ch02.module-counter-obj.js]');
counter.seOutputVal(10);               //设置从10开始计数
counter.setIncrement (10);             //设置增量为10
counter.printNextCount();
counter.printNextCount();
counter.printNextCount();
counter.printNextCount();
/**
 * require()多次调用同一模块不会重复加载
 */
var counter = require('./ch02.module-counter-obj.js');
console.log('第二次调用模块[ch02.module-counter-obj.js]');
counter.printNextCount();
counter.printNextCount();
counter.printNextCount();
counter.printNextCount();
counter.printNextCount();
/**
 * 自动清零操作
 */
console.log('自动清零操作');
counter.autoSetZero();
console.log();