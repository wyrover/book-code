/**
 * Created by king on 15-2-12.
 *
 * ch03.partial-func.js
 */
var isType = function(type){
    return function(obj){
        return toString.call(obj)=='[object '+type+']';
    }
};
var isString = isType('String');
var isFunction = isType('Function');
var isArray= isType('Array');



var say =function(name){
    return function(text){
        console.log(name+' say '+text);
    }
};
var tomSay = say('tom');

tomSay ('hello');