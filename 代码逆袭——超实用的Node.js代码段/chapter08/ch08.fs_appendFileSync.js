/**
 * Created by king on 15-4-30.
 *
 * ch08.fs_appendFileSync.js
 */
console.info("------   fs appendFileSync()   ------");
console.info();
var file_path = "txt/appendFileSync.txt";
var fs = require('fs'); // TODO: 引入文件系统模块
if(fs.existsSync(file_path)) {
    var file_contents_pre = fs.readFileSync(file_path, 'utf-8');  // TODO: 读文件（同步方式）
    console.info('read txt/appendFileSync.txt contents: ');
    console.info(file_contents_pre);    // TODO: 打印输出文件内容
    console.log('fs.readFileSync() Done.');
    console.info();
    console.info('write to txt/appendFileSync.txt : ');
    fs.writeFileSync(file_path, 'fs.appendFileSync(filename, data[, options])\n');  // TODO: 写文件（同步方式）
    console.log('fs.writeFileSync() Done.');
    console.info();
    var file_contents_suf = fs.readFileSync(file_path, 'utf-8');  // TODO: 读文件（同步方式）
    console.info('read txt/appendFileSync.txt contents: ');
    console.info(file_contents_suf);    // TODO: 打印输出文件内容
    console.log('fs.readFileSync() Done.');
    console.info();
    fs.appendFileSync(file_path, 'add filename to appendFileSync.txt\n');  // TODO: 追加写入文件（同步方式）
    console.log('fs.appendFileSync() Done.');
    console.info();
    var file_contents_add = fs.readFileSync(file_path, 'utf-8');  // TODO: 读文件（同步方式）
    console.info('read txt/appendFileSync.txt contents: ');
    console.info(file_contents_add);    // TODO: 打印输出文件内容
    console.log('fs.readFileSync() Done.');
    console.info();
} else {
    console.log(file_path + 'is not exists.');
    console.info();
}