/**
 * Created by king on 15-2-23.
 *
 * ch01.console-hanoi.js
 */
var iCount = 0;     //TODO: 定义移动次数
var strMoveStep = "";   // TODO: 保存移动步骤
console.info();
var readline = require('readline');
var rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});
rl.question("请输入汉诺塔问题的圆盘数目:\t", function(count) {
    // TODO: print user input
    console.log("输入汉诺塔问题的圆盘数目为:\n", count);
    hanoi(count, "A", "B", "C");
    console.info("移动步骤如下:\t%s", strMoveStep);
    console.info("移动次数为:\t%s", iCount);
    console.info();
    rl.close();
});
/**
 * 移动圆盘的方法
 * @param x
 * @param y
 */
function move(x, y) {
    iCount++;
    //console.info("%s->%s", x, y);
    strMoveStep += x + "->" + y + "\t";
}
/**
 * 完成汉诺塔问题的递归方法
 * @param m
 * @param a
 * @param b
 * @param c
 */
function hanoi(m, a, b, c) {
    if(m == 1)
    {
        move(a, c);
    }
    else
    {
        hanoi(m-1, a, c, b);
        move(a, c);
        hanoi(m-1, b, a ,c);
    }
}