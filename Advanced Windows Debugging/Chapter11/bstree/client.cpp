#include <windows.h>
#include "bstree.h"

void __cdecl wmain()
{
	CBinaryTree* pTree=new CBinaryTree();
	if(pTree)
	{
		pTree->Add(10);
		pTree->Add(5);
		pTree->Add(15);
		pTree->Add(7);
		pTree->Add(13); 

		pTree->Traverse();
        delete pTree;
	}
}