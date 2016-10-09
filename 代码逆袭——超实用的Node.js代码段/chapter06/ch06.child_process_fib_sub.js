/**
 * Created by king on 15-4-10.
 *
 * ch06.child_process_fib_sub.js
 */
console.info("------   child_process fork sub   ------");
console.info();
/**
 * function fib
 */
function fib(n) {
    if (n < 2) {
        return 1;
    } else {
        return fib(n - 2) + fib(n - 1);
    }
}
/**
 * process send message
 */
process.on('message', function(m) {
    process.send({result: fib(m.input)});   // TODO: send message
});