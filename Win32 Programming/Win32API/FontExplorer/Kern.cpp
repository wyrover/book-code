#include "stdafx.h"
#include "kern.h"

/****************************************************************************
*                             KERNINFO::KERNINFO
* Inputs:
*       CDC * dc: Display context
* Effect: 
*       Gets the kerning pairs
****************************************************************************/

KERNINFO::KERNINFO(CDC * dc)
    {
     count = dc->GetKerningPairs(0, NULL);
     if(count == 0)
        { /* no pairs */
         pairs = NULL;
        } /* no pairs */
     else
        { /* has pairs */
         pairs = new KERNINGPAIR[count];
         dc->GetKerningPairs(count, pairs);
        } /* has pairs */
    }

/****************************************************************************
*                             KERNINFO::~KERNINFO
* Effect: 
*       Frees the kerning pair info
****************************************************************************/

KERNINFO::~KERNINFO()
    {
     if(pairs != NULL)
        delete [] pairs;
    }

/****************************************************************************
*                             KERNINFO::findPair
* Inputs:
*       UINT first: First character of pair
*       UINT second: Second character of pair
* Result: LPKERNINGPAIR
*       Pointer to entry in array which matches the two characters, or NULL
*       if no kerning data found for pair
****************************************************************************/

LPKERNINGPAIR KERNINFO::findPair(UINT first, UINT second)
    {
     for(int i = 0; i < count; i++)
        { /* check each */
         if(pairs[i].wFirst != first)
           continue;
         if(pairs[i].wSecond != second)
            continue;
         return &pairs[i];
        } /* check each */
     return NULL;
    }
