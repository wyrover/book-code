/**
 * Created by KING on 2015/3/2.
 *
 * ch02.module-exports-obj.js
 */
var EventEmitter = require('events').EventEmitter;
module.exports = new EventEmitter();
// Do some work, and after some time emit the 'ready' event from the module itself.
setTimeout(function() {
    module.exports.emit('ready');
}, 1000);