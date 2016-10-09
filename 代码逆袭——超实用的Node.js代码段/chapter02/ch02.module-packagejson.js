/**
 * Created by king on 15-2-12.
 */
简单模式

==========================

{

    name: "myApp",

    version :"0.0.1"

}

完整模式

===========================

{

    "name": "myApp",
    "version": "0.0.0",
    "author" : "simple",
    "description" : "Nodejs Package json介绍",
    "keywords" : "javascript, nodejs",
    "respository" : {
        "type" :"git",
        "url" :"http://path/to/url"
    },

    "bugs" : {
        "url" : "http://path/to/bug",
        "email" : "bug@example.com"
    },
    "contributors" : [

        {"name" : "zhangsan", "email" : "zhangsan@example.com"

    ]

    "license" : "MIT",
    "engines" : { "node" : "0.10.x"},
    "script" : {
        "start" : "node index.js"
    },
    "private": true,
    "scripts": {
        "start": "node ./bin/www"
    },

    "dependencies": {
        "express": "~4.9.0",
        "body-parser": "~1.8.1",
        "cookie-parser": "~1.3.3",
        "morgan": "~1.3.0",
        "serve-favicon": "~2.1.3",
        "debug": "~2.0.0",
        "jade": "~1.6.0"
    },


    "devDependencies": {
        "bower" : "~1.2.8",
        "grunt" : "~0.4.1",
        "grunt-contrib-concat" : "~0.3.0",
        "grunt-contrib-jshint" : "~0.7.2",
        "grunt-contrib-uglify" : "~0.2.7",
        "grunt-contrib-clean"  : "~0.5.0",
        "browserify" : "2.36.1",
        "grunt-browserify" : "~1.3.0"
    }
}

1.scripts

运行指定脚本命令。
2.
npm install express –save
npm install express –save-dev
上面代码表示单独安装express模块，

–save参数表示将该模块写入dependencies属性，

–save-dev表示将该模块写入devDependencies属性。
3.关于指定版本号

（1）波浪号~（tilde）+指定版本：比如~1.2.2，表示安装1.2.x的最新版本（不低于1.2.2），但是不安装1.3.x，也就是说安装时不改变大版本号和次要版本号。