/**
 * Created by king on 15-3-4.
 *
 * random-a.js
 */
var a;
console.info('随机生成100以内的整数...');
/**
 * exports random a
 * @returns {number}
 */
exports.generate_a = function() {
    a = Math.round(Math.random() * 100);    // TODO: 随机生成100以内的整数
    return a.toString();
}