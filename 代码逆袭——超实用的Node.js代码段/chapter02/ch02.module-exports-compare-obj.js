/**
 * Created by KING on 2015/3/2.
 *
 * ch02.module-exports-compare-obj.js
 */
module.exports = function(name, age) {
    this.name = name;
    this.age = age;
    this.userinfo = function() {
        console.log(this.name +' is '+ this.age +' years old');
    };
};
exports.tag = function() {
    console.log('This is a tag function.');
};