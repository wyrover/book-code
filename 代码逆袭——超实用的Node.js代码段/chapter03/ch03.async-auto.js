/**
 * Created by KING on 2015/2/25.
 *
 * ch03.async-auto.js
 */
var async = require('async');
async.auto({
    getData: function(callback){
        callback(null, 'data', 'converted to array');
    },
    makeFolder: function(callback){
        callback(null, 'folder');
    },
    writeFile: ['getData', 'makeFolder', function(callback, results){
        callback(null, 'filename');
    }],
    emailLink: ['writeFile', function(callback, results){
        callback(null, {'file':results.writeFile, 'email':'user@example.com'});
    }]
}, function(err, results) {
    console.log('err = ', err);
    console.log('results = ', results);
});
//auto(tasks, [callback]);
//用来处理有依赖关系的多个任务的执行。
//示例中writeFile依赖getData和makeFolder,emailLink依赖writeFile。
//执行示例代码，将所有的results输出到控制台，观察输出顺序和内容。
//存在多个依赖关系的时候，auto让代码变得简单了不少。