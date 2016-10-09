/**
 * Created by KING on 2015/3/15.
 *
 * ch04.buffer-byteLength.js
 */
console.info("------Buffer.byteLength()------");
console.info();
str = '\u00bd + \u00bc = \u00be';   // TODO: 定义字符串
// TODO: ½ + ¼ = ¾: 9 characters, 12 bytes
console.log(str + ": " + str.length + " characters, " + Buffer.byteLength(str, 'utf8') + " bytes");
console.info();
console.info("------Buffer.byteLength()------");