/**
 * Created by KING on 2015/2/2.
 *
 * ch01.console-fibonacci.js
 */
console.info("\n");
console.info("------Fibonacci数列应用------");
console.info("\n");
var strFibonacci;       // 定义字符串变量,用于输出Fibonacci数列
var i=1, j=1, s=i+j;    // 初始化变量i,j
strFibonacci = "输出Fibonacci数列>>> 1 1 ";    // 初始化Fibonacci数列
// 循环结束判断条件
while(s <= 1000)
{
    strFibonacci = strFibonacci + s + " ";
    i = j;
    j = s;
    s = i + j;  // Fibonacci 数列：F(n) = F(n-1) + F(n-2)
}
console.info(strFibonacci);     // 打印输出Fibonacci数列
console.info("\n");