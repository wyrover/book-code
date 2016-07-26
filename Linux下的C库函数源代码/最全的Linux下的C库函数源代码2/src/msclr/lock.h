/***
*lock.h
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:   lock class for automatically acquiring/releasing
*           the monitor lock on managed types.
*
*       [Public]
*
****/

#pragma once

#if !defined (_INC_MSCLR_LOCK)

#ifndef __cplusplus_cli
#error ERROR: msclr libraries are not compatible with /clr:oldSyntax
#endif  /* __cplusplus_cli */

#using <mscorlib.dll>
#include <msclr\safebool.h>
#if !defined (_M_CEE_SAFE)
#include <vcclr.h>
#endif  /* !defined (_M_CEE_SAFE) */

namespace msclr
{

    // The lock_when enumeration is used to defer the lock object
    // from taking the lock.
    enum lock_when { lock_later };

    ref class lock
    {
    private:
        System::Object ^ m_object;
        bool             m_locked;

        template<class T,class U> value struct is_not { typedef int __dont_use_this_type__; };
        template<class T> value struct is_not<T,T> { };

    public:
        // By default, take the lock immediately with an infinite timeout.
        // Accept an optional timespan (in milliseconds) and throw on failure or timeout.
        template<class T> lock( T ^ _object)
            : m_object( _object ),
              m_locked( false )
        {
            // ensure that T is not a ReaderWriterLock.
            is_not<T, System::Threading::ReaderWriterLock>::__dont_use_this_type__;

            acquire(System::Threading::Timeout::Infinite);
        }

        template<class T> lock( T ^ _object, int _timeout )
            : m_object( _object ),
              m_locked( false )
        {
            // ensure that T is not a ReaderWriterLock.
            is_not<T, System::Threading::ReaderWriterLock>::__dont_use_this_type__;

            acquire( _timeout );
        }

        // By default, take the lock immediately within the specified timespan
        // and throw on failure or timeout.
        template<class T> lock( T ^ _object, System::TimeSpan _timeout )
            : m_object( _object ),
              m_locked( false )
        {
            // ensure that T is not a ReaderWriterLock.
            is_not<T, System::Threading::ReaderWriterLock>::__dont_use_this_type__;

            acquire( _timeout );
        }

        // If you use the "lock_later" enumeration, it causes lock
        // to defer taking the lock.
        template<class T> lock( T ^ _object, lock_when )
            : m_object( _object ),
              m_locked( false )
        {
            // ensure that T is not a ReaderWriterLock.
            is_not<T, System::Threading::ReaderWriterLock>::__dont_use_this_type__;
        }

#if !defined (_M_CEE_SAFE)
        template<class T> lock( gcroot<T ^> _object)
            : m_object( _object ),
              m_locked( false )
        {
            // ensure that T is not a ReaderWriterLock.
            is_not<T, System::Threading::ReaderWriterLock>::__dont_use_this_type__;

            acquire(System::Threading::Timeout::Infinite);
        }


        template<class T> lock( gcroot<T ^> _object, int _timeout )
            : m_object( _object ),
              m_locked( false )
        {
            // ensure that T is not a ReaderWriterLock.
            is_not<T, System::Threading::ReaderWriterLock>::__dont_use_this_type__;

            acquire( _timeout );
        }

        // By default, take the lock immediately within the specified timespan
        // and throw on failure or timeout.
        template<class T> lock( gcroot<T ^> _object, System::TimeSpan _timeout )
            : m_object( _object ),
              m_locked( false )
        {
            // ensure that T is not a ReaderWriterLock.
            is_not<T, System::Threading::ReaderWriterLock>::__dont_use_this_type__;

            acquire( _timeout );
        }

        // If you use the "lock_later" enumeration, it causes lock
        // to defer taking the lock.
        template<class T> lock( gcroot<T ^> _object, lock_when )
            : m_object( _object ),
              m_locked( false )
        {
            // ensure that T is not a ReaderWriterLock.
            is_not<T, System::Threading::ReaderWriterLock>::__dont_use_this_type__;
        }
#endif  /* !defined (_M_CEE_SAFE) */

        // release the lock if it is not currently held
        ~lock()
        {
            release();
        }

        // Check to see if this lock object is currently holding the lock
        bool is_locked()
        {
            return m_locked;
        }

        // Check to see if this lock object is currently holding the lock
        operator _detail_class::_safe_bool()
        {
            return is_locked() ? _detail_class::_safe_true : _detail_class::_safe_false;
        }

        // disallow explicit comparisons to _safe_bool
        template<class T> bool operator==( T t )
        {
            // ensure that T is not a _safe_bool.
            is_not<T, _detail_class::_safe_bool>::__dont_use_this_type__;

            return m_object == t;
        }

        template<class T> bool operator!=( T t )
        {
            // ensure that T is not a _safe_bool.
            is_not<T, _detail_class::_safe_bool>::__dont_use_this_type__;

            return m_object != t;
        }

        // Take the lock within the specified timespan, or with an infinite
        // timespan if none is specified. Throw an ApplicationException on timeout.
        void acquire( int _timeout )
        {
            if( ! m_locked )
            {
                if( ! System::Threading::Monitor::TryEnter( m_object, _timeout ) )
                {
                    throw gcnew System::ApplicationException( "lock timeout" );
                }
                m_locked = true;
            }
        }

        void acquire()
        {
            if( ! m_locked )
            {
                if( ! System::Threading::Monitor::TryEnter( m_object,
                          System::Threading::Timeout::Infinite ) )
                {
                    throw gcnew System::ApplicationException( "lock timeout" );
                }
                m_locked = true;
            }
        }

        // Take the lock within the specified timespan.
        // Throw an ApplicationException on timeout.
        void acquire( System::TimeSpan _timeout )
        {
            if( ! m_locked )
            {
                if( ! System::Threading::Monitor::TryEnter( m_object, _timeout ) )
                {
                    throw gcnew System::ApplicationException( "lock timeout" );
                }
                m_locked = true;
            }
        }

        // Try to take the lock within the specified timespan. In the case
        // of timeout, return false.
        bool try_acquire( int _timeout ) // throw()
        {
            if( ! m_locked )
            {
                if( ! System::Threading::Monitor::TryEnter( m_object, _timeout ) )
                {
                    return false;
                }
                m_locked = true;
            }
            return true;
        }

        // Try to take the lock within the specified timespan. In the case
        // of timeout, return false.
        bool try_acquire( System::TimeSpan _timeout ) // throw()
        {
            if( ! m_locked )
            {
                if( ! System::Threading::Monitor::TryEnter( m_object, _timeout ) )
                {
                    return false;
                }
                m_locked = true;
            }
            return true;
        }

        // Release the lock if it is currently held
        void release()
        {
            if( m_locked )
            {
                m_locked = false;
                System::Threading::Monitor::Exit( m_object );
            }
        }
    };
}

#define _INC_MSCLR_LOCK

#endif  /* !defined (_INC_MSCLR_LOCK) */
