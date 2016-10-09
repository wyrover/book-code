/**
 * Created by king on 15-2-10.
 *
 * ch01.console-factorial.js
 */
console.info("\n");
console.info("------阶乘计算------");
console.info("\n");
var iNum, iFactorial=1;     // TODO: 定义并初始化变量
var strFactorial;       // TODO: 定义字符串变量,用于阶乘运算结果输出
// TODO: 从控制台读取用户输入
var readline = require('readline');
var rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});
rl.question("请输入阶乘运算起始数值:\nn = ", function(iNum) {
    // TODO: 计算阶乘结果
    strFactorial = iNum + "! = " + factorial(iNum);
    console.info(strFactorial);     // 输出计算结果
    console.info("\n");
    rl.close();
});
/**
 * 定义阶乘运算的递归方法
 */
function factorial(n) {
    if(n > 0)
    {
        if(n == 1)
        {
            iFactorial = 1;
        }
        else
        {
            iFactorial = n * factorial(n-1);
        }
    }
    return iFactorial;
}
