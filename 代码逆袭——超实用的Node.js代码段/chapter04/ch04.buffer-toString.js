/**
 * Created by KING on 2015/3/15.
 *
 * ch04.buffer-toString.js
 */
console.info("------   Buffer toString   ------");
console.info();
// TODO: define a buffer obj
buf = new Buffer(26);
// TODO: init a buffer obj
for (var i = 0 ; i < 26 ; i++) {
    buf[i] = i + 97;    // TODO: 97 is ASCII 'a'
}
// TODO: outputs: abcdefghijklmnopqrstuvwxyz
console.info("buf.toString('ascii'): " + buf.toString('ascii'));
console.info();
// TODO: outputs: abcde
console.info("buf.toString('ascii', 0, 5): " + buf.toString('ascii', 0, 5));
console.info();
// TODO: outputs: abcde
console.info("buf.toString('utf8', 0, 8): " + buf.toString('utf8', 0, 8));
console.info();
// TODO: outputs: abcde
console.info("buf.toString('hex', 0, 8): " + buf.toString('hex', 0, 12));
console.info();
// TODO: encoding defaults to 'utf8', outputs abcde
console.info("buf.toString(undefined, 0, 18): " + buf.toString(undefined, 0, 18));
console.info();
console.info("------   Buffer toString   ------");