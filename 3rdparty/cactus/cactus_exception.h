/*!
* \file cactus_exception.h
* \brief 异常类声明
*
* 异常类声明
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#ifndef __CACTUS_CACTUS_EXCEPTION_H__
#define __CACTUS_CACTUS_EXCEPTION_H__

#include <cactus/cactus.h>
#include <stdexcept>

namespace cactus
{
class CACTUS_API Exception: public std::exception
/// This is the base class for all exceptions defined
/// in the Poco class library.
{
public:
    Exception(const std::string& msg, int code = 0);
    /// Creates an exception.

    Exception(const std::string& msg, const std::string& arg, int code = 0);
    /// Creates an exception.

    Exception(const std::string& msg, const Exception& nested, int code = 0);
    /// Creates an exception and stores a clone
    /// of the nested exception.

    Exception(const Exception& exc);
    /// Copy constructor.

    ~Exception() throw();
    /// Destroys the exception and deletes the nested exception.

    Exception& operator = (const Exception& exc);
    /// Assignment operator.

    virtual const char* name() const throw();
    /// Returns a static string describing the exception.

    virtual const char* className() const throw();
    /// Returns the name of the exception class.

    virtual const char* what() const throw();
    /// Returns a static string describing the exception.
    ///
    /// Same as name(), but for compatibility with std::exception.

    const Exception* nested() const;
    /// Returns a pointer to the nested exception, or
    /// null if no nested exception exists.

    const std::string& message() const;
    /// Returns the message text.

    int code() const;
    /// Returns the exception code if defined.

    std::string displayText() const;
    /// Returns a string consisting of the
    /// message name and the message text.

    virtual Exception* clone() const;
    /// Creates an exact copy of the exception.
    ///
    /// The copy can later be thrown again by
    /// invoking rethrow() on it.

    virtual void rethrow() const;
    /// (Re)Throws the exception.
    ///
    /// This is useful for temporarily storing a
    /// copy of an exception (see clone()), then
    /// throwing it again.

protected:
    Exception(int code = 0);
    /// Standard constructor.

    void message(const std::string& msg);
    /// Sets the message for the exception.

    void extendedMessage(const std::string& arg);
    /// Sets the extended message for the exception.

private:
    std::string _msg;
    Exception*  _pNested;
    int         _code;
};


//
// inlines
//
inline const Exception* Exception::nested() const
{
    return _pNested;
}


inline const std::string& Exception::message() const
{
    return _msg;
}


inline void Exception::message(const std::string& msg)
{
    _msg = msg;
}


inline int Exception::code() const
{
    return _code;
}


//
// Macros for quickly declaring and implementing exception classes.
// Unfortunately, we cannot use a template here because character
// pointers (which we need for specifying the exception name)
// are not allowed as template arguments.
//
#define CACTUS_DECLARE_EXCEPTION_CODE(API, CLS, BASE, CODE) \
    class API CLS: public BASE                                                      \
    {                                                                               \
    public:                                                                         \
        CLS(int code = CODE);                                                       \
        CLS(const std::string& msg, int code = CODE);                               \
        CLS(const std::string& msg, const std::string& arg, int code = CODE);       \
        CLS(const std::string& msg, const cactus::Exception& exc, int code = CODE); \
        CLS(const CLS& exc);                                                        \
        ~CLS() throw();                                                             \
        CLS& operator = (const CLS& exc);                                           \
        const char* name() const throw();                                           \
        const char* className() const throw();                                      \
        cactus::Exception* clone() const;                                           \
        void rethrow() const;                                                       \
    };

#define CACTUS_DECLARE_EXCEPTION(API, CLS, BASE) \
    CACTUS_DECLARE_EXCEPTION_CODE(API, CLS, BASE, 0)

#define CACTUS_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)                                                 \
    CLS::CLS(int code): BASE(code)                                                                  \
    {                                                                                               \
    }                                                                                               \
    CLS::CLS(const std::string& msg, int code): BASE(msg, code)                                     \
    {                                                                                               \
    }                                                                                               \
    CLS::CLS(const std::string& msg, const std::string& arg, int code): BASE(msg, arg, code)        \
    {                                                                                               \
    }                                                                                               \
    CLS::CLS(const std::string& msg, const cactus::Exception& exc, int code): BASE(msg, exc, code)  \
    {                                                                                               \
    }                                                                                               \
    CLS::CLS(const CLS& exc): BASE(exc)                                                             \
    {                                                                                               \
    }                                                                                               \
    CLS::~CLS() throw()                                                                             \
    {                                                                                               \
    }                                                                                               \
    CLS& CLS::operator = (const CLS& exc)                                                           \
    {                                                                                               \
        BASE::operator = (exc);                                                                     \
        return *this;                                                                               \
    }                                                                                               \
    const char* CLS::name() const throw()                                                           \
    {                                                                                               \
        return NAME;                                                                                \
    }                                                                                               \
    const char* CLS::className() const throw()                                                      \
    {                                                                                               \
        return typeid(*this).name();                                                                \
    }                                                                                               \
    cactus::Exception* CLS::clone() const                                                               \
    {                                                                                               \
        return new CLS(*this);                                                                      \
    }                                                                                               \
    void CLS::rethrow() const                                                                       \
    {                                                                                               \
        throw *this;                                                                                \
    }


//
// Standard exception classes
//
CACTUS_DECLARE_EXCEPTION(CACTUS_API, LogicException, Exception)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, AssertionViolationException, LogicException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, NullPointerException, LogicException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, NullValueException, LogicException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, BugcheckException, LogicException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, InvalidArgumentException, LogicException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, NotImplementedException, LogicException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, RangeException, LogicException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, IllegalStateException, LogicException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, InvalidAccessException, LogicException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, SignalException, LogicException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, UnhandledException, LogicException)

CACTUS_DECLARE_EXCEPTION(CACTUS_API, RuntimeException, Exception)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, NotFoundException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, ExistsException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, TimeoutException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, SystemException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, RegularExpressionException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, LibraryLoadException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, LibraryAlreadyLoadedException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, NoThreadAvailableException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, PropertyNotSupportedException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, PoolOverflowException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, NoPermissionException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, OutOfMemoryException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, DataException, RuntimeException)

CACTUS_DECLARE_EXCEPTION(CACTUS_API, DataFormatException, DataException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, SyntaxException, DataException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, CircularReferenceException, DataException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, PathSyntaxException, SyntaxException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, IOException, RuntimeException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, ProtocolException, IOException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, FileException, IOException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, FileExistsException, FileException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, FileNotFoundException, FileException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, PathNotFoundException, FileException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, FileReadOnlyException, FileException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, FileAccessDeniedException, FileException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, CreateFileException, FileException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, OpenFileException, FileException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, WriteFileException, FileException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, ReadFileException, FileException)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, UnknownURISchemeException, RuntimeException)

CACTUS_DECLARE_EXCEPTION(CACTUS_API, ApplicationException, Exception)
CACTUS_DECLARE_EXCEPTION(CACTUS_API, BadCastException, RuntimeException)

CACTUS_DECLARE_EXCEPTION(CACTUS_API, DLLLoadException, Exception)

}


#endif // __CACTUS_CACTUS_EXCEPTION_H__
