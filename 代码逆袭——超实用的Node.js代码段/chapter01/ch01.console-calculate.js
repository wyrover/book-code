/**
 * Created by KING on 2015/1/19.
 *
 * ch01.console-calculate.js
 */
var a, b, sign, summary;
console.info("Calculate start!");
/**
 * 模拟一个控制台输入界面
 * @type {exports}
 */
var readline = require('readline'); // TODO: 引入'readline'模块
rl = readline.createInterface(
    process.stdin,
    process.stdout
);
rl.setPrompt('Calculate> ');
rl.prompt();
a = Math.round(Math.random() * 100);    // TODO: 随机生成100以内的整数
b = Math.round(Math.random() * 100);    // TODO: 随机生成100以内的整数
sign = Math.round(Math.random() * 3);   // TODO: 随机选取四则运算符
summary = switchSign(a, b, sign);
rl.on('line', function(line) {
    if(line.trim() == summary) {       // TODO: 判断用户给出的运算结果是否正确
        console.info("answer is right!");
    } else {
        console.info("answer is error! The correct answer is " + summary + ".");
    }
    rl.prompt();
    a = Math.round(Math.random() * 100);    // TODO: 随机生成100以内的整数
    b = Math.round(Math.random() * 100);    // TODO: 随机生成100以内的整数
    sign = Math.round(Math.random() * 3);   // TODO: 随机选取四则运算符
    summary = switchSign(a, b, sign);
}).on('close', function() {
    console.log('Calculate exits!');
    process.exit(0);
});
/**
 * 四则运算方法 - switchSign()
 * @param a
 * @param b
 * @param sign - 运算符
 * @returns {*}
 */
function switchSign(a, b, sign) {
    var c;
    switch(sign) {
        case 0:
            c = a + b;
            console.info(a + "+" + b + "=");
            break;
        case 1:
            c = a - b;
            console.info(a + "-" + b + "=");
            break;
        case 2:
            c = a * b;
            console.info(a + "*" + b + "=");
            break;
        default:
            c = a + b;
            console.info(a + "+" + b + "=");
            break;
    }
    return c;
}