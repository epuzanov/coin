#ifndef CC_MUTEXP_H
#define CC_MUTEXP_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.  See the file LICENSE.GPL
 *  at the root directory of this source distribution for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  www.sim.no, support@sim.no, Voice: +47 22114160, Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_INTERNAL
#error You have tried to use one of the private Coin header files
#endif /* ! COIN_INTERNAL */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

#define NO_IMPLEMENTATION
#ifdef USE_PTHREAD
#include <pthread.h>
#undef NO_IMPLEMENTATION
#endif /* USE_PTHREAD */

#ifdef USE_W32THREAD
#include <windows.h>
typedef BOOL cc_mutex_try_enter_critical_section_func(LPCRITICAL_SECTION);
cc_mutex_try_enter_critical_section_func * cc_mutex_try_enter_critical_section;
#undef NO_IMPLEMENTATION
#endif /* USE_W32THREAD */

struct cc_mutex {
#ifdef USE_PTHREAD
  struct cc_pthread_mutex_data {
    pthread_mutex_t mutexid;
  } pthread;
#endif /* USE_PTHREAD */
#ifdef USE_W32THREAD
  union cc_w32thread_mutex_data {
    HANDLE mutexhandle;
    CRITICAL_SECTION critical_section;
  }
#endif /* USE_W32THREAD */
};

#ifdef NO_IMPLEMENTATION
#error missing threads implementation support
#endif /* ! NO_IMPLEMENTATION */

/* ********************************************************************** */

void cc_mutex_init(void);
void cc_mutex_global_lock(void);
void cc_mutex_global_unlock(void);
void cc_mutex_struct_init(cc_mutex * mutex_struct);
void cc_mutex_struct_clean(cc_mutex * mutex_struct);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_MUTEXP_H */
