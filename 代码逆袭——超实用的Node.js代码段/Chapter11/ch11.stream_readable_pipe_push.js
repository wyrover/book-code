/**
 * Created by king on 15-6-4.
 *
 * ch11.stream_readable_pipe_push.js
 */
console.info("------   stream readable pipe push   ------");
console.info();
var stream = require('stream');  // TODO: 引入流(Stream)模块
var rs = new stream.Readable;   // TODO: 定义Readable对象
rs.push('Stream '); // TODO: 使用push()方法发送数据
rs.push('Readable ');   // TODO: 使用push()方法发送数据
rs.push('Push() '); // TODO: 使用push()方法发送数据
rs.push('Pipe() '); // TODO: 使用push()方法发送数据
rs.push('\n');  // TODO: 使用push()方法发送数据
rs.push(null);  // TODO: 使用push()方法通知发送数据完毕
rs.pipe(process.stdout);    // TODO: 使用pipe()方法导出数据