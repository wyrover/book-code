/**
 * Created by KING on 2015/1/16.
 *
 * ch01.console-readline.js
 */
var readline = require('readline');
var rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});
rl.question("What is your name? ", function(answer) {
    // TODO: Log the answer in a database
    console.log("My name is ", answer);
    rl.close();
});
rl.question("How old are you? ", function(answer) {
    // TODO: Log the answer in a database
    console.log("I'm ", answer);
    rl.close();
});