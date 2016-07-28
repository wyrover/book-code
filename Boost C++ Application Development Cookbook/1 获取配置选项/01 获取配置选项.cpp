//BOOST_ALL_DYN_LINK;
//PATH = %PATH%; D:\boost_dir\libs64;
//此处的PATH必须要有，否则编译链接ok后，执行的时候找不到dll，要把D:\boost_dir\libs64中的dll拷贝到当前目录下，才可以执行。


#include<boost/program_options.hpp>
//'reading_file' 异常类在errors.hpp中声明
#include<boost/program_options/errors.hpp>
#include<iostream>
#include<cstdlib>
//起别名
namespace opt = boost::program_options;

int main(int argc, char* argv[])
{
    //构造一个选项描述变量，并给它一个文本的描述"All options"(所有选项)
    opt::options_description desc("All options");
    //当我们添加选项时，第1个参数是在命令行中使用的名称
    //第2个参数是那个选项的类型，它在value<>类中包装
    //第3个参数是对那个选项的一个简短的描述
    //'a'和'o'分别是"apples"和"oranges"选项的简称
    //因为'name'选项没有被标注为'required()',所以用户可能不支持它
    desc.add_options()
    ("apples,a", opt::value<int>()->default_value(10), "apples that you have")
    ("oranges,o", opt::value<int>(), "oranges that you have")
    ("name", opt::value<std::string>(), "your name")
    ("help", "produce help message");
    //用于保存命令行参数的变量
    opt::variables_map vm;
    //解析并存储参数
    opt::store(opt::parse_command_line(argc, argv, desc), vm);
    //也可以用'parse_environment'方法解析环境变量
    opt::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        system("pause");
        return 1;
    }

    //从"apples_oranges.cfg"配置文件添加丢失的选项
    //也可以提供一个可用作输入流(istreamable)的对象作为'parse_config_file'的第一个参数
    //'char'模板参数将被传递给底层的std::basic_istream对象
    try {
        opt::store(
            opt::parse_config_file<char>("apples_oranges.cfg", desc), vm);
    } catch (const opt::reading_file& e) {
        std::cout << "Failed to open file 'apples_oranges.cfg': " << e.what();//打开apples_oranges.cfg文件失败
    }

    opt::notify(vm);

    if (vm.count("name")) {
        std::cout << "Hi, " << vm["name"].as<std::string>() << "!\n";
    }

    std::cout << "Fruits count: " << vm["apples"].as<int>() + vm["oranges"].as<int>() << std::endl;//水果计数
    system("pause");
    return 0;
}