/*!
* \file Ctext.h
* \brief 概述 
* 
* asdfasdfasdfasdf
* 
* \author wyrover 
* \version 版本号(maj.min，主版本.分版本格式) 
* \date 2013-11-4 
*/

#pragma once
/// \brief 命名空间的简单概述 
/// 
/// 命名空间的详细概述ddddddddddddddddddddd
namespace text
{

}

/// \brief Ctext的doxygen测试
///
/// 作doxygen测试用
class Ctext
{
public:
    Ctext(void);
    ~Ctext(void);
    /// \brief 测试函数说明
    /// \param n1 参数1说明
    /// \param c2 参数2说明
    ///    \return 返回说明
    bool text(int n1,Ctext c2);
    /// \brief 函数简要说明-测试函数
    /// 
    /// 函数详细说明，这里写函数的详细说明信息，说明可以换行
    /// ，如这里所示，同时需要注意的是详细说明和简要说明之间必须空一行
    /// ，详细说明之前不需要任何标识符
    /// \param n1 参数1说明
    /// \param c2 参数2说明
    ///    \return 返回说明
    bool text2(int n1,Ctext c2);

    /// 函数说明-测试函数
    /// 
    /// 函数详细说明，这里写函数的详细说明信息，说明可以换行
    /// ，如这里所示，同时需要注意的是详细说明和简要说明之间必须空一行
    /// ，详细说明之前不需要任何标识符
    /// \param n1 参数1说明
    /// \param c2 参数2说明
    ///    \return 返回说明
    bool text3(int n1,Ctext c2);
    /// \brief 函数说明-测试函数4
    /// \param n1 参数1说明
    /// \param c2 参数2说明
    ///    \return 返回说明
    /// \see text3 text2 text
    bool text4(int n1,Ctext c2);

    int m_a;     ///< 成员变量1m_a说明
    double m_b; ///< 成员变量2m_b说明

    /// \brief 成员变量m_c简要说明
    ///
    /// 成员变量m_c的详细说明，这里可以对变量进行
    ///详细的说明和描述，具体方法和函数的标注是一样的
    float m_c;

    /// \brief xxx枚举变量的简要说明
    ///
    /// xxx枚举变量的详细说明--枚举变量的详细说明和函数的详细说明
    ///的写法是一致的。每个枚举变量下可以进行单独说明
    enum{
        em_1,///< 枚举值1的说明
        em_2,///< 枚举值2的说明
        em_3 ///< 枚举值3的说明
    };
};