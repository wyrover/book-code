/**
 * Created by KING on 2015/3/17.
 *
 * ch04.buffer-concat.js
 */
console.info("------   Buffer concat vs String concat  ------");
console.info();
/**
 * define variable
 * @type {Buffer}
 */
var buf = new Buffer("this is Buffer concat test!");
var str = "this is String concat test!";
/**
 * start record time
 */
console.time("buffer concat test!");
var list = [];
var len = 100000 * buf.length;
for(var i=0; i<100000; i++){
    list.push(buf);
    len += buf.length;
}
/**
 * Buffer 对象拼接
 */
Buffer.concat(list, len);
// var s1 = Buffer.concat(list, len).toString();
console.timeEnd("buffer concat test!");
console.info();
console.time("string concat test!");
var list = [];
for(var i=100000; i>=0; i--) {
    list.push(str);
}
/**
 * String 对象拼接
 * @type {string}
 */
var s2 = list.join("");
console.timeEnd("string concat test!");
/**
 * end record time
 */
console.info();
console.info("------   Buffer concat vs String concat  ------");