/**
 * Created by king on 15-3-4.
 *
 * calculate.js
 */
var a, b, c, sign_a, sign_b, summary;
var ra = require('./random-a');
var rb = require('./random-b');
var rc = require('./random-c');
var sign = require('./sign');
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
a = ra.generate_a();    // TODO: 随机生成100以内的整数
b = rb.generate_b();    // TODO: 随机生成100以内的整数
c = rc.generate_c();    // TODO: 随机生成100以内的整数
sign_a = sign.generate_sign_a();   // TODO: 随机选取四则运算符
sign_b = sign.generate_sign_b();   // TODO: 随机选取四则运算符
summary = cal(a, b, c, sign_a, sign_b);
rl.on('line', function(line) {
    if(line.trim() == summary) {       // TODO: 判断用户给出的运算结果是否正确
        console.info("answer is right!");
    } else {
        console.info("answer is error! The correct answer is " + summary + ".");
    }
    rl.prompt();
    a = ra.generate_a();    // TODO: 随机生成100以内的整数
    b = rb.generate_b();    // TODO: 随机生成100以内的整数
    c = rc.generate_c();    // TODO: 随机生成100以内的整数
    sign_a = sign.generate_sign_a();   // TODO: 随机选取四则运算符
    sign_b = sign.generate_sign_b();   // TODO: 随机选取四则运算符
    summary = cal(a, b, c, sign_a, sign_b);
}).on('close', function() {
    console.log('Calculate exits!');
    process.exit(0);
});
/**
 * 四则运算方法 - cal()
 * @param a
 * @param b
 * @param sign - 运算符
 * @returns {*}
 */
function cal(stra, strb, strc, signa, signb) {
    var s = "";
    var result = 0;
    s = stra + signa + strb + signb + strc;
    console.log(s);
    result = eval(s);   // TODO: 调用eval()方法计算字符串表达式
    return result;
}