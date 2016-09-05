/// -*- tab-width: 4; c-basic-offset: 4; indent-tabs-mode: t -*-
///
/// \file   cowptr_priv.h
/// \author Martin Reddy
/// \brief  A class template with copy-on-write behavior. 
///
/// Copyright (c) 2010, Martin Reddy. All rights reserved.
/// Distributed under the X11/MIT License. See LICENSE.txt.
/// See http://APIBook.com/ for the latest version.
///

#ifndef COWPTR_PRIV_H
#define COWPTR_PRIV_H

namespace apibook {

template <class T>
CowPtr<T>::CowPtr() : mPtr(0) {}

template <class T>
CowPtr<T>::~CowPtr() {}

template <class T>
inline CowPtr<T>::CowPtr(T *other) : mPtr(other) {}

template <class T>
inline CowPtr<T>::CowPtr(const CowPtr<T> &other) : mPtr(other.mPtr) {}

template <class T>
inline T &CowPtr<T>::operator*()
{
	Detach();
	return *mPtr.get();
}

template <class T>
inline const T &CowPtr<T>::operator*() const
{
	return *mPtr.get();
}

template <class T>
inline T *CowPtr<T>::operator->()
{
	Detach();
	return mPtr.get();
}

template <class T>
inline const T *CowPtr<T>::operator->() const
{
	return mPtr.get();
}

template <class T>
inline CowPtr<T>::operator T *()
{
	Detach();
	return mPtr.get();
}

template <class T>
inline CowPtr<T>::operator const T *() const
{
	return mPtr.get();
}

template <class T>
inline T *CowPtr<T>::data()
{
	Detach();
	return mPtr.get();
}

template <class T>
inline const T *CowPtr<T>::data() const
{
	return mPtr.get();
}

template <class T>
inline const T *CowPtr<T>::constData() const
{
	return mPtr.get();
}

template <class T>
inline bool CowPtr<T>::operator==(const CowPtr<T> &other) const
{
	return mPtr.get() == other.mPtr.get();
}

template <class T>
inline bool CowPtr<T>::operator!=(const CowPtr<T> &other) const
{
	return mPtr.get() != other.mPtr.get();
}

template <class T>
inline bool CowPtr<T>::operator!() const
{
	return !mPtr.get();
}

template <class T>
inline CowPtr<T> & CowPtr<T>::operator=(const CowPtr<T> &other)
{
	if (other.mPtr != mPtr)
	{
		mPtr = other.mPtr;
	}
	return *this;
}

template <class T>
inline CowPtr<T> &CowPtr<T>::operator=(T *other)
{
	mPtr = RefPtr(other);
	return *this;
}

template <class T>
inline void CowPtr<T>::Detach()
{
	T* temp = mPtr.get();
	if (temp && ! mPtr.unique())
	{
		mPtr = RefPtr(new T(*temp));
	}
}

}

#endif
