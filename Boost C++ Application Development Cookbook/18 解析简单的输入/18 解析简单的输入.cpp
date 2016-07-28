#include<boost/spirit/include/phoenix_core.hpp>
#include<boost/spirit/include/phoenix_operator.hpp>
#include<assert.h>
#include<boost/spirit/include/qi.hpp>
#include <algorithm>
#include <functional>


//如果想避免代码膨胀，尝试在源文件中而不是在头文件中编写解析器。也注意传递到
//boost::spirit::parse函数的迭代器类型，使用的迭代器的不同类型越少，得到的二进制文件大小就越小。
//在源文件编写解析器还有一个好处：它不会降低项目编译速度（注意到，Spirit解析器编译缓慢，
//所以最好是在源文件编译一次，而不是在头文件中定义它们并在整个项目中使用此文件）。
struct date {
    unsigned short year;
    unsigned short month;
    unsigned short day;
};

date parse_date_time1(const std::string& s)
{
    using boost::spirit::qi::_1;
    using boost::spirit::qi::ushort_;
    using boost::spirit::qi::char_;
    using boost::phoenix::ref;
    date res;
    const char* first = s.data();
    const char* const end = first + s.size();
    bool success = boost::spirit::qi::parse(first, end,
                                            ushort_[ref(res.year) = _1] >> char_('-')
                                            >> ushort_[ref(res.month) = _1] >> char_('-') >>
                                            ushort_[ref(res.day) = _1]
                                           );

    //年份C++11 lambda表达式编写YYYY（年份）的规则，but不知为何
    //NOTE:    error C3750: “&”: 属性列表中的意外标记
    //bool success = boost::spirit::qi::parse(first, end, ushort_[ [&res](unsigned short s) {res.year = s; }] >> char_('-') >> ushort_[ref(res.month) = _1] >> char_('-') >> ushort_[ref(res.day) = _1]);
    if (!success || first != end) {
        throw std::logic_error("Parsing failed");
    }

    return res;
}

date parse_date_time2(const std::string& s)
{
    using boost::spirit::qi::_1;
    using boost::spirit::qi::uint_parser;
    using boost::spirit::qi::char_;
    using boost::phoenix::ref;
    //使用unsigned short作为输出类型，需要十进制，并用2位到2位的数字
    uint_parser<unsigned short, 10, 2, 2> u2_;
    //使用unsigned short作为输出类型，需要十进制，并用4位到4位的数字
    uint_parser<unsigned short, 10, 4, 4> u4_;
    date res;
    const char* first = s.data();
    const char* const end = first + s.size();
    bool success = boost::spirit::qi::parse(first, end, u4_[ref(res.year) = _1]
                                            >> char_('-') >> u2_[ref(res.month) = _1] >> char_('-') >> u2_[ref(res.day) = _1]
                                           );

    if (!success || first != end) {
        throw std::logic_error("Parsing failed");
    }

    return res;
}

int main()
{
    date d = parse_date_time1("2016-1-14");
    assert(d.year == 2016);
    std::cout << d.month << std::endl;
    assert(d.month == 1);
    assert(d.day == 14);
    parse_date_time1("2016-1-14");
    assert(d.year == 2016);
    assert(d.month == 1);
    assert(d.day == 14);

    try {
        parse_date_time2("12345-12-31");
        assert(false);
    } catch (const std::logic_error&) {}

    system("pause");
    return 0;
}