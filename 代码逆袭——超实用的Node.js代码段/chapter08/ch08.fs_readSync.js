/**
 * Created by king on 15-4-29.
 *
 * ch08.fs_readSync.js
 */
console.info("------   fs readSync()   ------");
console.info();
var readpathSync = "txt/readSync.txt";
var fs = require('fs');
var contents = fs.readFileSync(readpathSync, 'utf-8');
console.info('read file txt/readSync.txt : ');
console.info(contents);
console.info('fs.readFileSync() done.');
console.info();
fs.open(readpathSync, 'r', function (err, fd) {
    if(err) {
        console.error(err);
        console.info();
        return;
    } else {
        var buf_a = new Buffer(8);
        var readbyte_a = fs.readSync(fd, buf_a, 0, 8, null);
        console.info('读取的字节数: ' + readbyte_a);
        console.info('读取的内容: ');
        console.info(buf_a);
        console.info('fs.readSync() done.');
        console.info();
        var buf_b = new Buffer(4);
        var readbyte_b = fs.readSync(fd, buf_b, 0, 4, 4);
        console.info('读取的字节数: ' + readbyte_b);
        console.info('读取的内容: ');
        console.info(buf_b);
        console.info('fs.readSync() done.');
        console.info();
        var buf_c = new Buffer(6);
        var readbyte_c = fs.readSync(fd, buf_c, 2, 4, 2);
        console.info('读取的字节数: ' + readbyte_c);
        console.info('读取的内容: ');
        console.info(buf_c);
        console.info('fs.readSync() done.');
        console.info();
    }
});