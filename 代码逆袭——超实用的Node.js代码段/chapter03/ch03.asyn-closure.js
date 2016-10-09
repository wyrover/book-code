/**
 * Created by king on 15-2-11.
 *
 * ch03.asyn-closure.js
 */
function func() {   // TODO: 定义func函数实现闭包
    var n = 1;  // TODO: 初始化局部变量
    nAdd = function () {    // TODO: 全局变量nAdd实现的匿名函数实际也是闭包函数
        n++;                // TODO: 实现局部变量 n 的自加操作
    }
    // TODO: 定义closure()显式闭包函数
    function closure() {
        console.info(n);    // TODO: 打印变量
    }
    return closure;     // TODO: 返回闭包函数
}
var print_n = func();    // TODO: 将函数func()赋给变量 print_n
print_n();  // TODO: 打印func()函数中的局部变量 n(=1）
nAdd();     // TODO: 调用全局变量nAdd
print_n();  // TODO: 打印func()函数中的局部变量 n(=2）

/*在这段代码中，result实际上就是闭包f2函数。它一共运行了两次，第一次的值是999，第二次的值是1000。这证明了，函数f1中的局部变量n一直保存在内存中，并没有在f1调用后被自动清除。
为什么会这样呢？原因就在于f1是f2的父函数，而f2被赋给了一个全局变量，这导致f2始终在内存中，而f2的存在依赖于f1，因此f1也始终在内存中，不会在调用结束后，被垃圾回收机制（garbage collection）回收。
这段代码中另一个值得注意的地方，就是“nAdd=function(){n+=1}”这一行，首先在nAdd前面没有使用var关键字，因此 nAdd是一个全局变量，而不是局部变量。其次，nAdd的值是一个匿名函数（anonymous function），而这个
匿名函数本身也是一个闭包，所以nAdd相当于是一个setter，可以在函数外部对函数内部的局部变量进行操作。*/
