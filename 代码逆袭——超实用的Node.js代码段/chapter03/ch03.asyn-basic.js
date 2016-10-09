/**
 * Created by king on 15-2-11.
 *
 * ch03.asyn-basic.js
 */
console.info('------   Node.js异步编程:基础初步!   ------');
console.info("\n");
// 使用setTimeout()异步方法初探异步机制
setTimeout(function(){
    console.log('asyn - print it now!');  // 在回调函数内输出信息
},3000);
console.log('asyn - print it 3 second later!');    // 异步方法后输出信息
console.info("\n");
console.info('------   Node.js异步编程   ------');
console.info("\n");