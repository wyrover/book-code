#include <stdio.h> 
int M; 
void ZH(int n,int m,int s[],int total) 
{ int i,j,k,flag=0; 
 if(m==0) 
 {for(i=1;i<=n;i++) 
 {flag=0; 
 for(k=M-1;k>0;k--) 
 if (s[k]==i) {flag++;break;} 
 if (flag==0) 
 { 
 s[0]=i; 
 for(j=0;j<M;j++) 
 printf("%d ",s[j]); 
 total++; 
 printf("\n"); 
 } 

 } 
 } 
 else for(i=1;i<=n;i++) 
 {for(k=M-1;k>m;k--) 
 if (s[k]==i) {flag++;break;} 
 if (flag==0) 
 { 
 s[m]=i; 
 ZH(n,m-1,s,total); 
 } 
 } 
} 
main() 
{ 
int s[100],n,m,total=0; 
printf("input n m\n"); 
scanf("%d %d",&n,&m); 
s[m]=0; 
M=m; 
ZH(n,m-1,s,total); 

getchar(); 
getchar(); 

} 

