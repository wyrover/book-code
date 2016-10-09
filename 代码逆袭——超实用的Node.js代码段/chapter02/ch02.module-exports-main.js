/**
 * Created by king on 15-2-12.
 *
 * ch02.module-exports-main.js
 */
console.log();
var obj = require('./ch02.module-exports-obj.js');
obj.on('ready', function() {
    console.log('module ch02.module-exports-obj.js is ready');
    console.log();
});
console.log();