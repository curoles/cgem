#include "logging.h"

using namespace logging;

Color Color::reset(Color::RESET);
Color Color::blue(Color::BLUE);
Color Color::magenta(Color::MAGENTA);

void
logging::Stream::
print(const char* msg)
{
    stream << msg;
}


