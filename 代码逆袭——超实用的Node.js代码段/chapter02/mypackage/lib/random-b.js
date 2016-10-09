/**
 * Created by king on 15-3-4.
 *
 * random-b.js
 */
var b;
console.info('随机生成100以内的整数...');
/**
 * exports random b
 * @returns {number}
 */
exports.generate_b = function() {
    b = Math.round(Math.random() * 100);    // TODO: 随机生成100以内的整数
    return b.toString();
}