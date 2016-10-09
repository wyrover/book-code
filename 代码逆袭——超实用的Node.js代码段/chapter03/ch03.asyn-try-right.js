/**
 * Created by KING on 2015/3/18.
 *
 * ch03.asyn-try-right.js
 */
console.info('------   Node.js异步编程:异常捕获!   ------');
/**
 * 正确方式
 */
setTimeout(function(){
    try{
        var data = b/0;     // TODO: 错误的计算
    }catch(e){
        console.log(e);
    }
},1000);
console.info();