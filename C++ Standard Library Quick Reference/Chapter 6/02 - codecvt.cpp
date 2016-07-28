#include <iostream>
#include <string>
#include <codecvt>
#include <fstream>

// Forward all constructors and make protected destructor public:
template <class Base> class deletable : public Base {
public:
	template <typename... Params>
	deletable(Params&&... params) : Base(std::forward<Params>(params)...) {}
	~deletable() {}
};

int main()
{
	std::string s(u8"字符串"); // UTF-8 encoding of Chinese word for "string"
	std::cout << s.length();   // Length: 9 code units!

	/*  Does not compile. See text for the explanation.
	{
		typedef std::codecvt<char32_t, char, std::mbstate_t> cvt;
		std::wstring_convert<cvt, char32_t> convertor;
		std::u32string s_u32 = convertor.from_bytes(s);
		std::cout << s_u32.length(); // Length: 3 code units
	}
	*/


	{ // Does not work in Visual C++ 2015. See text.
		typedef deletable<std::codecvt<char32_t, char, std::mbstate_t>> cvt;
		std::wstring_convert<cvt, char32_t> convertor;
		std::u32string s_u32 = convertor.from_bytes(s);
		std::cout << s_u32.length(); // Length: 3 code units
	}


	{ // Does not work in Visual C++ 2015. See text.
		typedef deletable<std::codecvt_byname<char32_t, char, std::mbstate_t>> cvt;
		std::wstring_convert<cvt, char32_t> convertor(new cvt(""));
		std::u32string s_u32 = convertor.from_bytes(s);
		std::cout << s_u32.length(); // Length: 3 code units
	}


	{ // Works fine with Visual C++ 2015.
		std::ofstream out("test.txt");	  // char-based file output stream
		std::wbuffer_convert<std::codecvt_utf8<wchar_t>> cvt(out.rdbuf());
		std::wostream wout(&cvt);         // wchar_t output stream
		wout << L"I am written as UTF-8, irrespective of the native wide format!";
	}

	std::cout << std::endl;
}
