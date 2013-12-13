#include "cgem/logging.h"


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

    log << logging::Level(0) << "output stream\n";// << std::endl;
    log << logging::Level(1) << "level 1\n";
    log << logging::Level(2) << "level 2" << std::endl;
    log << logging::Color::magenta();
    log << logging::Level(3) <<  "level 3\n";
    log << logging::Color::reset();
    log << logging::Level(4) << "level 4\n";

    return 0;
}
