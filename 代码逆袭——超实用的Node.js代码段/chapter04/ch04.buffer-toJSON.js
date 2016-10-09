/**
 * Created by KING on 2015/3/15.
 *
 * ch04.buffer-toJSON.js
 */
console.info("------   Buffer toJSON   ------");
console.info();
var buf = new Buffer('test');
var json = JSON.stringify(buf);
console.log(json);
/**
 * {
 *      "type":"Buffer",
 *      "data":[116,101,115,116]
 * }
  */
var copy = JSON.parse(json, function(key, value) {
    return value && value.type === 'Buffer'
        ? new Buffer(value.data)
        : value;
});
console.log(copy);
// <Buffer 74 65 73 74>
console.info();
console.info("------   Buffer toJSON   ------");