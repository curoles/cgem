/**@file
 * @brief     Logging
 * @author    Igor Lesik
 * @copyright 2013 Igor Lesik
 *
 * Other interesting implementations:
 * - http://www.drdobbs.com/cpp/a-highly-configurable-logging-framework/225700666
 * - http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505
 */

#include <cstring>

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <memory>
#include <cstdarg>

namespace logging {

typedef std::size_t level_type;

struct Level { level_type level; Level(level_type l):level(l){} };


class Stream
{
public:
    std::ofstream stream;

public:
    Stream(const char* file_name):stream(file_name, std::ofstream::out) {
    }

    /// Create Stream that outputs to stdout and stderr 
    Stream(int std_out = 0) {
        stream.std::ios::rdbuf( (std_out == 0)?
            std::cout.std::ios::rdbuf() : std::cerr.std::ios::rdbuf());
    }

    Stream(const Stream&) /*= delete;*/ {}

    void print(const char* msg);
};

template<std::size_t DBG_LEVEL>
class Logger
{
public:
    typedef std::list<Stream> StreamList;

private:
    StreamList streams;

    level_type level;
    level_type current_stream_level;

public:
    Logger(const Logger&) = delete;

    Logger(level_type level=DBG_LEVEL):level(level), current_stream_level(0)
    {
    }

    bool add_stream(const char* file_name) {
        streams.emplace_back(file_name);
        return true;
    }

    bool add_stdout_stream(int std_out = 0) {
        streams.emplace_back(std_out);
        return true;
    }

    template<std::size_t LEVEL>
    void print(const char* frmt, ...)
    {
        if (LEVEL <= DBG_LEVEL)
        {
            if (LEVEL <= level)
            {
                char buf[128];
                va_list args;
                va_start (args, frmt);
                vsnprintf(buf, sizeof(buf), frmt, args);

                for (auto& stream : streams)
                {
                    stream.print(buf);
                }

                va_end(args);
            }
        }
    }

    template <typename T>
    Logger& operator<<(const T& a)
    {
        if (current_stream_level <= DBG_LEVEL and current_stream_level <= level)
        {
            for (auto& stream : streams)
            {
                stream.stream << a; 
            }
        }
        return *this;
    }

    /// Support manipulators like std::endl
    Logger& operator<< (std::ostream& (*pf)(std::ostream&))
    {
        for (auto& stream : streams)
        {
            stream.stream << pf; 
        }

        return *this;
    }

    Logger& operator<<(const Level& level)
    {
        current_stream_level = level.level;
        return *this;
    }

    Logger& operator()(level_type level)
    {
        current_stream_level = level;
        return *this;
    }

};

struct Color
{
    short clr;
    enum { RESET = 0, BLUE = 34, MAGENTA = 35 };

    Color(short c = RESET):clr(c){}

    std::string operator()() {
        return std::string("\033[") + std::to_string(clr) + "m";
    }

    static Color reset;
    static Color magenta;
    static Color blue;
};

} // end logging


