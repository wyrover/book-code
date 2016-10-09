/**
 * Created by king on 15-2-12.
 */
如果按文件名没有查找到，那么node会添加 .js和 .json后缀名，再尝试加载，如果还是没有找到，最后会加上.node的后缀名再次尝试加载。

.js 会被解析为Javascript纯文本文件，.json 会被解析为JSON格式的纯文本文件. .node 则会被解析为编译后的插件模块，由dlopen进行加载。

模块以'/'为前缀，则表示绝对路径。例如，require('/home/marco/foo.js') ，加载的是/home/marco/foo.js这个文件。

模块以'./'为前缀，则路径是相对于调用require()的文件。 也就是说，circle.js必须和foo.js在同一目录下，require('./circle')才能找到。

当没有以'/'或者'./'来指向一个文件时，这个模块要么是"核心模块"，要么就是从node_modules文件夹加载的。

如果指定的路径不存在，require()会抛出一个code属性为'MODULE_NOT_FOUND'的错误。