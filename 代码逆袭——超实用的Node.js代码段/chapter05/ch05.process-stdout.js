/**
 * Created by king on 15-3-22.
 *
 * ch05.process-stdout.js
 */
console.info("------   Process stdout   ------");
console.info();
console.log('Node.js Process Module - stdout method.');
process.stdout.write('Node.js Process Module - stdout method.');
console.info('\n');
console.info('console.log()方法封装了process.stdout.write()方法');
console.log = function(d){
    process.stdout.write('process.stdout.write: ' + d + '\n');
}
console.log('Node.js Process Module - stdout method.');
console.info();
console.info("------   Process stdout   ------");