/**
 * Created by king on 15-2-12.
 *
 * ch02.module-file-foo.js
 */
var circle = require('./ch02.module-file-circle.js');
console.info();
var readline = require('readline');
var rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});
rl.question("Please input radius:\t", function(answer) {
    // TODO: print user input
    console.log("The radius is:\t", answer);
    console.log( 'The area of a circle of radius ' + answer + ' is ' + circle.area(answer));
    console.log( 'The circumference of a circle of radius ' + answer + ' is ' + circle.circumference(answer));
    console.info();
    rl.close();
});