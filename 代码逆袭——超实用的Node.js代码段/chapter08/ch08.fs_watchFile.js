/**
 * Created by king on 15-4-30.
 *
 * ch08.fs_watchFile.js
 */
console.info("------   fs watchFile()   ------");
console.info();
var file_path = "txt/watchFile.txt";
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 监控文件函数方法
 *
 * fs.watchFile(filename, [options], listener);
 */
fs.watchFile(file_path, function (curr, prev) {
    console.log('the current mtime is: ' + curr.mtime);
    console.log('the previous mtime was: ' + prev.mtime);
    /**
     * 监控文件函数方法
     *
     * fs.watch(filename, [options], [listener]);
     */
    fs.watch(file_path, function (event, filename) {
        console.log('the event is: ' + event);
        console.log('the filename is: ' + filename);
    });
});