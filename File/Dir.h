/**@file
 * @brief     Directory operations
 * @author    Igor Lesik 2010-2013
 * @copyright 2010-2013 Igor Lesik
 *
 */
#pragma once
#ifndef CGEM_DIR_H_INCLUDED
#define CGEM_DIR_H_INCLUDED

#include <cstdio>
#include <string>
#include <functional>
#include <stdexcept>

namespace gem {

class Dir
{
public:
    static const char DELIM[]; // = "/";

public:

    static
    bool scan(
        const std::string& dirname,
        std::function<void(const std::string&, const std::string&)> visitor,
        const std::string& filter = "",
        bool recursive = false);

};

} // end namespace gem

#endif
