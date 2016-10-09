/**
 * Created by king on 15-2-12.
 */
从node_modules文件夹中加载#

如果require()中的模块名不是一个本地模块，也没有以'/', '../', 或是 './'开头，那么node会从当前模块的父目录开始，尝试在它的/node_modules文件夹里加载相应模块。

如果没有找到，那么就再向上移动到父目录，直到到达顶层目录位置。

例如，如果位于'/home/ry/projects/foo.js'的文件调用了require('bar.js')，那么node查找的位置依次为：

/home/ry/projects/node_modules/bar.js
/home/ry/node_modules/bar.js
/home/node_modules/bar.js
/node_modules/bar.js

这就要求程序员应尽量把依赖放在就近的位置，以防崩溃。