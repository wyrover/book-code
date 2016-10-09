/**
 * Created by king on 15-2-12.
 *
 * ch03.asyn-try-wrong.js
 */
console.info('------   Node.js异步编程:异常捕获!   ------');
/**
 * 错误方式
 */
try{
    setTimeout(function(){
        var data = a/0;     // TODO: 错误的计算
    },1000);
}catch (e){
    console.log(e);
}
console.info();