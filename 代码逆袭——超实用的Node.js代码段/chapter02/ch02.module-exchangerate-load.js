/**
 * Created by king on 15-2-12.
 *
 * ch02.module-exchangerate-load.js
 */
console.info("\n");
console.info("------汇率换算应用------");
console.info("\n");
var exRate = require('./ch02.module-exchangerate.js');  // 加载自定义模块
var readline = require('readline');     // 加载核心模块
var rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});
rl.question("Please enter count: ", function(iMoney) {
    // TODO: 调用dollar2rmb（）方法将美元换算成人民币
    console.info(iMoney + ' US-Dollar exchange to RMB equals ' + exRate.dollar2rmb(iMoney));
    // TODO: 调用rmb2dollar（）方法将人民币换算成美元
    console.info(iMoney + ' RMB exchange to US-Dollar equals ' + exRate.rmb2dollar(iMoney));
    rl.close();
});