/***
*typeinfo.h - Defines the type_info structure and exceptions used for RTTI
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the type_info structure and exceptions used for
*       Runtime Type Identification.
*
*       [Public]
*
****/

#if _MSC_VER > 1000
#pragma once
#endif  /* _MSC_VER > 1000 */

#include <crtdefs.h>

#ifndef _INC_TYPEINFO
#define _INC_TYPEINFO

#ifdef _MSC_VER
#pragma pack(push,_CRT_PACKING)
#endif  /* _MSC_VER */

#ifndef RC_INVOKED

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif  /* __cplusplus */

#ifndef _CRTBLD
/* This version of the header files is NOT for user programs.
 * It is intended for use when building the C runtimes ONLY.
 * The version intended for public use will not have this message.
 */
#error ERROR: Use of C runtime library internal header file.
#endif  /* _CRTBLD */

#ifndef _SYSCRT
#include <typeinfo>

#ifndef _TICORE

#ifdef __RTTI_OLDNAMES
/* Some synonyms for folks using older standard */
using std::bad_cast;
using std::bad_typeid;

typedef type_info Type_info;
typedef bad_cast Bad_cast;
typedef bad_typeid Bad_typeid;
#endif  /* __RTTI_OLDNAMES */

#endif  /* _TICORE */

#else  /* _SYSCRT */

#ifndef _SYSCRT
struct __type_info_node {
    void *memPtr;
    __type_info_node* next;
};

extern __type_info_node __type_info_root_node;
#endif  /* _SYSCRT */

class type_info {
public:
    _CRTIMP virtual __thiscall ~type_info();
    _CRTIMP int __thiscall operator==(__in const type_info& _Rhs) const;
    _CRTIMP int __thiscall operator!=(__in const type_info& _Rhs) const;
    _CRTIMP int __thiscall before(__in const type_info& _Rhs) const;
#ifdef _SYSCRT
    _CRTIMP __checkReturn const char* __thiscall name() const;
#else  /* _SYSCRT */
    _CRTIMP __checkReturn const char* __thiscall name(__inout __type_info_node* __ptype_info_node = &__type_info_root_node) const;
#endif  /* _SYSCRT */
    _CRTIMP __checkReturn const char* __thiscall raw_name() const;
private:
    void *_m_data;
    char _m_d_name[1];
    __thiscall type_info(__in const type_info& _Rhs);
    type_info& __thiscall operator=(__in const type_info& _Rhs);
};
#ifndef _TICORE

/* This include must occur below the definition of class type_info */
#include <stdexcpt.h>

class _CRTIMP bad_cast : public std::exception {
public:
    __CLR_OR_THIS_CALL bad_cast(__in_z const char * _Message = "bad cast");
    __CLR_OR_THIS_CALL bad_cast(__in const bad_cast & _Bad_cast);
    virtual __CLR_OR_THIS_CALL ~bad_cast();
#ifndef _INTERNAL_IFSTRIP_
#ifdef CRTDLL
private:
    /* This is aliased to public:bad_cast(const char * const &) to provide */
    /* the old, non-conformant constructor. */
    bad_cast(__in_z const char * const * _Message);
#endif  /* CRTDLL */
#endif  /* _INTERNAL_IFSTRIP_ */
};

class _CRTIMP bad_typeid : public std::exception {
public:
    bad_typeid(__in_z const char * _Message = "bad typeid");
    bad_typeid(__in const bad_typeid &);
    virtual ~bad_typeid();
};

class _CRTIMP __non_rtti_object : public bad_typeid {
public:
    __non_rtti_object(__in_z const char * _Message);
    __non_rtti_object(__in const __non_rtti_object &);
    virtual ~__non_rtti_object();
};

#ifdef __RTTI_OLDNAMES
/* Some synonyms for folks using older standard */
typedef type_info Type_info;
typedef bad_cast Bad_cast;
typedef bad_typeid Bad_typeid;
#endif  /* __RTTI_OLDNAMES */

#endif  /* _TICORE */

#endif  /* _SYSCRT */

#endif  /* RC_INVOKED */

#ifdef _MSC_VER
#pragma pack(pop)
#endif  /* _MSC_VER */

#endif  /* _INC_TYPEINFO */
