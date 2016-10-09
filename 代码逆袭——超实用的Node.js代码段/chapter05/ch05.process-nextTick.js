/**
 * Created by king on 15-3-23.
 *
 * ch05.process-nextTick.js
 */
console.info("------   Process nextTick   ------");
console.info();
/**
 * 使用setTimeout()方法执行异步操作
 */
console.time('startB'); // TODO: console.time()计时器B
console.log('start - setTimeout');
/**
 * setTimeout()方法
 */
setTimeout(function() {
    console.log('nextTick callback 2');
}, 0);
console.log('scheduled - setTimeout');
console.timeEnd('startB');  // TODO: console.time()计时器B
/**
 * 使用process.nextTick()方法执行异步操作
 */
console.time('startA'); // TODO: console.time()计时器A
console.log('start - nextTick');
/**
 * process.nextTick()方法
 */
process.nextTick(function() {
    console.log('nextTick callback 1');
});
console.log('scheduled - nextTick');
console.timeEnd('startA');  // TODO: console.time()计时器A
console.info();
console.info("------   Process nextTick   ------");