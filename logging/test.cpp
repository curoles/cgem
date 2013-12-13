#include "cgem/logging.h"

#include <chrono>

int main()
{
    enum { ERR=0, WRN, INFO, DBG };

    logging::Logger<3> log;

    log.add_stdout_stream();
    log.add_stream("test.log");
    log.add_stream("test1.log");

    log.print<ERR>("Hi\n");
    log.print<WRN>("%s world\n", "human");
    log.print<INFO>("I\n");
    log.print<DBG>("am #%d\n", 1);
    log.print<4>("here\n");

    log << logging::Level(0) << "output stream\n";
    log << logging::Level(1) << "level 1\n";
    log << logging::Level(2) << "level 2" << std::endl;
    log << logging::Color::magenta();
    log << logging::Level(3) <<  "level 3\n";
    log << logging::Color::reset();
    log << logging::Level(4) << "level 4\n";

    log(0) << "output stream\n";
    log(1) << "level 1\n";
    log(2) << "level 2" << std::endl;
    log(3) << logging::Color::blue();
    log(3) << "level 3\n";
    log(3) << logging::Color::reset();
    log(4) << "level 4\n";

#if 0
    logging::Logger<4> log1;
    log1.add_stream("chrono1.log");

    logging::Logger<4> log2;
    log2.add_stream("chrono2.log");

    auto start1 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000000; ++i)
    {
        log1.print<0>("print int %d\n", 7);
        log1.print<1>("print float %f\n", 7.7);
        log1.print<2>("print string %s\n", "I am string");
        log1.print<3>("print nothing\n");
    }
    auto end1 = std::chrono::high_resolution_clock::now();

    auto start2 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000000; ++i)
    {
        log2(0) << "print int " << 7 << "\n";
        log2(1) << "print float " << 7.7 << "\n";;
        log2(2) << "print string " << "I am string" << "\n";
        log2(3) << "print nothing" << "\n";
    }
    auto end2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed1 = end1 - start1;
    std::chrono::duration<double> elapsed2 = end2 - start2;

    log << logging::Level(0);
    log << "printf API: " << elapsed1.count() << "\n";
    log << "stream API: " << elapsed2.count() << "\n";
#endif

    return 0;
}
