/**
 * module define
 * @type {*|exports}
 */
var express = require('express');
var router = express.Router();
/**
 * GET about page.
 */
router.get('/about', function(req, res, next) {
    res.render('about', {
        title: 'Express - Routes'
    });
});
/**
 * 输出模型router
 */
module.exports = router;