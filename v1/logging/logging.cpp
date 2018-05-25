/**@file
 * @brief     Logging
 * @author    Igor Lesik
 * @copyright 2013-2015 Igor Lesik
 *
 */
#include "logging.h"

#include <cstdarg>
#include <cassert>

using namespace cgem::Log;

/// Create stream to output to file
Stream::Stream(const char* file_name):
    stream(file_name, std::ofstream::out),
    filebuf(nullptr)
{
}

/// Create stream to output to stdout and stderr 
Stream::Stream(int std_out):
    filebuf(nullptr)
{
    stream.std::ios::rdbuf( (std_out == 0)?
        std::cout.std::ios::rdbuf() : std::cerr.std::ios::rdbuf());
}

Stream::Stream(FILE* fd) : filebuf(fd)
{
    stream.std::ios::rdbuf(&filebuf);
}

void
Stream::write(const char* msg)
{
    stream << msg;
}

static Sink static_sink;

/*static*/ Sink* Sink::instance() { return &static_sink; }

Sink::Sink():
    m_creation_time(Chronometer::now().time_since_epoch().count())
{
}

Sink::~Sink()
{
}

void
Sink::write(const char* msg)
{
    //std::lock_guard<std::mutex> lock_guard(m_mutex);

    for (auto& stream : streams)
    {
        stream.write(msg);
    }
}

void
Sink::print(const char* frmt, ...)
{
    char buf[256];

    va_list args;
    va_start (args, frmt);
    vsnprintf(buf, sizeof(buf), frmt, args);

    buf[sizeof(buf)-1] = '\0';

    write(buf);

    va_end(args);
}

Logger::Logger(std::size_t max_level):
    Logger(Sink::instance(), max_level)
{
}

Logger::Logger(Sink* sink, std::size_t max_level):
    m_sink(sink),
    m_max_level(max_level)
{
    assert(m_sink != nullptr);
}

Sink&
Logger::operator()(std::size_t level)
{
    if (level > m_max_level) {
        return m_null;
    }

    print_header(level);

    return *m_sink;
}

std::string
Logger::setName(const std::string& new_name)
{
    std::string old_name = m_name;
    m_name = new_name;
    return old_name;
}

void
Logger::print_header(std::size_t level)
{
    char buf[128];
    std::size_t time = Chronometer::now().time_since_epoch().count() - m_sink->getCreationTime();

    double secs = ((double)time * Chronometer::period::num) / Chronometer::period::den;

    snprintf(buf, sizeof(buf), "%01lu %10.5f %16s -- ", level, secs, m_name.c_str());

    if (0/*m_use_color*/) {
        const char* clr = (level==0)? "\033[31m":((level==1)? "\033[33m":((level==2)? "\033[34m":"\033[37m"));
        (*m_sink) << clr << buf << "\033[0m";
    }
    else {
        m_sink->write(buf);
    }

}

