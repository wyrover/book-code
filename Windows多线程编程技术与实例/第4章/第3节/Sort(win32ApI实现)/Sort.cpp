// Sort.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#include <time.h>
#include <sys/timeb.h>




#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	SortMulti(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


///////////////////////////////////////////////////////////////////////////
//下面的结构是用于传送排序的数据给各个排序子线程
struct ThreadPrama
{
	long* data;
	int iLength;
	HWND hDlg;
};

struct _timeb timetr1;
struct _timeb timetr2;

ThreadPrama* StructData1 ;
ThreadPrama* StructData2 ;
ThreadPrama* StructData3 ;
ThreadPrama* StructData4 ;
long *data;
int iDataLen;
UINT m_nTimer;
time_t sec1,sec2;


HANDLE hThread[4];

void OnButtonSort(HWND hDlg) ;
int getData(HWND hDlg);
void PrintResult(HWND hDlg ,long* Array, int iLength, int seqList);
void OnButtonClear(HWND hDlg );





//排序函数
DWORD WINAPI BubbleSort(void* theArray);  //冒泡排序
DWORD WINAPI SelectSort(void* theArray);  //选择排序
DWORD WINAPI HeapSort(void* theArray);    //堆排序
DWORD WINAPI InsertSort(void* theArray);  //插入排序
/*以上四个函数的声明必须乎合作为一个线程函数的必要条件才可以使用CreateThread
建立一个线程。#define PASCAL __stdcall  #define WINAPI FAR PASCAL
（1）调用方法必须是__stdcall，即函数参数压栈顺序由右到左，而且由函数本身负责
栈的恢复, C和C++默认是__cdecl, 所以要显式声明是__stdcall
（2）返回值必须是unsigned long
（3）参数必须是一个32位值，如一个指针值或long类型
 (4) 如果函数是类成员函数，必须声明为static函数，在CreateThread时函数指针有特
 殊的写法。如下(函数是类CThreadTest的成员函数中)：
 static unsigned long _stdcall MyThreadFun(void* pParam);
 handleRet = CreateThread(NULL, 0, &CThreadTestDlg::MyThreadFun, NULL, 0, &ThreadID);
 之所以要声明为static是由于，该函数必须要独立于对象实例来使用，即使没有声明实例也可以
 使用。
*/

int QuickSort(long* Array, int iLow, int iHigh);     //快速排序

//////////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SORT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SORT);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_SORT);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_SORT;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_CREATE:
			 DialogBox(hInst, (LPCTSTR)IDD_SORT_DIALOG, hWnd, (DLGPROC)SortMulti);
			 break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				case IDM_SORT:
				   DialogBox(hInst, (LPCTSTR)IDD_SORT_DIALOG, hWnd, (DLGPROC)SortMulti);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


// Mesage handler for sort box.
LRESULT CALLBACK SortMulti(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hBubbleList = GetDlgItem(hDlg,IDC_LIST_BUBBLE);
	HWND hHeadList = GetDlgItem(hDlg,IDC_LIST_HEAP);
	HWND hInsertList = GetDlgItem(hDlg,IDC_LIST_INSERT);
	HWND hQuickList = GetDlgItem(hDlg,IDC_LIST_QUICK);
	HWND hSelectList = GetDlgItem(hDlg,IDC_LIST_SELECT);

	DWORD dw = 0;


	switch (message)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hDlg,IDC_RADIO_MULTI,IDC_RADIO_SINGLE,IDC_RADIO_SINGLE);
		ListView_SetExtendedListViewStyle(hBubbleList,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
		ListView_DeleteAllItems( hBubbleList);

		LVCOLUMN column;
		column.mask = LVCF_TEXT;
		column.pszText = "次序";
		ListView_InsertColumn(hBubbleList,0,&column);
		column.pszText = "数据";
		ListView_InsertColumn(hBubbleList,1,&column);		
		ListView_SetColumnWidth( hBubbleList, 0, 50);
		ListView_SetColumnWidth( hBubbleList, 1, 50);

		ListView_SetExtendedListViewStyle(hHeadList,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
		ListView_DeleteAllItems( hHeadList);
		column.pszText = "次序";
		ListView_InsertColumn(hHeadList,0,&column);
		column.pszText = "数据";
		ListView_InsertColumn(hHeadList,1,&column);		
		ListView_SetColumnWidth( hHeadList, 0, 50);
		ListView_SetColumnWidth( hHeadList, 1, 50);

		ListView_SetExtendedListViewStyle(hInsertList,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
		ListView_DeleteAllItems( hInsertList);
		column.pszText = "次序";
		ListView_InsertColumn(hInsertList,0,&column);
		column.pszText = "数据";
		ListView_InsertColumn(hInsertList,1,&column);		
		ListView_SetColumnWidth( hInsertList, 0, 50);
		ListView_SetColumnWidth( hInsertList, 1, 50);

		ListView_SetExtendedListViewStyle(hQuickList,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
		ListView_DeleteAllItems( hQuickList);
		column.pszText = "次序";
		ListView_InsertColumn(hQuickList,0,&column);
		column.pszText = "数据";
		ListView_InsertColumn(hQuickList,1,&column);		
		ListView_SetColumnWidth( hQuickList, 0, 50);
		ListView_SetColumnWidth( hQuickList, 1, 50);

		ListView_SetExtendedListViewStyle(hSelectList,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
		ListView_DeleteAllItems( hSelectList);
		column.pszText = "次序";
		ListView_InsertColumn(hSelectList,0,&column);
		column.pszText = "数据";
		ListView_InsertColumn(hSelectList,1,&column);		
		ListView_SetColumnWidth( hSelectList, 0, 50);
		ListView_SetColumnWidth( hSelectList, 1, 50);		
		return TRUE;
	case WM_TIMER:
		dw = WaitForMultipleObjects(4,hThread,TRUE, 1);
		if (dw != WAIT_TIMEOUT) {
			CloseHandle(hThread[0]);
			CloseHandle(hThread[2]);
			CloseHandle(hThread[3]);  
			CloseHandle(hThread[4]);

			delete[] StructData1->data;
			delete StructData1;
			delete[] StructData2->data;
			delete StructData2;
			delete[] StructData3->data;
			delete StructData3;
			delete[] StructData4->data;
			delete StructData4;
			
			_ftime( &timetr2);		
			time( &sec2);
			long milliseconds=(sec2-sec1)*1000+(timetr2.millitm-timetr1.millitm);
			
			CString str;
			str.Format("%d",milliseconds);
			SetWindowText(GetDlgItem(hDlg,IDC_EDIT_TIME),str);
			KillTimer(hDlg,m_nTimer);
 
			TRACE("---------in-------------TIMER---\n");
		}

		
		return TRUE;
		
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
		
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_SORT) {
			OnButtonSort(hDlg); 
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLEAR) {
			OnButtonClear(hDlg); 
			return TRUE;
		}		
		break;
	}
    return FALSE;
}



void OnButtonSort(HWND hDlg) 
{
	// TODO: Add your control notification handler code here
//	long* data1;// ,*data,*data2,*data3,*data4,*data5
	getData(hDlg);

	time( &sec1);
	_ftime( &timetr1);

	//为了对各个子线程分别对原始数据进行排序和保存排序结果
	//分别分配内存对data数组的数据进行复制
	long *data1, *data2, *data3, *data4;

	StructData1 = new ThreadPrama;
	StructData2 = new ThreadPrama;
	StructData3 = new ThreadPrama;
	StructData4 = new ThreadPrama;
	data1 = new long[iDataLen];
	
	memcpy(data1, data, iDataLen << 2); //把data中的数据复制到data1中
	//内存复制 memcpy(目标内存指针, 源内存指, 针复制字节数), 因为long的长度
	//为4字节,所以复制的字节数为iDataLen << 2, 即等于iDataLen*4
	StructData1->data = data1;
	StructData1->iLength = iDataLen;
	StructData1->hDlg = hDlg;

	data2 = new long[iDataLen];
	memcpy(data2, data, iDataLen << 2);
	StructData2->data = data2;
	StructData2->iLength = iDataLen;
	StructData2->hDlg = hDlg;

	data3 = new long[iDataLen];
	memcpy(data3, data, iDataLen << 2);
	StructData3->data = data3;
	StructData3->iLength = iDataLen;
	StructData3->hDlg = hDlg;

	data4 = new long[iDataLen];
	memcpy(data4, data, iDataLen << 2);
	StructData4->data = data4;
	StructData4->iLength = iDataLen;
	StructData4->hDlg = hDlg;



	unsigned long TID1, TID2, TID3, TID4 ; 



	if (IsDlgButtonChecked( hDlg, IDC_RADIO_MULTI) == BST_CHECKED) {
		m_nTimer=SetTimer(hDlg,1, 100, 0);	
		//分别建立各个子线程
		hThread[0] = CreateThread(NULL, 0, &BubbleSort, StructData1, NULL, &TID1);
		hThread[1] = CreateThread(NULL, 0, &SelectSort, StructData2, NULL, &TID2);
		hThread[2] = CreateThread(NULL, 0, &HeapSort, StructData3, NULL, &TID3);
		hThread[3] = CreateThread(NULL, 0, &InsertSort, StructData4, NULL, &TID4);
		
	
	}	
	else{
		
		BubbleSort(StructData1);
		SelectSort(StructData2);
		HeapSort(StructData3);
		InsertSort(StructData4);
		
		delete[] StructData1->data;
		delete StructData1;
		delete[] StructData2->data;
		delete StructData2;
		delete[] StructData3->data;
		delete StructData3;
		delete[] StructData4->data;
		delete StructData4;
		
		_ftime( &timetr2);		
		time( &sec2);
		long milliseconds=(sec2-sec1)*1000+(timetr2.millitm-timetr1.millitm);
		
		CString str;
		str.Format("%d",milliseconds);
		SetWindowText(GetDlgItem(hDlg,IDC_EDIT_TIME),str);
		
	}
	

	delete[] data;


}

/*
冒泡排序思想(升序，降序同理，后面的算法一样都是升序):
从头到尾对数据进行两两比较进行交换,小的放前大的放后。
这样一次下来，最大的元素就会被交换的最后，然后下一次
循环就不用对最后一个元素进行比较交换了，所以呢每一次
比较交换的次数都比上一次循环的次数少一，这样n之后
数据就变得升序排列了
*/
DWORD WINAPI BubbleSort(void* theArray)
{
	long* arrayBubble = ((ThreadPrama*)theArray)->data;
	int iLengthBubble = ((ThreadPrama*)theArray)->iLength;
	HWND hDlg = ((ThreadPrama*)theArray)->hDlg;

	int i, j=0;
	long swap;


	for (i = iLengthBubble-1;  i > 0; i--)
	{
		for(j = 0; j < i; j++)
		{
			if(arrayBubble[j] > arrayBubble[j+1])  //前比后大，交换
			{
				swap = arrayBubble[j];
				arrayBubble[j] = arrayBubble[j+1];
				arrayBubble[j+1] = swap;
			}
			for(int k=0;k<100000;k++);
		 	Sleep(1);
		}
	}

	PrintResult(hDlg,arrayBubble,iLengthBubble, 0); 

return 0;	
}


DWORD WINAPI HeapSort(void* theArray)
{
	long* Array = ((ThreadPrama*)theArray)->data;
	int iLength = ((ThreadPrama*)theArray)->iLength;
	HWND hDlg = ((ThreadPrama*)theArray)->hDlg;

	int i, j, p;
	long swap;

	for(i=0; i<iLength-1; i++)
	{
		for(j = iLength - 1; j>i; j--) //从最后倒数上去比较字节点和父节点
		{
			p = (j - i - 1)/2 + i;   //计算父节点数组下标
			//注意到树节点序数跟数组下标不是等同的，因为建堆的元素个数逐个递减

			if(Array[j] < Array[p])  //如果父节点数值大则交换父节点和字节点
			{
				swap = Array[j];
				Array[j] = Array[p];
				Array[p] = swap;
			}
		
			Sleep(1);
		}
	}
	
	PrintResult(hDlg,Array,iLength, 1); 
	return 0;
}

DWORD WINAPI InsertSort(void* theArray)
{
	long* Array = ((ThreadPrama*)theArray)->data;
	int iLength = ((ThreadPrama*)theArray)->iLength;
	HWND hDlg = ((ThreadPrama*)theArray)->hDlg;

	int i=1, j=0;
	long temp;
	for(i=1; i<iLength; i++)
	{
		temp = Array[i];   //取出序列后面无序数据的第一个元素值
		for(j=i; j>0; j--) //和前面的有序数据逐个进行比较找出合适的插入位置
		{

			Sleep(1);

			if(Array[j - 1] > temp)  //如果该元素比插入值大则后移
				Array[j] = Array[j - 1];
			else //如果该元素比插入值小，那么该位置的后一位就是插入元素的位置
				break;   
		}
		Array[j] = temp;
	}


	PrintResult(hDlg,Array,iLength, 2); 

	return 0;
}

/*
快速排序思想：
快速排序是分治思想的一种应用，它先选取一个支点，然后把小于支点的元素交换
到支点的前边，把大于支点的元素交换到支点的右边。然后再对支点左边部分和右
边部分进行同样的处理，这样若干次之后，数据就会变得有序。

下面的实现使用了递归
建立两个游标：iLow，iHigh；iLow指向序列的第一个元素，iHigh指向最后一个

先选第一个元素作为支点，并把它的值存贮在一个辅助变量里。那么第一个位置就
变为空并可以放置其他的元素。 这样从iHigh指向的元素开始向前移动游标iHigh
查找比支点小的元素，如果找到，则把它放置到空置了的位置（现在是第一个位置）

然后iHigh游标停止移动，这时iHigh指向的位置被空置，然后移动iLow游标寻找比
支点大的元素放置到iHigh指向的空置的位置，如此往复直到iLow与iHigh相等。
最后使用递归对左右两部分进行同样处理
*/

int QuickSort(long* Array, int iLow, int iHigh)
{
	if(iLow >= iHigh) return 1; //递归结束条件

	long pivot = Array[iLow];

	int iLowSaved = iLow, iHighSaved = iHigh;  //保未改变的iLow，iHigh值保存起来

	while (iLow < iHigh)
	{

		for(int k=0;k<100000;k++);
		Sleep(5);

		while (Array[iHigh] >= pivot && iHigh > iLow) //寻找比支点大的元素
			iHigh -- ;
		Array[iLow] = Array[iHigh]; //把找到的元素放置到空置的位置

		while (Array[iLow] < pivot && iLow < iHigh) //寻找比支点小的元素
			iLow ++ ;
		Array[iHigh] = Array[iLow]; //把找到的元素放置到空置的位置
	}
	Array[iLow] = pivot; //把支点值放置到支点位置，这时支点位置是空置的

	//对左右部分分别进行递归处理
	QuickSort(Array, iLowSaved, iHigh-1);
	QuickSort(Array, iLow+1, iHighSaved);

	return 0;
}



//每一个线程都使用这个函数进行输出

void PrintResult(HWND hDlg ,long* Array, int iLength, int seqList)
{	
	HWND hBubbleList = GetDlgItem(hDlg,IDC_LIST_BUBBLE);
	HWND hHeadList = GetDlgItem(hDlg,IDC_LIST_HEAP);
	HWND hInsertList = GetDlgItem(hDlg,IDC_LIST_INSERT);
	HWND hQuickList = GetDlgItem(hDlg,IDC_LIST_QUICK);
	HWND hSelectList = GetDlgItem(hDlg,IDC_LIST_SELECT);
	
	int i;
	CString strSeq,strData;
	
	
	LVITEM item;
	item.iSubItem = 0;
	item.mask = LVIF_TEXT;
	item.pszText = "";
	
	switch(seqList) {
	case 0 :
		for (i=0; i<iLength; i++)
		{
			strSeq.Format("%d",i);
			strData.Format("%d",Array[i]);
			item.iItem = i;
			ListView_InsertItem(hBubbleList, &item);
			
			ListView_SetItemText(hBubbleList, i, 0,strSeq.GetBuffer(10) );// 
			strSeq.ReleaseBuffer();
			ListView_SetItemText(hBubbleList, i, 1, strData.GetBuffer(10));
			strData.ReleaseBuffer();	
		}
		break;
	case 1:
		for (i=0; i<iLength; i++)
		{
			strSeq.Format("%d",i);
			strData.Format("%d",Array[i]);
			item.iItem = i;
			ListView_InsertItem(hHeadList, &item);
			
			ListView_SetItemText(hHeadList, i, 0,strSeq.GetBuffer(10) );// 
			strSeq.ReleaseBuffer();
			ListView_SetItemText(hHeadList, i, 1, strData.GetBuffer(10));
			strData.ReleaseBuffer();	
		}
		break;
	case 2:
		for (i=0; i<iLength; i++)
		{
			strSeq.Format("%d",i);
			strData.Format("%d",Array[i]);
			item.iItem = i;
			ListView_InsertItem(hInsertList, &item);
			
			ListView_SetItemText(hInsertList, i, 0,strSeq.GetBuffer(10) );// 
			strSeq.ReleaseBuffer();
			ListView_SetItemText(hInsertList, i, 1, strData.GetBuffer(10));
			strData.ReleaseBuffer();
			
		}
		break;
	case 3:
		for (i=0; i<iLength; i++)
		{
			strSeq.Format("%d",i);
			strData.Format("%d",Array[i]);
			item.iItem = i;
			ListView_InsertItem(hQuickList, &item);
			
			ListView_SetItemText(hQuickList, i, 0,strSeq.GetBuffer(10) );// 
			strSeq.ReleaseBuffer();
			ListView_SetItemText(hQuickList, i, 1, strData.GetBuffer(10));
			strData.ReleaseBuffer();
		}
		break;
	default:
		for (i=0; i<iLength; i++)
		{
			strSeq.Format("%d",i);
			strData.Format("%d",Array[i]);
			item.iItem = i;
			ListView_InsertItem(hSelectList, &item);
			
			ListView_SetItemText(hSelectList, i, 0,strSeq.GetBuffer(10) );// 
			strSeq.ReleaseBuffer();
			ListView_SetItemText(hSelectList, i, 1, strData.GetBuffer(10));
			strData.ReleaseBuffer();
		}
		
	}

}







/*
选择排序思想:
每一次都从无序的数据中找出最小的元素，然后和前面已经有序的元素序列
的后一个元素进行交换，这样整个源序列就会分成两部分，前面一部分是已经
排好序的有序序列，后面一部分是无序的，用于选出最小的元素。
循环N次之后，前面的有序序列加长到跟源序列一样长，后面的无序部分长度变
为0，排序就完成了。
*/
DWORD WINAPI SelectSort(void* theArray)
{
	long* Array = ((ThreadPrama*)theArray)->data;
	int iLength = ((ThreadPrama*)theArray)->iLength;
	HWND hDlg = ((ThreadPrama*)theArray)->hDlg;

	long lMin, lSwap;
	int i, j, iMinPos;

	for(i=0; i < iLength-1; i++)
	{
		lMin = Array[i];
		iMinPos = i;

		for(j=i + 1; j <= iLength-1; j++) //从无序的元素中找出最小的元素
		{
			if(Array[j] < lMin)
			{
				iMinPos = j;
				lMin = Array[j];
			}

	

		}
		//把选出的元素交换拼接到有序序列的最后
		lSwap = Array[i];
		Array[i] = Array[iMinPos];
		Array[iMinPos] = lSwap;


	}
	PrintResult(hDlg,Array,iLength, 4);


	return 0;
}

//******************************************************************************

void OnButtonClear(HWND hDlg )
{

	
	HWND hBubbleList = GetDlgItem(hDlg,IDC_LIST_BUBBLE);
	HWND hHeadList = GetDlgItem(hDlg,IDC_LIST_HEAP);
	HWND hInsertList = GetDlgItem(hDlg,IDC_LIST_INSERT);
	HWND hQuickList = GetDlgItem(hDlg,IDC_LIST_QUICK);
	HWND hSelectList = GetDlgItem(hDlg,IDC_LIST_SELECT);

	ListView_DeleteAllItems( hBubbleList);
	ListView_DeleteAllItems( hHeadList);
	ListView_DeleteAllItems( hInsertList);
	ListView_DeleteAllItems( hQuickList);
	ListView_DeleteAllItems( hSelectList);

	SetWindowText(GetDlgItem(hDlg,IDC_EDIT_TIME),"");
	SetWindowText(GetDlgItem(hDlg,IDC_EDIT_DATA),"");
	
}


int getData(HWND hDlg)
{
	iDataLen = 0;
	CString m_strData = "";
	LPTSTR p = m_strData.GetBuffer( 500 );
	GetWindowText(GetDlgItem(hDlg,IDC_EDIT_DATA),p,300);
	m_strData.ReleaseBuffer();

	int i=0,n=0,iTotal;
	CString strTemp,str;

	TRACE("%s---------------------\n",m_strData);
	while (n!=-1){
		n = m_strData.Find(',', i);
		i = n+1;
		if (n!=-1) {
			iDataLen++;
		}
	}
	
	iDataLen++;
	TRACE("----------iDataLen------------%d--------------------------------------------\n",iDataLen);
	strTemp = m_strData;
	data = new long[iDataLen];
	iTotal = m_strData.GetLength();
	for(i=0;i<iDataLen-1;i++)
	{
		n=strTemp.Find(',');
		str=strTemp.Left(n);
		data[i] = atol(str);
		iTotal = iTotal-n-1;
		strTemp=strTemp.Right(iTotal);
	}
	data[i]=atol(strTemp);
	
	for(i=0;i<iDataLen;i++)
	{
		TRACE("-----------iDataLen-1-------------%d---\n",data[i]);
	}	
	return iDataLen;
}
