/**
 * module define
 * @type {*|exports}
 */
var express = require('express');
var router = express.Router();
/**
 * GET post page.
 */
router.post('/post', function(req, res, next) {
    res.render('post', {
        title: 'Express - GET/POST',
        name: req.body.name,
        email: req.body.email
    });
});
/**
 * 输出模型router
 */
module.exports = router;
