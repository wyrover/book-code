/**
 * Created by king on 15-4-28.
 *
 * ch08.fs_readFileSync.js
 */
console.info("------   fs readFileSync()   ------");
console.info();
var file_path = "writable.txt";
var fs = require('fs'); // TODO: 引入文件系统模块
if(fs.existsSync(file_path)) {
    var file_contents = fs.readFileSync(file_path, 'utf-8');  // TODO: 读文件（同步方式）
    console.info('read writable.txt contents: ');
    console.info();
    console.info(file_contents);    // TODO: 打印输出文件内容
    console.info();
} else {
    console.log(file_path + 'is not exists.');
    console.info();
}
console.log('fs.readfileSync() Done.');
console.info();