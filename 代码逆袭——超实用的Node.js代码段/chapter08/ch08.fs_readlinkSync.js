/**
 * Created by king on 15-4-23.
 *
 * ch08.fs_readlinkSync.js
 */
console.info("------   fs readlinkSync()   ------");
console.info();
var srcpath = "txt/symlinkSync.txt";
var linkpath = "txtlink/symlinkSync.txt";
var fs = require('fs'); // TODO: 引入文件系统模块
var linkString = fs.readlinkSync(linkpath);  // TODO: 读取链接文件（同步方式）
console.info('read linkpath: ' + linkString);
console.log('fs.readlinkSync() Done.');
var srcpath = fs.readlinkSync(srcpath);  // TODO: 读取链接文件（同步方式）
console.info('read srcpath: ' + srcpath);
console.log('fs.readlinkSync() Done.');
console.info();