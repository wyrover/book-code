/**
 * Created by king on 15-4-10.
 *
 * ch06.child_process_fork_sub.js
 */
console.info("------   child_process fork sub   ------");
console.info();
/**
 * process on message
 */
process.on('message', function(m) {
    console.log('CHILD got message:', m);
});
/**
 * process send message
 */
process.send({ sub: 'main' });    // TODO: send message
console.info();