/**
 * Created by KING on 2015/3/15.
 *
 * ch04.buffer-write.js
 */
console.info("------   Buffer Write   ------");
console.info();
// TODO: buf.write() usage
buf = new Buffer(32);
len = buf.write('\u00bd + \u00bc = \u00be', 0);
console.log("buf.write() usage: " + buf.toString('utf8', 0, len) + " , " + len + " bytes");
console.info();
// TODO: buf.writeUInt8() usage
var buf8 = new Buffer(4);
buf8.writeUInt8(0x5, 0);
buf8.writeUInt8(0x6, 1);
buf8.writeUInt8(0x12, 2);
buf8.writeUInt8(0x34, 3);
console.log("buf.writeUInt8() usage: ");
console.log(buf8);
console.info();
// TODO: buf.writeUInt16BE() usage
var buf16BE = new Buffer(4);
buf16BE.writeUInt16BE(0x1234, 0);
buf16BE.writeUInt16BE(0xabcd, 2);
console.log("buf.writeUInt16BE() usage: ");
console.log(buf16BE);
console.info();
// TODO: buf.writeUInt16LE() usage
var buf16LE = new Buffer(4);
buf16LE.writeUInt16LE(0x1234, 0);
buf16LE.writeUInt16LE(0xabcd, 2);
console.log("buf.writeUInt16LE() usage: ");
console.log(buf16LE);
console.info();
// TODO: buf.writeUInt32BE() usage
var buf32BE = new Buffer(4);
buf32BE.writeUInt32BE(0xfeedface, 0);
console.log("buf.writeUInt32BE() usage: ");
console.log(buf32BE);
console.info();
// TODO: buf.writeUInt32LE() usage
var buf32LE = new Buffer(4);
buf32LE.writeUInt32LE(0xfeedface, 0);
console.log("buf.writeUInt32LE() usage: ");
console.log(buf32LE);
console.info();
console.info("------   Buffer Write   ------");