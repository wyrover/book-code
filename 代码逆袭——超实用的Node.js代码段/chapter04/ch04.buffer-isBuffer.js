/**
 * Created by KING on 2015/3/15.
 *
 * ch04.buffer-isBuffer.js
 */
console.info("------Buffer.isBuffer()------");
console.info();
var buffer = new Buffer('nodejs', 'utf8');    // TODO: 编码形式
// TODO: 判断是否为Buffer对象
if(Buffer.isBuffer(buffer))
{
    console.info("The 'buffer' is a Buffer obj.");
}
else
{
    console.info("The 'buffer' is not a Buffer obj.");
}
var str = "nodejs"; // TODO: 定义字符串变量
// TODO: 判断是否为Buffer对象
if(Buffer.isBuffer(str))
{
    console.info("The 'str' is a Buffer obj.");
}
else
{
    console.info("The 'str' is not a Buffer obj.");
}
console.info();
console.info("------Buffer.isBuffer()------");