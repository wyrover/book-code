#include <iostream.h> 
void Bubble2Sort(int* pData,int Count) 
{ 
  int iTemp; 
  int left = 1; 
  int right =Count -1; 
  int t; 
  do 
  { 
    //正向的部分 
    for(int i=right;i>=left;i--) 
    { 
      if(pData[i]<pData[i-1]) 
      { 
        iTemp = pData[i]; 
        pData[i] = pData[i-1]; 
        pData[i-1] = iTemp; 
        t = i; 
      } 
    } 
    left = t+1; 

    //反向的部分 
    for(int i=left;i<right+1;i++) 
    { 
      if(pData[i]<pData[i-1]) 
      { 
        iTemp = pData[i]; 
        pData[i] = pData[i-1]; 
        pData[i-1] = iTemp; 
        t = i; 
      } 
    } 
    right = t-1; 
  }while(left<=right); 
} 

void main() 
{ 
  int data[] = {10,9,8,7,6,5,4}; 
  Bubble2Sort(data,7); 
  for (int i=0;i<7;i++) 
    cout<<data[i]<<" "; 
  cout<<"\n"; 
} 

