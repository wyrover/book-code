/**
 * Created by KING on 2015/2/25.
 *
 * ch03.async-iterator.js
 */
var async = require('async');
var iter = async.iterator([
    function() { console.log('111') },
    function() { console.log('222') },
    function() { console.log('333') }
]);
iter();

//iterator(tasks)
将一组函数包装成为一个iterator，可通过next()得到以下一个函数为起点的新的iterator。该函数通常由async在内部使用，但如果需要时，也可在我们的代码中使用它。

var iter = async.iterator([
    function() { console.log('111') },
    function() { console.log('222') },
    function() { console.log('333') }
]);
iter();
/*
直接调用()，会执行当前函数，并返回一个由下个函数为起点的新的iterator。调用next()，不会执行当前函数，直接返回由下个函数为起点的新iterator。

对于同一个iterator，多次调用next()，不会影响自己。如果只剩下一个元素，调用next()会返回null。

通过next函数获得新的iter，并调用它，看看输出结果。

想一想哪些场景可以用到iterator呢。

课程小结
async模块在流程控制方面给我们带来了比较全面的解决办法，下面我们来回顾一下都有哪几种方案：

串行控制：
series、waterfall、compose;

并行控制：
parallel、parallelLimit、queue;

循环控制：
whilst、doWhilst、until、doUntil、forever;

其他控制：
apply‘applyEach、iterator、auto;

学习了async模块后，对一般的函数嵌套问题都能够引刃而解，如果还有比较特别的应用需求，可以跟大家一起讨论一下，看看小伙伴有没有更好的解决办法，欢迎继续学习其他课程。
*/