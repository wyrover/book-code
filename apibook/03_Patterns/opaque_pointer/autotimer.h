/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
/**
 * \file   autotimer.h
 * \author Martin Reddy
 * \brief  An object to time the lifetime of a scope.
 *
 * Copyright (c) 2010, Martin Reddy. All rights reserved.
 * Distributed under the X11/MIT License. See LICENSE.txt.
 * See http://APIBook.com/ for the latest
 */

#ifndef AUTOTIMER_H
#define AUTOTIMER_H

/* declare an opaque pointer to an AutoTimer structure */
typedef struct AutoTimer *AutoTimerPtr;

/* create a new AutoTimer structure */
AutoTimerPtr AutoTimerCreate(const char *name);

/* destroy an existing AutoTimer structure */
void AutoTimerDestroy(AutoTimerPtr ptr);


#endif
