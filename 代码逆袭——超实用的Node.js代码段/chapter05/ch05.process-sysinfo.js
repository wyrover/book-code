/**
 * Created by king on 15-3-22.
 *
 * ch05.process-sysinfo.js
 */
console.info("------   Process System Info   ------");
console.info();
console.info('当前进程id:');
console.info(process.pid);
console.info();
console.info('当前进程名称:');
console.info(process.title);
console.info();
console.info('Node.js版本号:');
console.info(process.version);
console.info();
console.info('Node.js版本属性:');
console.info(process.versions);
console.info();
console.info('Node.js配置选项:');
console.info(process.config);
console.info();
console.info('当前系统平台:');
console.info(process.platform);
console.info();
console.info('当前CPU架构:');
console.info(process.arch);
console.info();
console.info('运行当前进程可执行文件的绝对路径:');
console.info(process.execPath);
console.info();
console.info('当前进程的命令行参数数组:');
console.info(process.argv);
console.info();
console.info('指向当前shell的环境变量:');
console.info(process.env);
console.info();
console.info("------   Process System Info   ------");