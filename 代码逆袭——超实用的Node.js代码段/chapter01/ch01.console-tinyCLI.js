/**
 * Created by KING on 2015/1/16.
 *
 * ch01.console-tinyCLI.js
 */
var readline = require('readline');
rl = readline.createInterface(
    process.stdin,
    process.stdout
);
rl.setPrompt('NodeJS> ');
rl.prompt();
rl.on('line', function(line) {
    switch(line.trim()) {
        case 'name':
            console.log('king!');
            break;
        case 'code':
            console.log('Node.js!');
            break;
        case 'time':
            console.log('2015!');
            break;
        default:
            console.log('Say what? I might have heard `' + line.trim() + '`');
            break;
    }
    rl.prompt();
}).on('close', function() {
    console.log('Have a great day!');
    process.exit(0);
});
