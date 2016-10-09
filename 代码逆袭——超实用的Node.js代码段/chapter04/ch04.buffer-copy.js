/**
 * Created by KING on 2015/3/15.
 *
 * ch04.buffer-copy.js
 */
console.info("------   Buffer copy   ------");
console.info();
/**
 * define Buffer obj
 * @type {Buffer}
 */
buf1 = new Buffer(26);
buf2 = new Buffer(26);
/**
 * init a new Buffer obj
 */
for(var i=0; i<26; i++) {
    buf1[i] = i + 97;   // TODO: 97 is ASCII 'a'
    buf2[i] = 35;       // TODO: ASCII '#'
}
buf1.copy(buf2, 6, 0, 10);  // TODO: Buffer copy
console.log(buf2.toString('ascii', 0, 25));
console.log();
/**
 * define Buffer obj
 * @type {Buffer}
 */
buf = new Buffer(26);
/**
 * init a new Buffer obj
 */
for(var i=0; i<26; i++) {
    buf[i] = i + 97;    // TODO: 97 is ASCII 'a'
}
console.info(buf.toString());
console.log();
buf.copy(buf, 0, 6, 10);    // TODO: Buffer copy
console.log(buf.toString());
console.info();
console.info("------   Buffer copy   ------");