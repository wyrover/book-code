/**
 * Created by KING on 2015/2/5.
 *
 * ch01.console-narcissus.js
 */
var m, a, b, c, sum;
for(m=100; m<1000; m++) {   // 循环判断100~999整数中的水仙花数
    a = parseInt(m/100);    // 计算出百位数
    b = parseInt((m-a*100)/10);     // 计算出十位数
    c = parseInt(m-a*100-b*10);     // 计算出个位数
    sum = a*a*a + b*b*b + c*c*c;
    if(m == sum) {          // 判断是否满足水仙花规则
        console.info("%d", m);
    }
}