/**
 * 加载依赖库,原来这个类库都封装在connect中,现在需要单独加载
 * @type {*|exports}
 */
var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
/**
 * 加载路由控制
 * @type {router|exports}
 */
var routes = require('./routes/index');
var users = require('./routes/users');
var about = require('./routes/about');
var name = require('./routes/name');
var pwd = require('./routes/pwd');
/**
 * 创建项目实例
 */
var app = express();
/**
 * 定义EJS模板引擎和模板文件位置,也可以使用jade或其他模型引擎
 * view engine setup
 */
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');
/**
 * 定义icon图标
 * uncomment after placing your favicon in /public
 */
//app.use(favicon(__dirname + '/public/favicon.ico'));
/**
 * 定义日志和输出级别
 */
app.use(logger('dev'));
/**
 * 定义数据解析器
 */
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
/**
 * 定义cookie解析器
 */
app.use(cookieParser());
/**
 * 定义静态文件目录
 */
app.use(express.static(path.join(__dirname, 'public')));
/**
 * 匹配路径和路由
 */
app.use('/', routes); // TODO: 访问根路径
app.use('/users', users); // TODO: 访问路径/users
app.get('/test', function(req, res) { // TODO: 访问路径/test
  res.send('Test Routes!');
});
app.get('/about', about); // TODO: 访问路径/about
app.get('/name', name); // TODO: 访问路径/name
app.get('/wildcard/*', function(req, res) { // TODO: 访问通配符路径/wildcard/*
  res.send(req.originalUrl); // TODO: req.originalUrl 获取当前URL
});
app.get('/pwd/:passwd', pwd); // TODO: 访问路径/pwd/:passwd
app.get('/next/*', function(req, res, next) {  // TODO: 访问路径/next/*
  req.control = "转移控制权";
  next(); //TODO: 把权限转移到下一个路由
});
app.get('/next/control', function(req, res) {  // TODO: 访问路径/next/control
  res.send('next() : ' + req.control);
});
/**
 * error handlers
 * catch 404 and forward to error handler
 * development error handler
 */
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});
/**
 * 开发环境，500错误处理和错误堆栈跟踪
 * will print stacktrace
 */
if (app.get('env') === 'development') {
  app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
      message: err.message,
      error: err
    });
  });
}
/**
 * production error handler
 * no stacktraces leaked to user
 * 生产环境，500错误处理
 */
app.use(function(err, req, res, next) {
  res.status(err.status || 500);
  res.render('error', {
    message: err.message,
    error: {}
  });
});
/**
 * 输出模型app
 */
module.exports = app;
