// 这是主 DLL 文件。

#include <cliext\adapter>
#include <cliext\algorithm>
#include <cliext\vector>
#include <vector>
#include <msclr\marshal_cppstd.h>
#include "MyAlgorithm.h"
#include "StlClrLibrary.h"
#include "native.h"

void StlClrLibrary::Tools::Shuffle(System::Collections::Generic::IList<System::String^>^ list)
{
	// 将列表复制到本地矢量
	cliext::vector<System::String^> v(list);
	
	// 随机排列
	cliext::random_shuffle(v.begin(), v.end());

	// 封送回列表
	int i = 0;
	for each(System::String^ str in v)
	{
		list[i]=str;
		i++;
	}
}

void StlClrLibrary::Tools::BubbleSort(System::Collections::Generic::IList<System::String^>^ list)
{
	//您无需创建临时矢量副本，可以使用集合适配器
	cliext::collection_adapter<System::Collections::Generic::IList<System::String^>> adapter(list);
	MyAlgor::BubbleSort(adapter.begin(),adapter.end());
}

void StlClrLibrary::Tools::BubbleStep(System::Collections::Generic::IList<System::String^>^ list)
{
	//您无需创建临时矢量副本，可以使用集合适配器
	typedef cliext::collection_adapter<System::Collections::Generic::IList<System::String^>> MyCont;
	MyCont adapter(list);
	
	// 这不完全是冒泡排序法，但是研究一下它是如何排序的也很有意思。
	MyCont::iterator prev, current; 
	prev = current = adapter.begin();
	current++;
	for( ; current < adapter.end(); prev=current, ++current)
	{
		// 进行交换时退出此函数
		if (MyAlgor::BubbleStep(prev, current))
			break;
	}
}

// 和 BubbleSort() 相同，但这是本机版本
void StlClrLibrary::Tools::BubbleSortNative(System::Collections::Generic::IList<System::String^>^ list)
{
	// 首先封送到本机的本地矢量
	std::vector<std::string> native_vec;
	for each(System::String^ str in list)
	{
		native_vec.push_back(msclr::interop::marshal_as<std::string>(str));
	}

	// 调用将执行排序操作的本机函数
	Native::BubbleSort(native_vec);
	
	// 封送回列表
	int i = 0;
	for each(std::string str in native_vec)
	{
		list[i]=msclr::interop::marshal_as<System::String^>(str);
		i++;
	}
}

