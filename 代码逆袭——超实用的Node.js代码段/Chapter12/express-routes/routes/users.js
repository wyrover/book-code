/**
 * module define
 * @type {*|exports}
 */
var express = require('express');
var router = express.Router();
/**
 * GET users listing
 */
router.get('/', function(req, res, next) {
  res.send('respond with a resource');
});
/**
 * 输出模型router
 */
module.exports = router;
