/**@file
 * @brief     Logging
 * @author    Igor Lesik
 * @copyright 2013-2015 Igor Lesik
 *
 * Other interesting implementations:
 * - http://www.drdobbs.com/cpp/a-highly-configurable-logging-framework/225700666
 * - http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505
 * - Boost.Log
 */
#pragma once

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <chrono>
#include <mutex>

#include <ext/stdio_sync_filebuf.h> // __gnu_cxx::stdio_filebuf

namespace cgem { namespace Log {

/** Factory class that creates output stream depending on ctor signature.
 *
 */
class Stream
{
public:
    /// Created stream as result of calling ctor Stream
    std::ofstream stream;

public:
    /// Create stream to output to file
    Stream(const char* file_name);

    /// Create stream to output to stdout and stderr 
    Stream(int std_out = 0);

    /// Create stream using C file descriptor
    Stream(FILE* fd);

    /// No copy ctor
    Stream(const Stream&) = delete;

    /// No move ctor
    Stream(const Stream&&) = delete;

    /// Helper to output C string
    void write(const char* msg);

private:
    /// Helper to implement stream to FILE*
    __gnu_cxx::stdio_sync_filebuf<char> filebuf;
};

typedef std::chrono::high_resolution_clock Chronometer;

/** Sink is Logger output destination device.
 *
 */
class Sink
{
private:
    typedef std::list<Stream> StreamList;
    StreamList streams;

    std::size_t m_creation_time;

    //std::mutex m_mutex;

public:
    Sink();
    Sink(const Sink&) = delete;
    Sink(const Sink&&) = delete;
   ~Sink();

    static Sink* instance();

    void add_stream(const char* file_name) {
        streams.emplace_back(file_name);
    }

    void add_stream(const std::string& file_name) {
        streams.emplace_back(file_name.c_str());
    }

    void add_stdout_stream(int std_out = 0) {
        streams.emplace_back(std_out);
    }

    void add_stream(FILE* fd) {
        streams.emplace_back(fd);
    }

    std::size_t getCreationTime() const {return m_creation_time;}

    void write(const char* msg);

    void print(const char* frmt, ...);

    template <typename T>
    Sink& operator<<(const T& a)
    {

       //std::lock_guard<std::mutex> lock_guard(m_mutex);

       for (auto& stream : streams)
       {
           stream.stream << a; 
       }
       return *this;
    }

    /// Support manipulators like std::endl
    Sink& operator<< (std::ostream& (*pf)(std::ostream&))
    {
        for (auto& stream : streams)
        {
            stream.stream << pf; 
        }
        return *this;
    }

};

/** Logger implementation.
 *
 */
class Logger
{
private:
    Sink* m_sink; ///< output device
    Sink  m_null; ///< null output device to throw away verbose messages

    std::size_t m_max_level; ///< verbosity limit

    std::string m_name; ///< module/component name shown in header

public:
   explicit Logger(std::size_t max_level = 10);
   explicit Logger(Sink* sink, std::size_t max_level = 10);

   Logger(const Logger& master):
       m_sink(master.m_sink), m_max_level(master.m_max_level)
   {}

   /// Usage: log(DBG) << msg or log(DBG).print(...)
   Sink& operator()(std::size_t level);

   /// Set logger name shown in message header
   std::string setName(const std::string& new_name);

private:
   /// Print message header 
   void print_header(std::size_t level);
};

enum { ALWAYS=0, ERR=ALWAYS, NOTE, WARN=NOTE, INFO, DBG };

#define LOG_PRINT(logger, level, frmt, ...) \
    logger(level).print(frmt, ## __VA_ARGS__)

}} // cgem::Log
