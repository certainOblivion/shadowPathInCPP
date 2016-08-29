#pragma once 
#include <exception>
#include <string>
#include <sstream>
#include <iostream>

// Exception type for assertion failures
class AssertCheck : public std::exception
{
private:
    const char* expression;
    const char* file;
    int line;
    std::string message;
    std::string report;

public:

    // Log error before throwing
    void LogError()
    {
#ifdef ASSERT_LOGGER
        ASSERT_LOGGER(this, report);
#else
        std::cerr << report << std::endl;
#endif
    }

    // Construct an assertion failure exception
    AssertCheck(const char* expression, const char* file, int line, const std::string& message)
        : expression(expression)
        , file(file)
        , line(line)
        , message(message)
    {
        std::ostringstream outputStream;

        if (!message.empty())
        {
            outputStream << message << ": ";
        }

        std::string expressionString = expression;

        if (expressionString == "false" || expressionString == "0" || expressionString == "FALSE")
        {
            outputStream << "Fail Notification";
        }
        else
        {
            outputStream << "'" << expression << "' is false.";
        }

        outputStream << " Failed in file '" << file << "' line " << line;
        report = outputStream.str();

        LogError();
    }

    // The assertion message
    virtual const char* what() const
    {
        return report.c_str();
    }

    // The expression which was asserted to be true
    const char* Expression() const
    {
        return expression;
    }

    // Source file
    const char* File() const
    {
        return file;
    }

    // Source line
    int Line() const
    {
        return line;
    }

    // Description of failure
    const char* Message() const
    {
        return message.c_str();
    }

    ~AssertCheck() throw()
    {
    }
};


// Assert that EXPRESSION evaluates to true, otherwise raise AssertionFailureException with associated MESSAGE (which may use C-style string formatting with variadic parameters)
#if !RELEASE
#define throw_assert(EXPRESSION, MESSAGE, ...) if(!(EXPRESSION)) \
{ \
    const size_t size = snprintf(NULL,0, MESSAGE, __VA_ARGS__);\
    std::vector<char> __buffer;\
    __buffer.resize(size+1);\
    sprintf_s(&(__buffer[0]),size+1, MESSAGE, __VA_ARGS__);\
    throw AssertCheck(#EXPRESSION, __FILE__, __LINE__, &(__buffer[0])); \
}

#define notify_fail(MESSAGE, ...) throw_assert(false, MESSAGE, __VA_ARGS__);
#else
#define throw_assert(EXPRESSION, MESSAGE, ...)
#define notify_fail(MESSAGE, ...)
#endif // !defined(INCLUDED__THROW_ASSERT_HPP)

