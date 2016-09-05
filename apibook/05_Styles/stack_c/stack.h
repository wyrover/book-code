/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
/**
 * \file   stack.h
 * \author Martin Reddy
 * \brief  An example of a plain C style API.
 *
 * Copyright (c) 2010, Martin Reddy. All rights reserved.
 * Distributed under the X11/MIT License. See LICENSE.txt.
 * See http://APIBook.com/ for the latest
 */

#ifndef STACK_H
#define STACK_H

/* An opaque pointer to a stack data structure */
typedef struct Stack *StackPtr;

/* Create a new stack data structure */
StackPtr StackCreate();
/* Destroy an existing stack data structure */
void StackDestroy(StackPtr stack);

/* Push a new value onto the stack */
void StackPush(StackPtr stack, int val);
/* Pop the last value from the stack */
int StackPop(StackPtr stack);
/* Return 1 if the stack contains no values */
int StackIsEmpty(const StackPtr stack);

#endif
