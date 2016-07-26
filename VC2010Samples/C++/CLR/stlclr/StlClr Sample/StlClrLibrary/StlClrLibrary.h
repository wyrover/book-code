// StlClrLibrary.h

#pragma once

using namespace System;

namespace StlClrLibrary {

	public ref class Tools
	{
		// TODO: 在此处添加此类的方法。
			
	public:
		void Shuffle(System::Collections::Generic::IList<System::String^>^ list);
		void BubbleSort(System::Collections::Generic::IList<System::String^>^ list);
		void BubbleSortNative(System::Collections::Generic::IList<System::String^>^ list);
		void BubbleStep(System::Collections::Generic::IList<System::String^>^ list);
	private:
	};
}
