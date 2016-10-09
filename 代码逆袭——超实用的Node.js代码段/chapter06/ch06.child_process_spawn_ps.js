/**
 * Created by king on 15-4-9.
 *
 * ch06.child_process_spawn_ps.js
 */
var spawn = require('child_process').spawn,
    ps    = spawn('ps', ['ax']),
    grep  = spawn('grep', ['ssh']);

ps.stdout.on('data', function (data) {
    grep.stdin.write(data);
});

ps.stderr.on('data', function (data) {
    console.log('ps stderr: ' + data);
});

ps.on('close', function (code) {
    if (code !== 0) {
        console.log('ps process exited with code ' + code);
    }
    grep.stdin.end();
});

grep.stdout.on('data', function (data) {
    console.log('' + data);
});

grep.stderr.on('data', function (data) {
    console.log('grep stderr: ' + data);
});

grep.on('close', function (code) {
    if (code !== 0) {
        console.log('grep process exited with code ' + code);
    }
});