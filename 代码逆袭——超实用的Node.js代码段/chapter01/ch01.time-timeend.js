/**
 * Created by KING on 2015/1/7.
 *
 * ch01.time-timeend.js
 */
var n=0;
console.time('10000x10000-times');
for(var i=0; i<10000; i++) {
	for(var j=0; j<10000; j++) {
		n++;
	}
}
console.timeEnd('10000x10000-times');
console.info(n);