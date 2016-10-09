/**
 * module define
 * @type {*|exports}
 */
var express = require('express');
var router = express.Router();
/**
 * GET name page.
 * with req.query.name parameters
 */
router.get('/name', function(req, res) {
    res.render('name', {
        title: 'Express - Routes',
        name: req.query.name
    });
});
/**
 * 输出模型router
 */
module.exports = router;