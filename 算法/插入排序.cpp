#include <iostream.h> 
void InsertSort(int* pData,int Count) 
{ 
  int iTemp; 
  int iPos; 
  for(int i=1;i<Count;i++) 
  { 
    iTemp = pData[i]; 
    iPos = i-1; 
    while((iPos>=0) && (iTemp<pData[iPos])) 
    { 
      pData[iPos+1] = pData[iPos]; 
      iPos--; 
    } 
    pData[iPos+1] = iTemp; 
  } 
} 

void main() 
{ 
  int data[] = {10,9,8,7,6,5,4}; 
  InsertSort(data,7); 
  for (int i=0;i<7;i++) 
    cout<<data[i]<<" "; 
  cout<<"\n"; 
} 

