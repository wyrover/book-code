/**
 * module define
 * @type {*|exports}
 */
var express = require('express');
var router = express.Router();
/**
 * GET root page.
 */
router.get('/', function(req, res, next) {
  res.render('index', {
    title: 'Express - GET/POST'
  });
});
/**
 * 输出模型router
 */
module.exports = router;
