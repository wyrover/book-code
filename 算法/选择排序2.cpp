#include <iostream.h> 
void SelectSort(int* pData,int Count) 
{ 
  int iTemp; 
  int iPos; 
  for(int i=0;i<Count-1;i++) 
  { 
    iTemp = pData[i]; 
    iPos = i; 
    for(int j=i+1;j<Count;j++) 
    { 
      if(pData[j]<iTemp) 
      { 
        iTemp = pData[j]; 
        iPos = j; 
      } 
    } 
    pData[iPos] = pData[i]; 
    pData[i] = iTemp; 
  } 
} 

void main() 
{ 
  int data[] = {10,9,8,7,6,5,4}; 
  SelectSort(data,7); 
  for (int i=0;i<7;i++) 
    cout<<data[i]<<" "; 
  cout<<"\n"; 
} 

