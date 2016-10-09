/**
 * Created by KING on 2015/3/16.
 *
 * ch04.buffer-slice.js
 */
console.info("------   Buffer slice   ------");
console.info();
/**
 * Create a new Buffer obj
 */
var buf = new Buffer(26);
for(var i = 0; i < 26; i++) {
    buf[i] = i + 97;    // TODO: 97 is ASCII 'a'
}
var buf_slice_a = buf.slice(0, 5);    // TODO: slice buf
console.log(buf_slice_a.toString('ascii', 0, buf_slice_a.length));
for(var j = 0; j < 26; j++) {
    buf[j] = 122 - j;    // TODO: 122 is ASCII 'z'
}
var buf_slice_b = buf.slice(0, 5);    // TODO: slice buf
console.log(buf_slice_b.toString('ascii', 0, buf_slice_b.length));
console.info();
console.info("------   Buffer slice   ------");