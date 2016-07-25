// sequenceMultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sequenceMult.h"
#include "sequenceMultDlg.h"


#include "stdlib.h"
#include "memory.h"

#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSequenceMultDlg dialog

CSequenceMultDlg::CSequenceMultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSequenceMultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSequenceMultDlg)
	m_strData = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	hasPrintBubble=FALSE;
	hasPrintHeap = FALSE;
	hasPrintSelect=FALSE;
	hasPrintInsert=FALSE;
	iDataLen = 0;

}

void CSequenceMultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSequenceMultDlg)
	DDX_Control(pDX, IDC_LIST_SELECT, m_selectList);
	DDX_Control(pDX, IDC_LIST_QUICK, m_quickList);
	DDX_Control(pDX, IDC_LIST_INSERT, m_insertList);
	DDX_Control(pDX, IDC_LIST_HEAP, m_headList);
	DDX_Control(pDX, IDC_LIST_BUBBLE, m_bubbleList);
	DDX_Text(pDX, IDC_EDIT_DATA, m_strData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSequenceMultDlg, CDialog)
	//{{AFX_MSG_MAP(CSequenceMultDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SORT, OnButtonSort)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSequenceMultDlg message handlers


///////////////////////////////////////////////////////////////////////////



HANDLE evtTerminate; //事件信号,标记是否所有子线程都执行完
HANDLE evtPrint;
HANDLE evtHeap;
HANDLE evtBubble;
HANDLE evtInsert;
HANDLE evtQuick;
HANDLE evtSelect;



/*
下面使用了三种控制方法，你可以注释其中两种，使用其中一种。
注意修改时要连带修改临界区PrintResult里的相应控制语句
*/
//HANDLE evtPrint;   //事件信号,标记事件是否已发生
//CRITICAL_SECTION csPrint;   //临界区
//HANDLE mtxPrint; //互斥信号,如有信号表明已经有线程进入临界区并拥有此信号

static long ThreadCompleted = 0;  




/*用来标记四个子线程中已完成线程的个数,
当一个子线程完成时就对ThreadCompleted进行加一操作, 
要使用InterlockedIncrement(long* lpAddend)和
InterlockedDecrement(long* lpAddend)进行加减操作
*/

//下面的结构是用于传送排序的数据给各个排序子线程
struct MySafeArray
{
	long* data;
	int iLength;
};


//打印每一个线程的排序结果



//排序函数
unsigned long __stdcall BubbleSort(void* theArray);  //冒泡排序
unsigned long __stdcall SelectSort(void* theArray);  //选择排序
unsigned long __stdcall HeapSort(void* theArray);    //堆排序
unsigned long __stdcall InsertSort(void* theArray);  //插入排序
/*以上四个函数的声明必须乎合作为一个线程函数的必要条件才可以使用CreateThread
建立一个线程。
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




///////////////////////////////////////////////////////////////////////////////////







BOOL CSequenceMultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// TODO: Add extra initialization her
	((CButton*)GetDlgItem(IDC_RADIO_MULTI))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_SINGLE))->SetCheck(1);	

	RECT rect;
	m_bubbleList.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	ListView_SetExtendedListViewStyle(m_bubbleList.m_hWnd,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_bubbleList.DeleteAllItems();
	m_bubbleList.InsertColumn(0,"次序");
	m_bubbleList.InsertColumn(1,"数据");
	m_bubbleList.GetWindowRect(&rect);
	int nTotalX = rect.right-rect.left;
	m_bubbleList.SetColumnWidth(0,2*nTotalX/5);
	m_bubbleList.SetColumnWidth(1,3*nTotalX/5);

	m_headList.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	ListView_SetExtendedListViewStyle(m_headList.m_hWnd,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_headList.DeleteAllItems();
	m_headList.InsertColumn(0,"次序");
	m_headList.InsertColumn(1,"数据");
	m_headList.GetWindowRect(&rect);
	nTotalX = rect.right-rect.left;
	m_headList.SetColumnWidth(0,2*nTotalX/5);
	m_headList.SetColumnWidth(1,3*nTotalX/5);

	m_insertList.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	ListView_SetExtendedListViewStyle(m_insertList.m_hWnd,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_insertList.DeleteAllItems();
	m_insertList.InsertColumn(0,"次序");
	m_insertList.InsertColumn(1,"数据");
	m_insertList.GetWindowRect(&rect);
	nTotalX = rect.right-rect.left;
	m_insertList.SetColumnWidth(0,2*nTotalX/5);
	m_insertList.SetColumnWidth(1,3*nTotalX/5);

	m_quickList.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	ListView_SetExtendedListViewStyle(m_quickList.m_hWnd,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_quickList.DeleteAllItems();
	m_quickList.InsertColumn(0,"次序");
	m_quickList.InsertColumn(1,"数据");
	m_quickList.GetWindowRect(&rect);
	nTotalX = rect.right-rect.left;
	m_quickList.SetColumnWidth(0,2*nTotalX/5);
	m_quickList.SetColumnWidth(1,3*nTotalX/5);

	m_selectList.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	ListView_SetExtendedListViewStyle(m_selectList.m_hWnd,LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_selectList.DeleteAllItems();
	m_selectList.InsertColumn(0,"次序");
	m_selectList.InsertColumn(1,"数据");
	m_selectList.GetWindowRect(&rect);
	nTotalX = rect.right-rect.left;
	m_selectList.SetColumnWidth(0,2*nTotalX/5);
	m_selectList.SetColumnWidth(1,3*nTotalX/5);



	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSequenceMultDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSequenceMultDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSequenceMultDlg::OnButtonSort() 
{
	// TODO: Add your control notification handler code here
	getData();


	time( &sec1);
	_ftime( &timetr1);




//	long data[] = {907,788,566,556,3434,4556,2234,4354,5656,23445,1,3,2,54,13,112,23,56,67,76,66,55,33,22,11,76,78,89,85,57,200,3003,1235,678,345,678,789,4567,6789,1223,1004,123,34,546,754,34,74,3,56,2,4,6,111,121,145,123,1111,1234,1543,1789,1987};
//	iDataLen = 61;

	//为了对各个子线程分别对原始数据进行排序和保存排序结果
	//分别分配内存对data数组的数据进行复制

	MySafeArray StructData1, StructData2, StructData3, StructData4;

	data1 = new long[iDataLen];
	
	memcpy(data1, data, iDataLen << 2); //把data中的数据复制到data1中
	//内存复制 memcpy(目标内存指针, 源内存指, 针复制字节数), 因为long的长度
	//为4字节,所以复制的字节数为iDataLen << 2, 即等于iDataLen*4
	StructData1.data = data1;
	StructData1.iLength = iDataLen;

	data2 = new long[iDataLen];
	memcpy(data2, data, iDataLen << 2);
	StructData2.data = data2;
	StructData2.iLength = iDataLen;

	data3 = new long[iDataLen];
	memcpy(data3, data, iDataLen << 2);
	StructData3.data = data3;
	StructData3.iLength = iDataLen;

	data4 = new long[iDataLen];
	memcpy(data4, data, iDataLen << 2);
	StructData4.data = data4;
	StructData4.iLength = iDataLen;

	data5 = new long[iDataLen];
	memcpy(data5, data, iDataLen << 2);


	unsigned long TID1, TID2, TID3, TID4 ; 


	//对信号量进行初始化

	evtTerminate = CreateEvent(NULL, FALSE, FALSE, "Terminate");
	evtPrint = CreateEvent(NULL, FALSE, TRUE, "PrintResult");
	evtHeap = CreateEvent(NULL, FALSE, FALSE, "heap");
	evtBubble = CreateEvent(NULL, FALSE, FALSE, "bubble");
	evtInsert = CreateEvent(NULL, FALSE, FALSE, "insert");
	evtQuick = CreateEvent(NULL, FALSE, FALSE, "Quick");
	evtSelect = CreateEvent(NULL, FALSE, FALSE, "select");
	//mtxPrint = CreateMutex(NULL, FALSE, "PrintMutex");
	//InitializeCriticalSection(&csPrint);

	m_nTimer=SetTimer(1, 100, 0);


	if (((CButton*)GetDlgItem(IDC_RADIO_SINGLE))->GetCheck()==0) {
	
	//分别建立各个子线程
	CreateThread(NULL, 0, &BubbleSort, &StructData1, NULL, &TID1);
	CreateThread(NULL, 0, &SelectSort, &StructData2, NULL, &TID2);
	CreateThread(NULL, 0, &HeapSort, &StructData3, NULL, &TID3);
	CreateThread(NULL, 0, &InsertSort, &StructData4, NULL, &TID4);
	TRACE("--------------------enter thread----------\n");
		
	}	
	else{
		
	BubbleSort(&StructData1);
	SelectSort(&StructData2);
	HeapSort(&StructData3);
	InsertSort(&StructData4);
	}


	//在主线程中执行行快速排序，其他排序在子线程中执行
// 	QuickSort(data5, 0, iDataLen - 1);
	


	


	delete[] data5;
	delete[] data;

//	CloseHandle(evtPrint);


}

/*
冒泡排序思想(升序，降序同理，后面的算法一样都是升序):
从头到尾对数据进行两两比较进行交换,小的放前大的放后。
这样一次下来，最大的元素就会被交换的最后，然后下一次
循环就不用对最后一个元素进行比较交换了，所以呢每一次
比较交换的次数都比上一次循环的次数少一，这样N次之后
数据就变得升序排列了
*/
unsigned long __stdcall BubbleSort(void* theArray)
{
	long* arrayBubble = ((MySafeArray*)theArray)->data;
	int iLengthBubble = ((MySafeArray*)theArray)->iLength;

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


	InterlockedIncrement(&ThreadCompleted);      //返回前使线程完成数标记加1
	if(ThreadCompleted == 4) SetEvent(evtTerminate);
	SetEvent(evtBubble); //检查是否其他线程都已执行完
	//若都执行完则设置程序结束信号量

return 0;	
}


unsigned long __stdcall HeapSort(void* theArray)
{
	long* Array = ((MySafeArray*)theArray)->data;
	int iLength = ((MySafeArray*)theArray)->iLength;

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
	

//	PrintResult(Array, iLength, "Heap Sort"); //向控制台打印排序结果
	InterlockedIncrement(&ThreadCompleted);   //返回前使线程完成数标记加1
	if(ThreadCompleted == 4) SetEvent(evtTerminate); //检查是否其他线程都已执行完
	//若都执行完则设置程序结束信号量
	SetEvent(evtHeap);
	return 0;
}

unsigned long __stdcall InsertSort(void* theArray)
{
	long* Array = ((MySafeArray*)theArray)->data;
	int iLength = ((MySafeArray*)theArray)->iLength;

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



	InterlockedIncrement(&ThreadCompleted);  //返回前使线程完成数标记加1
	if(ThreadCompleted == 4)  //检查是否其他线程都已执行完
		SetEvent(evtTerminate);//若都执行完则设置程序结束信号量
	
	SetEvent(evtInsert);
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

//*****************************临界区***************************************

//每一个线程都要使用这个函数进行输出，而且只有一个显示器，产生多个线程
//竞争对控制台的使用权。
void CSequenceMultDlg::PrintResult(long* Array, int iLength, int seqList)
{

WaitForSingleObject(evtPrint, 0);
	
	int i;
	CString strSeq,strData;
	switch(seqList) {
	case 0 :
		for (i=0; i<iLength; i++)
		{
			strSeq.Format("%d",i);
			strData.Format("%d",Array[i]);
			m_bubbleList.InsertItem(i,"");
			m_bubbleList.SetItemText(i,0,strSeq);
			m_bubbleList.SetItemText(i,1,strData);
			
		}
		break;
	case 1:
		for (i=0; i<iLength; i++)
		{
			strSeq.Format("%d",i);
			strData.Format("%d",Array[i]);
			m_headList.InsertItem(i,"");
			m_headList.SetItemText(i,0,strSeq);
			m_headList.SetItemText(i,1,strData);
			
		}
		break;
	case 2:
		for (i=0; i<iLength; i++)
		{
			strSeq.Format("%d",i);
			strData.Format("%d",Array[i]);
			m_insertList.InsertItem(i,"");
			m_insertList.SetItemText(i,0,strSeq);
			m_insertList.SetItemText(i,1,strData);
			
		}
		break;
	case 3:
		for (i=0; i<iLength; i++)
		{
			strSeq.Format("%d",i);
			strData.Format("%d",Array[i]);
			m_quickList.InsertItem(i,"");
			m_quickList.SetItemText(i,0,strSeq);
			m_quickList.SetItemText(i,1,strData);
			
		}
		break;
	default:
		for (i=0; i<iLength; i++)
		{
			strSeq.Format("%d",i);
			strData.Format("%d",Array[i]);
			m_selectList.InsertItem(i,"");
			m_selectList.SetItemText(i,0,strSeq);
			m_selectList.SetItemText(i,1,strData);
			
		}
		
	}
	SetEvent(evtPrint);
}







/*
选择排序思想:
每一次都从无序的数据中找出最小的元素，然后和前面已经有序的元素序列
的后一个元素进行交换，这样整个源序列就会分成两部分，前面一部分是已经
排好序的有序序列，后面一部分是无序的，用于选出最小的元素。
循环N次之后，前面的有序序列加长到跟源序列一样长，后面的无序部分长度变
为0，排序就完成了。
*/
unsigned long __stdcall SelectSort(void* theArray)
{
	long* Array = ((MySafeArray*)theArray)->data;
	int iLength = ((MySafeArray*)theArray)->iLength;

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

//	PrintResult(Array, iLength, "Select Sort"); //向控制台打印排序结果
	InterlockedIncrement(&ThreadCompleted);  //返回前使线程完成数标记加1
	if(ThreadCompleted == 4) SetEvent(evtTerminate);//检查是否其他线程都已执行完
	//若都执行完则设置程序结束信号量

	SetEvent(evtSelect);
	return 0;
}

//******************************************************************************

void CSequenceMultDlg::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	m_bubbleList.DeleteAllItems();
	m_headList.DeleteAllItems();
	m_insertList.DeleteAllItems();
	m_quickList.DeleteAllItems();
	m_selectList.DeleteAllItems();
	GetDlgItem(IDC_EDIT_TIME)->SetWindowText("");
	GetDlgItem(IDC_EDIT_DATA)->SetWindowText("");

	hasPrintBubble = FALSE;
	hasPrintInsert = FALSE;
	hasPrintSelect = FALSE;
	hasPrintHeap = FALSE;

	ThreadCompleted = 0;

	
}

void CSequenceMultDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

		if (WaitForSingleObject(evtHeap, 0)==WAIT_OBJECT_0) {
			if (!hasPrintHeap) {
			PrintResult(data3, iDataLen, 1);  
			hasPrintHeap=TRUE;
			TRACE("----------print2-------------\n");
			delete[] data3;


			}
		}

		if (WaitForSingleObject(evtBubble, 0)==WAIT_OBJECT_0) {
			TRACE("---------------------------WAIT\n");
			if ( !hasPrintBubble ) 
			{
			
			PrintResult(data1, iDataLen, 0);  
			hasPrintBubble = TRUE;
			TRACE("----------print1-------------\n");
			delete[] data1;

			}
		}

		if (WaitForSingleObject(evtSelect, 0)==WAIT_OBJECT_0) {
			if (!hasPrintSelect) {
				PrintResult(data2, iDataLen, 4);  
				hasPrintSelect = TRUE;
	
				delete[] data2;
			}
		}
		if (WaitForSingleObject(evtInsert, 0)==WAIT_OBJECT_0) {
			 if (!hasPrintInsert){
				PrintResult(data4, iDataLen, 2);  
				hasPrintSelect = TRUE;
		
				delete[] data4;
			}
		}
			
			//等待所有的子线程结束
			//所有的子线程结束后，主线程才可以结束
		if (WaitForSingleObject(evtTerminate, 0)==WAIT_OBJECT_0) {
			_ftime( &timetr2);
			
			time( &sec2);
			milliseconds=(sec2-sec1)*1000+(timetr2.millitm-timetr1.millitm);
			
			CString str;
			str.Format("%d",milliseconds);
			GetDlgItem(IDC_EDIT_TIME)->SetWindowText(str);
			KillTimer(m_nTimer);
		}	
			
// 	CDialog::OnTimer(nIDEvent);
}

void CSequenceMultDlg::getData()
{
	UpdateData(TRUE);
iDataLen=0;
	int i=0,n=0,iTotal;
	CString strTemp,str;

	 while (n!=-1){
		n = m_strData.Find(',', i);
		TRACE("=======---------------n------------%d\n",n);
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
			TRACE("------inin-----iDataLen-1-------------%d---\n",data[i]);
		iTotal = iTotal-n-1;
		strTemp=strTemp.Right(iTotal);
	}
	data[i]=atol(strTemp);

		for(i=0;i<iDataLen;i++)
	{
 	TRACE("-----------iDataLen-1-------------%d---\n",data[i]);
		}	

	


}
