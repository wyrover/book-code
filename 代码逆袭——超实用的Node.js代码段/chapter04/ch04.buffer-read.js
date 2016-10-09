/**
 * Created by KING on 2015/3/15.
 *
 * ch04.buffer-read.js
 */
console.info("------ Buffer Read ------");
console.info();
var str_readUInt8 = "";
var str_readInt8 = "";
var str_readUInt16LE = "";
var str_readInt16LE = "";
var str_readUInt16BE = "";
var str_readInt16BE = "";
var buf = new Buffer(4);
buf[0] = 0x6;
buf[1] = 0x8;
buf[2] = 0x23;
buf[3] = 0x57;
str_readUInt8 += "buf.readUInt8(i) is : ";
for (i=0; i<buf.length; i++) {
    str_readUInt8 += buf.readUInt8(i).toString(16) + "   ";
}
console.log(str_readUInt8);
console.info();
str_readInt8 += "buf.readInt8(i) is : ";
for (i=0; i<buf.length; i++) {
    str_readInt8 += buf.readInt8(i).toString(16) + "   ";
}
console.log(str_readInt8);
console.info();
str_readUInt16LE += "buf.readUInt16LE(i) is : ";
for (i=0; i<buf.length-1; i++) {
    str_readUInt16LE += buf.readUInt16LE(i).toString(16) + "   ";
}
console.log(str_readUInt16LE);
console.info();
str_readInt16LE += "buf.readInt16LE(i) is : ";
for (i=0; i<buf.length-1; i++) {
    str_readInt16LE += buf.readInt16LE(i).toString(16) + "   ";
}
console.log(str_readInt16LE);
console.info();
str_readUInt16BE += "buf.readUInt16BE(i) is : ";
for (i=0; i<buf.length-1; i++) {
    str_readUInt16BE += buf.readUInt16BE(i).toString(16) + "   ";
}
console.log(str_readUInt16BE);
console.info();
str_readInt16BE += "buf.readInt16BE(i) is : ";
for (i=0; i<buf.length-1; i++) {
    str_readInt16BE += buf.readInt16BE(i).toString(16) + "   ";
}
console.log(str_readInt16BE);
console.info();
console.info("buf.readUInt32LE(i) is : " + buf.readUInt32LE(0).toString(16));
console.info();
console.info("buf.readUInt32BE(i) is : " + buf.readUInt32BE(0).toString(16));
console.info();
console.info("buf.readInt32LE(i) is : " + buf.readInt32LE(0).toString(16));
console.info();
console.info("buf.readInt32BE(i) is : " + buf.readInt32BE(0).toString(16));
console.info();
console.info("------ Buffer Read ------");