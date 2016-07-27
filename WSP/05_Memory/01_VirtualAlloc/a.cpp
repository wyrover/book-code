#include <windows.h> 
#include <stdio.h> 
#include <stdlib.h> 
 
int main() 
{ 
    LPVOID lpvAddr; 
    DWORD cbSize; 
    BOOL vLock; 
    LPVOID commit; 
 
    // Amount of memory to allocate.
    cbSize = 512; 
 
    // Try to allocate some memory. 
    lpvAddr = VirtualAlloc(NULL,cbSize,MEM_RESERVE,PAGE_NOACCESS); 
 
    if(lpvAddr == NULL) 
    {
        fprintf(stdout,"VirtualAlloc failed on RESERVE with %ld\n", 
            GetLastError()); 
    }
 
    // Try to commit the allocated memory. 
    commit = 
    VirtualAlloc(NULL,cbSize,MEM_COMMIT,PAGE_READONLY|PAGE_GUARD); 
 
    if(commit == NULL) 
    {
        fprintf(stderr,"VirtualAlloc failed on COMMIT with %ld\n", 
            GetLastError()); 
    }
 
    else 
    {
        fprintf(stderr,"Committed %lu bytes at address %lp\n", 
        cbSize,commit); 
    }
 
    // Try to lock the committed memory. 
    vLock = VirtualLock(commit,cbSize); 
 
    if(!vLock) 
    {
        fprintf(stderr,"Cannot lock at %lp, error = %lu\n", 
            commit,GetLastError()); 
    }
    else fprintf(stderr,"Lock Achieved at %lp\n",commit); 
 
    // Try to lock the committed memory again. 
    vLock = VirtualLock(commit,cbSize); 
 
    if(!vLock) 
    {
        fprintf(stderr,"Cannot get 2nd lock at %lp, error = %lu\n", 
            commit,GetLastError()); 
    }
    else fprintf(stderr,"2nd Lock Achieved at %lp\n",commit); 
 
} 
