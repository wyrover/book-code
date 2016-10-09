/**
 * Created by KING on 2015/2/25.
 *
 * ch03.async-apply.js
 */
apply(function, arguments..)
apply是一个非常好用的函数，可以让我们给一个函数预绑定多个参数并生成一个可直接调用的新函数，简化代码。示例如下：

function(callback) {
    test(3, callback);
};
用apply改写：
async.apply(test, 3);
log是通过为console.log函数预设了一个参数'> '而得到的新函数，其输出格式为'> ****'，请实现log函数，并调用它输出'> good';

//apply是个非常实用的函数，运用得当会减少很多基础工作。