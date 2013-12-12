#include <cstring>

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <memory>

namespace logging {

typedef std::size_t level_type;

struct Level { level_type level; Level(level_type l):level(l){} };

class Stream
{
private:
    std::ofstream stream;

public:
    Stream(const char* file_name):stream(file_name, std::ofstream::out) {
    }

    void print(std::size_t level, const char* msg);
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
    Logger(level_type level=DBG_LEVEL):level(level), current_stream_level(level)
    {
    }

    bool add_stream(const char* file_name) {
        //gem::File f;
        //if (not f.open(file_name, "w")) return false;
        streams.emplace_back(file_name);
        return true;
    }

    template<std::size_t LEVEL>
    void print(const char* msg)
    {
        if (LEVEL <= DBG_LEVEL)
        {
            if (LEVEL <= level)
            {
                for (auto& stream : streams)
                {
                    stream.print(LEVEL, msg);
                }
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
                std::cout << a;//TODO stream << a; 
            }

        }
        return *this;
    }

    Logger& operator<<(const Level& level)
    {
        current_stream_level = level.level;
        return *this;
    }


};


} // end logging


void
logging::Stream::
print(std::size_t level, const char* msg)
{
    //printf("%lu %s", level, msg);
    stream << level << " " << msg;
}

int main()
{
    enum { ERR=0, WRN, INFO, DBG };

    logging::Logger<3> log;

    log.add_stream("test.log");
    log.add_stream("test1.log");

    log.print<ERR>("Hi\n");
    log.print<WRN>("world\n");
    log.print<INFO>("I\n");
    log.print<DBG>("am\n");
    log.print<4>("here\n");

    log << logging::Level(0) << "output stream\n";// << std::endl;
    log << logging::Level(1) << "level 1\n";
    log << logging::Level(2) << "level 2\n";
    log << logging::Level(3) << "level 3\n";
    log << logging::Level(4) << "level 4\n";

    return 0;
}
