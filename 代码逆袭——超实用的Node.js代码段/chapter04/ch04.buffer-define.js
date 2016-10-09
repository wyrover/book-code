/**
 * Created by KING on 2015/3/14.
 *
 * ch04.buffer-define.js
 */
// TODO: 定义并初始化Buffer对象 --- buffer16
var buffer16 = new Buffer([0x6e, 0x6f, 0x64, 0x65, 0x6a, 0x73]);    // TODO: 数组形式
console.log("The variable buffer16's length is " + buffer16.length + ".");
console.log("The variable buffer16 is " + buffer16 + ".");
// TODO: 使用数组下标形式读取Buffer对象
for(var i=0; i<buffer16.length; i++)
{
    console.log("buffer16[" + i + "] is " + buffer16[i]);
    console.log("buffer16[" + i + "].toString is " + buffer16[i].toString());
}
console.log(buffer16.toString('utf8')); // TODO: utf8编码
console.log(buffer16.toString('hex'));  // TODO: hex编码
// TODO: 定义并初始化Buffer对象 --- bufferUTF8
var bufferUTF8 = new Buffer('nodejs', 'utf8');  // TODO: 编码形式
console.log("The variable bufferUTF8's length is " + bufferUTF8.length + ".");
console.log("The variable bufferUTF8 is " + bufferUTF8 + ".");
// TODO: 使用数组下标形式读取Buffer对象
for(var i=0; i<bufferUTF8.length; i++)
{
    console.log("bufferUTF8[" + i + "] is " + bufferUTF8[i]);
    console.log("bufferUTF8[" + i + "].toString is " + bufferUTF8[i].toString());
}
console.log(bufferUTF8.toString('utf8'));   // TODO: utf8编码
console.log(bufferUTF8.toString('hex'));    // TODO: hex编码