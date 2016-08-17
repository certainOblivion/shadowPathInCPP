#pragma once 
#include <exception>
#include <string>
#include <sstream>
#include <iostream>

/// Exception type for assertion failures
class AssertCheck : public std::exception
{
private:
    const char* expression;
    const char* file;
    int line;
    std::string message;
    std::string report;

public:

    /// Helper class for formatting assertion message
    class StreamFormatter
    {
    private:

        std::ostringstream stream;

    public:

        operator std::string() const
        {
            return stream.str();
        }

        template<typename T>
        StreamFormatter& operator << (const T& value)
        {
            stream << value;
            return *this;
        }
    };

    /// Log error before throwing
    void LogError()
    {
#ifdef THROWASSERT_LOGGER
        THROWASSERT_LOGGER(report);
#else
        std::cerr << report << std::endl;
#endif
    }

    /// Construct an assertion failure exception
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
            outputStream << "Unreachable code assertion";
        }
        else
        {
            outputStream << "Assertion '" << expression << "'";
        }

        outputStream << " failed in file '" << file << "' line " << line;
        report = outputStream.str();

        LogError();
    }

    /// The assertion message
    virtual const char* what() const throw()
    {
        return report.c_str();
    }

    /// The expression which was asserted to be true
    const char* Expression() const throw()
    {
        return expression;
    }

    /// Source file
    const char* File() const throw()
    {
        return file;
    }

    /// Source line
    int Line() const throw()
    {
        return line;
    }

    /// Description of failure
    const char* Message() const throw()
    {
        return message.c_str();
    }

    ~AssertCheck() throw()
    {
    }
};


/// Assert that EXPRESSION evaluates to true, otherwise raise AssertionFailureException with associated MESSAGE (which may use C++ stream-style message formatting)
#if !RELEASE
#define throw_assert(EXPRESSION, MESSAGE) if(!(EXPRESSION)) { throw AssertCheck(#EXPRESSION, __FILE__, __LINE__, (AssertCheck::StreamFormatter() << MESSAGE)); }
#else
#define throw_assert(EXPRESSION, MESSAGE)
#endif // !defined(INCLUDED__THROW_ASSERT_HPP)

