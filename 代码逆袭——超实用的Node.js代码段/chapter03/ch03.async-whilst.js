/**
 * Created by KING on 2015/2/25.
 *
 * ch03.async-whilst.js
 */
console.info('------   Node.js异步编程: Async whilst   ------');
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
async.whilst(
    function () {
        return count < 5;
    },
    function (callback) {
        console.log(count);
        list[count].age += 1;
        count++;
        setTimeout(callback, 1000);
    },
    function (err) {
        console.log(count);
        console.log(list);
    }
);
console.info();
console.info('------   Node.js异步编程: Async whilst   ------');