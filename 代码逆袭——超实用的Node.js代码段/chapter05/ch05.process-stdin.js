/**
 * Created by king on 15-3-22.
 *
 * ch05.process-stdin.js
 */
console.info("------   Process stdin   ------");
console.info();
console.info('用户输入数据');
/**
 * readable - 接受控制台用户输入事件
 */
process.stdin.setEncoding('utf8');
process.stdin.on('readable', function() {
    var chunk = process.stdin.read();   // TODO: process.stdin.read() 方法
    if (chunk !== null) {
        process.stdout.write('Print Data: ' + chunk + '\n');    // TODO: 打印输出
    }
});
/**
 * end - 结束控制台输入事件
 */
process.stdin.on('end', function() {
    process.stdout.write('end.\n');
});
console.info();
console.info("------   Process stdin   ------");