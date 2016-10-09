/**
 * Created by king on 15-2-10.
 *
 * ch01.console-primeNum.js
 */
var i, j, iNum, strPrimeNum;
var bFlag;  // TODO: 定义该变量用于标记是否为质数
var readline = require('readline');
var rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});
/**
 * TODO: 用户输入一个整数作为上限,通过程序计算从2开始到该整数之间的全部质数
 */
rl.question("Please enter number? ", function(iNum) {
    // TODO: Calculate prime number
    strPrimeNum = " 2 ";
    for(i=3; i<=iNum; i++)  // TODO: 利用for循环计算质数
    {
        bFlag = true;
        for(j=2; j<i; j++)
        {
            if(i%j == 0)    // TODO: 通过if条件判断是否满足质数的条件
            {
                bFlag = false;
                break;
            }
        }
        if(bFlag == true)
        {
            strPrimeNum = strPrimeNum + i + " ";    // TODO: 将质数保存在一个字符串变量中
        }
    }
    console.log("Prime number > ", strPrimeNum);    // TODO: 打印输出质数序列
    rl.close();
});