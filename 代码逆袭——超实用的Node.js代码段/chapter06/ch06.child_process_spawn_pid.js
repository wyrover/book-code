/**
 * Created by king on 15-4-3.
 *
 * ch06.child_process_spawn_pid.js
 */
console.info("------   child_process spawn pid   ------");
console.info();
/**
 * child_process spawn
 * @type {exports.spawn|*}
 */
var spawn = require('child_process').spawn; // TODO: 引入child_process模块
/**
 * grep child pid of node
 */
var grep_node = spawn('grep', ['node']);
console.log('Spawned child pid of node: ' + grep_node.pid);
grep_node.stdin.end();
console.info();
/**
 * grep child pid of top
 */
var grep_top = spawn('grep', ['top']);
console.log('Spawned child pid of top: ' + grep_top.pid);
grep_top.stdin.end();
console.info();
/**
 * grep child pid of ssh
 */
var grep_ssh = spawn('grep', ['ssh']);
console.log('Spawned child pid of ssh: ' + grep_ssh.pid);
grep_ssh.stdin.end();
console.info();