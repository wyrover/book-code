/**
 * Created by king on 15-3-4.
 *
 * random-c.js
 */
var c;
console.info('随机生成100以内的整数...');
/**
 * exports random c
 * @returns {number}
 */
exports.generate_c = function() {
    c = Math.round(Math.random() * 100);    // TODO: 随机生成100以内的整数
    return c.toString();
}