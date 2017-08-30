#include <cactus/cactus_exception.h>
#include <typeinfo>

namespace cactus
{
Exception::Exception(int code): _pNested(0), _code(code)
{
}


Exception::Exception(const std::string& msg, int code): _msg(msg), _pNested(0), _code(code)
{
}


Exception::Exception(const std::string& msg, const std::string& arg, int code): _msg(msg), _pNested(0), _code(code)
{
    if (!arg.empty()) {
        _msg.append(": ");
        _msg.append(arg);
    }
}


Exception::Exception(const std::string& msg, const Exception& nested, int code): _msg(msg), _pNested(nested.clone()), _code(code)
{
}


Exception::Exception(const Exception& exc):
    std::exception(exc),
    _msg(exc._msg),
    _code(exc._code)
{
    _pNested = exc._pNested ? exc._pNested->clone() : 0;
}


Exception::~Exception() throw()
{
    delete _pNested;
}


Exception& Exception::operator = (const Exception& exc)
{
    if (&exc != this) {
        delete _pNested;
        _msg     = exc._msg;
        _pNested = exc._pNested ? exc._pNested->clone() : 0;
        _code    = exc._code;
    }

    return *this;
}


const char* Exception::name() const throw()
{
    return "Exception";
}


const char* Exception::className() const throw()
{
    return typeid(*this).name();
}


const char* Exception::what() const throw()
{
    return name();
}


std::string Exception::displayText() const
{
    std::string txt = name();

    if (!_msg.empty()) {
        txt.append(": ");
        txt.append(_msg);
    }

    return txt;
}


void Exception::extendedMessage(const std::string& arg)
{
    if (!arg.empty()) {
        if (!_msg.empty()) _msg.append(": ");

        _msg.append(arg);
    }
}


Exception* Exception::clone() const
{
    return new Exception(*this);
}


void Exception::rethrow() const
{
    throw *this;
}


CACTUS_IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception")
CACTUS_IMPLEMENT_EXCEPTION(AssertionViolationException, LogicException, "Assertion violation")
CACTUS_IMPLEMENT_EXCEPTION(NullPointerException, LogicException, "Null pointer")
CACTUS_IMPLEMENT_EXCEPTION(NullValueException, LogicException, "Null value")
CACTUS_IMPLEMENT_EXCEPTION(BugcheckException, LogicException, "Bugcheck")
CACTUS_IMPLEMENT_EXCEPTION(InvalidArgumentException, LogicException, "Invalid argument")
CACTUS_IMPLEMENT_EXCEPTION(NotImplementedException, LogicException, "Not implemented")
CACTUS_IMPLEMENT_EXCEPTION(RangeException, LogicException, "Out of range")
CACTUS_IMPLEMENT_EXCEPTION(IllegalStateException, LogicException, "Illegal state")
CACTUS_IMPLEMENT_EXCEPTION(InvalidAccessException, LogicException, "Invalid access")
CACTUS_IMPLEMENT_EXCEPTION(SignalException, LogicException, "Signal received")
CACTUS_IMPLEMENT_EXCEPTION(UnhandledException, LogicException, "Unhandled exception")

CACTUS_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
CACTUS_IMPLEMENT_EXCEPTION(NotFoundException, RuntimeException, "Not found")
CACTUS_IMPLEMENT_EXCEPTION(ExistsException, RuntimeException, "Exists")
CACTUS_IMPLEMENT_EXCEPTION(TimeoutException, RuntimeException, "Timeout")
CACTUS_IMPLEMENT_EXCEPTION(SystemException, RuntimeException, "System exception")
CACTUS_IMPLEMENT_EXCEPTION(RegularExpressionException, RuntimeException, "Error in regular expression")
CACTUS_IMPLEMENT_EXCEPTION(LibraryLoadException, RuntimeException, "Cannot load library")
CACTUS_IMPLEMENT_EXCEPTION(LibraryAlreadyLoadedException, RuntimeException, "Library already loaded")
CACTUS_IMPLEMENT_EXCEPTION(NoThreadAvailableException, RuntimeException, "No thread available")
CACTUS_IMPLEMENT_EXCEPTION(PropertyNotSupportedException, RuntimeException, "Property not supported")
CACTUS_IMPLEMENT_EXCEPTION(PoolOverflowException, RuntimeException, "Pool overflow")
CACTUS_IMPLEMENT_EXCEPTION(NoPermissionException, RuntimeException, "No permission")
CACTUS_IMPLEMENT_EXCEPTION(OutOfMemoryException, RuntimeException, "Out of memory")
CACTUS_IMPLEMENT_EXCEPTION(DataException, RuntimeException, "Data error")

CACTUS_IMPLEMENT_EXCEPTION(DataFormatException, DataException, "Bad data format")
CACTUS_IMPLEMENT_EXCEPTION(SyntaxException, DataException, "Syntax error")
CACTUS_IMPLEMENT_EXCEPTION(CircularReferenceException, DataException, "Circular reference")
CACTUS_IMPLEMENT_EXCEPTION(PathSyntaxException, SyntaxException, "Bad path syntax")
CACTUS_IMPLEMENT_EXCEPTION(IOException, RuntimeException, "I/O error")
CACTUS_IMPLEMENT_EXCEPTION(ProtocolException, IOException, "Protocol error")
CACTUS_IMPLEMENT_EXCEPTION(FileException, IOException, "File access error")
CACTUS_IMPLEMENT_EXCEPTION(FileExistsException, FileException, "File exists")
CACTUS_IMPLEMENT_EXCEPTION(FileNotFoundException, FileException, "File not found")
CACTUS_IMPLEMENT_EXCEPTION(PathNotFoundException, FileException, "Path not found")
CACTUS_IMPLEMENT_EXCEPTION(FileReadOnlyException, FileException, "File is read-only")
CACTUS_IMPLEMENT_EXCEPTION(FileAccessDeniedException, FileException, "Access to file denied")
CACTUS_IMPLEMENT_EXCEPTION(CreateFileException, FileException, "Cannot create file")
CACTUS_IMPLEMENT_EXCEPTION(OpenFileException, FileException, "Cannot open file")
CACTUS_IMPLEMENT_EXCEPTION(WriteFileException, FileException, "Cannot write file")
CACTUS_IMPLEMENT_EXCEPTION(ReadFileException, FileException, "Cannot read file")
CACTUS_IMPLEMENT_EXCEPTION(UnknownURISchemeException, RuntimeException, "Unknown URI scheme")


CACTUS_IMPLEMENT_EXCEPTION(ApplicationException, Exception, "Application exception")
CACTUS_IMPLEMENT_EXCEPTION(BadCastException, RuntimeException, "Bad cast exception")

CACTUS_IMPLEMENT_EXCEPTION(DLLLoadException, Exception, "Dll Load exception")
} // namespace cactus