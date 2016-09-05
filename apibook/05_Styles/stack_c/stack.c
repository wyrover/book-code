/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
/**
 * \file   stack.c
 * \author Martin Reddy
 * \brief  An example of a plain C style API.
 *
 * Copyright (c) 2010, Martin Reddy. All rights reserved.
 * Distributed under the X11/MIT License. See LICENSE.txt.
 * See http://APIBook.com/ for the latest
 */

#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

struct Stack
{
	int *mStack;
	int mCurSize;
};

StackPtr StackCreate()
{
	return (StackPtr) calloc(sizeof(struct Stack), 1);
}

void StackDestroy(StackPtr stack)
{
	if (stack)
	{
		if (stack->mStack) {
			free(stack->mStack);
		}
		free(stack);
	}
}

void StackPush(StackPtr stack, int val)
{
	if (! stack)
		return;

	if (stack->mStack)
	{
		int newsize = stack->mCurSize + 1;
		int *newptr = realloc(stack->mStack, sizeof(struct Stack)*newsize);
		if (newptr)
		{
			stack->mStack = newptr;
			stack->mStack[newsize-1] = val;
			stack->mCurSize = newsize;
		}
	}
	else
	{
		stack->mStack = malloc(sizeof(struct Stack));
		if (stack->mStack)
		{
			stack->mStack[0] = val;
			stack->mCurSize = 1;
		}
	}
}

int StackPop(StackPtr stack)
{
	if (! StackIsEmpty(stack))
	{
		return stack->mStack[--stack->mCurSize];
	}
	return 0;
}

int StackIsEmpty(const StackPtr stack)
{
	return stack == NULL || stack->mCurSize == 0;
}
