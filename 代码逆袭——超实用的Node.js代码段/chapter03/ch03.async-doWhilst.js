/**
 * Created by KING on 2015/2/25.
 *
 * ch03.async-doWhilst.js
 */
console.info('------   Node.js异步编程: Async doWhilst   ------');
console.info();
var async = require('async');
var count = 0;
/**
 * Define JSON Array
 * @type {{name: string, age: number}[]}
 */
var list = [
    {name:'Jack',age:20},
    {name:'Lucy',age:18},
    {name:'Jack',age:20},
    {name:'Lucy',age:18},
    {name:'Lucy',age:18}
];
async.doWhilst(
    function (callback) {
        console.log(count);
        list[count].age += 1;
        count++;
        setTimeout(callback, 1000);
    },
    function () { return count < 5; },
    function (err) {
        console.log(list);
    }
);
console.info();
console.info('------   Node.js异步编程: Async doWhilst   ------');