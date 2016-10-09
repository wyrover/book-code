/**
 * Created by king on 15-4-30.
 *
 * ch08.fs_appendFile.js
 */
console.info("------   fs appendFile()   ------");
console.info();
var file_path = "txt/appendFile.txt";
var fs = require('fs'); // TODO: 引入文件系统模块
/**
 * 判断文件是否存在（异步方式）
 */
fs.exists(file_path, function (exists) {
    if (exists) {
        var file_contents_pre = fs.readFileSync(file_path, 'utf-8');  // TODO: 读文件（同步方式）
        console.info('read txt/appendFile.txt contents: ');
        console.info(file_contents_pre);    // TODO: 打印输出文件内容
        console.log('fs.readFileSync() Done.');
        console.info();
        console.info('write to txt/appendFile.txt : ');
        fs.writeFileSync(file_path, 'fs.appendFile(filename, data[, options], callback)\n');  // TODO: 写文件（同步方式）
        console.log('fs.writeFileSync() Done.');
        console.info();
        var file_contents_suf = fs.readFileSync(file_path, 'utf-8');  // TODO: 读文件（同步方式）
        console.info('read txt/appendFile.txt contents: ');
        console.info(file_contents_suf);    // TODO: 打印输出文件内容
        console.log('fs.readFileSync() Done.');
        console.info();
        /**
         * 追加写入文件（异步方式）
         */
        fs.appendFile(file_path, 'add filename to appendFile.txt\n', function (err) {
            if (err) {
                throw err;
            } else {
                console.log('fs.appendFile() Done.');
                console.info();
                var file_contents_add = fs.readFileSync(file_path, 'utf-8');  // TODO: 读文件（同步方式）
                console.info('read txt/appendFile.txt contents: ');
                console.info(file_contents_add);    // TODO: 打印输出文件内容
                console.log('fs.readFileSync() Done.');
                console.info();
            }
        });
    } else {
        console.info('err: ' + err);
        console.info();
    }
});
console.info();