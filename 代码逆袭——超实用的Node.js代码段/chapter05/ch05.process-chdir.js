/**
 * Created by king on 15-3-22.
 *
 * ch05.process-chdir.js
 */
console.info("------   Process chdir   ------");
console.info();
console.log('Current directory: ' + process.cwd());
console.log('Change directory to: /home/king');
process.chdir('/home/king');
console.log('Current directory: ' + process.cwd());
console.info();
console.info("------   Process chdir   ------");