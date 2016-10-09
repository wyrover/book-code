/**
 * Created by king on 15-4-29.
 *
 * ch08.fs_writeSyncData.js
 */
console.info("------   fs writeSync()   ------");
console.info();
var file_path = "txt/writeSync.txt";
var fs = require('fs'); // TODO: 引入文件系统模块;
/**
 * 打开文件（异步方式）
 */
fs.open(file_path, 'a', function (err, fd) {
    if(err){
        throw err;
    } else {
        console.log('fs.open() done.');
        console.info();
        var file_contents_pre = fs.readFileSync(file_path, 'utf-8');  // TODO: 读文件（同步方式）
        console.info('read txt/writeSync.txt contents: ');
        console.info(file_contents_pre);    // TODO: 打印输出文件内容
        console.log('fs.readFileSync() Done.');
        console.info();
        console.info('write to txt/writeSync.txt : ');
        var data = 'fs.writeSync(fd, data[, position[, encoding]])\n';
        var len_data = fs.writeSync(fd, data, 0, 'utf-8');  // TODO: 写文件（同步方式）
        console.log('fs.writeSync() Done.');
        console.info();
        console.log('写入数据的字节数: ' + len_data);
        console.info();
        var file_contents_suf = fs.readFileSync(file_path, 'utf-8');  // TODO: 读文件（同步方式）
        console.info('read txt/writeSync.txt contents: ');
        console.info(file_contents_suf);    // TODO: 打印输出文件内容
        console.log('fs.readFileSync() Done.');
        console.info();
        /**
         * 关闭文件（异步方式）
         */
        fs.close(fd, function (err) {
            if (err) {
                throw err;
            } else {
                console.log('fs.close() done.');
                console.info();
            }
        });
    }
});