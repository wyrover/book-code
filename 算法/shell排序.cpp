#include <iostream.h> 
void ShellSort(int* pData,int Count) 
{ 
  int step[4]; 
  step[0] = 9; 
  step[1] = 5; 
  step[2] = 3; 
  step[3] = 1; 

  int iTemp; 
  int k,s,w; 
  for(int i=0;i<4;i++) 
  { 
    k = step[i]; 
    s = -k; 
    for(int j=k;j<Count;j++) 
    { 
      iTemp = pData[j]; 
      w = j-k;//求上step个元素的下标 
      if(s ==0) 
      { 
        s = -k; 
        s++; 
        pData[s] = iTemp; 
      } 
      while((iTemp<pData[w]) && (w>=0) && (w<=Count)) 
      { 
        pData[w+k] = pData[w]; 
        w = w-k; 
      } 
      pData[w+k] = iTemp; 
    } 
  } 
} 

void main() 
{ 
  int data[] = {10,9,8,7,6,5,4,3,2,1,-10,-1}; 
  ShellSort(data,12); 
  for (int i=0;i<12;i++) 
    cout<<data[i]<<" "; 
  cout<<"\n"; 
} 

