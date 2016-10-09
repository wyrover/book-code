/**
 * module define
 * @type {*|exports}
 */
var express = require('express');
var router = express.Router();
/**
 * GET pwd page.
 */
router.get('/pwd/:passwd', function(req, res, next) {
    res.render('pwd', {
        title: 'Express - Routes',
        passwd: req.params.passwd
    });
});
/**
 * 输出模型router
 */
module.exports = router;