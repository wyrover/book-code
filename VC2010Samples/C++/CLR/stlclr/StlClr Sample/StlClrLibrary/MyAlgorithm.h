// 注意如何编译冒泡排序算法，使之能用于本机代码和托管代码。

#ifdef _M_CEE
#include <cliext\memory>
#define _STD_OR_CLIEXT cliext
#else
#include <memory>
#define _STD_OR_CLIEXT std
#endif

namespace MyAlgor
{
	template<class _FwdIt>
	bool BubbleStep(_FwdIt begin, _FwdIt end)
	{
		if (*begin > *end)
		{
			_STD_OR_CLIEXT::swap(*begin, *end);	
			return true;
		}
		return false;
	}

	template <class _FwdIt>
	void BubbleSort(_FwdIt begin, _FwdIt end)
	{
		for(_FwdIt it = begin; it != end; ++it)
		{
			_FwdIt prev = begin;
			_FwdIt current = prev;
			current++;
			while(current != end)
			{
				BubbleStep(prev, current);
				prev=current;
				current++;
			}
		}
	}
};