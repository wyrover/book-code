/**
 * Created by king on 15-2-12.
 *
 * ch02.module-exchangerate.js
 */
// 使用module模块的exports方法导出rmb2dollar函数
exports.rmb2dollar = function (m) {
    return m / 6.2467 + " US Dollar";  // 将人民币按照最新汇率换算成美元并返回
};
// 使用module模块的exports方法导出dollar2rmb函数
exports.dollar2rmb = function (m) {
    return m * 6.2467 + " Yuan of PRC";  // 将美元按照最新汇率换算成人民币并返回
};