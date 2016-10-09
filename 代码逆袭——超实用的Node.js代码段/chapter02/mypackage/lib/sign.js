/**
 * Created by king on 15-3-4.
 *
 * sign.js
 */
var sign_a, sign_b;
console.info('随机生成运算符...');
exports.generate_sign_a = function() {
    sign_a = switchSign(Math.round(Math.random() * 3));   // TODO: 随机选取四则运算符
    return sign_a;
}
exports.generate_sign_b = function() {
    sign_b = switchSign(Math.round(Math.random() * 3));   // TODO: 随机选取四则运算符
    return sign_b;
}
/**
 * generate random sign
 * @param i
 * @returns {*}
 */
function switchSign(i) {
    var s;
    switch(i) {
        case 0:
            s = '+';
            break;
        case 1:
            s = '-';
            break;
        case 2:
            s = '*';
            break;
        default:
            s = '/';
            break;
    }
    return s;
}