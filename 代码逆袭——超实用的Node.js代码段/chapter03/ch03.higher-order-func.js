/**
 * Created by king on 15-2-12.
 *
 * ch03.higher-order-func.js
 */
var arrSort = [5,34,87,32,56,2,17];
arrSort.sort(function(a,b){
    return b-a;
});
console.info(arrSort);