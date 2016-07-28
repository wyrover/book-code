////sample 1
//#include<boost/any.hpp>
//#include<vector>
//#include<string>
//#include<typeinfo>
//#include<algorithm>
//#include<iostream>
//
////这些typedef和方法将包含在我们的头文件中，它对本地的SQL接口进行包装
//typedef boost::any cell_t;
//typedef std::vector<cell_t> db_row_t;
//
////这仅仅是一个例子，没有实际操作数据库
////db_row_t get_row(const char* /*query*/)
//db_row_t get_row(const char* query)
//{
//  //在实际应用程序中'query'参数应当具有'const char*'
//  //或'const std::string&'类型吗？参见60来得到答案
//  db_row_t row;
//  row.push_back(10);
//  row.push_back(10.1f);
//  row.push_back(std::string("hello again"));
//  return row;
//}
//
////这是用户如何使用你的类的方法
//struct db_sum :public std::unary_function<boost::any, void>
//{
//private:
//  double& sum_;
//public:
//  explicit db_sum(double& sum) :sum_(sum)
//  {}
//
//  void operator()(const cell_t& value)
//  {
//      const std::type_info& ti = value.type();
//      if (ti == typeid(int))
//      {
//          sum_ += boost::any_cast<int>(value);
//      }
//      else if (ti == typeid(float))
//      {
//          sum_ += boost::any_cast<float>(value);
//      }
//  }
//};
//
//int main()
//{
//  db_row_t row = get_row("Query:　Ｇive me some row, please.");
//  double res = 0.0;
//  std::for_each(row.begin(), row.end(), db_sum(res));
//  std::cout << "Sum of arithmetic types in database row is: " << res
//      << std::endl;//数据库中算术类型行的合计是:
//  system("pause");
//  return 0;
//}


//sample2
#include<boost/variant.hpp>
#include<vector>
#include<string>
#include<iostream>

//这些typedef和方法将包含在我们的头文件中，它对本地的SQL接口进行包装
typedef boost::variant<int, float, std::string> cell_t;
typedef std::vector<cell_t> db_row_t;

//这仅仅是一个例子，没有实际操作数据库
db_row_t get_row(const char* /*query*/)
{
    //参见60，其中可以找到'query'参数的一个更好的类型
    db_row_t row;
    row.push_back(10);
    row.push_back(10.1f);
    row.push_back("hello again");
    return row;
}

//这是对值求和所需的代码如果我们的访客没有返回值，
//可以提供无模板参数给boost::static_visitor<>
struct db_sum_visitor : public boost::static_visitor<double> {
    double operator()(int value) const
    {
        return value;
    }
    double operator()(float value) const
    {
        return value;
    }
    double operator()(const std::string& /*value*/)const
    {
        return 0.0;
    }
};

int main()
{
    db_row_t row = get_row("Query: Give me some row, please.");
    //查询：请提供一些行
    double res = 0.0;
    db_row_t::const_iterator it = row.begin(), end = row.end();

    for (; it != end; ++it) {
        res += boost::apply_visitor(db_sum_visitor(), *it);
    }

    std::cout << "Sum of arithmetic types in database row is: " << res << std::endl;
    system("pause");
    return 0;
}