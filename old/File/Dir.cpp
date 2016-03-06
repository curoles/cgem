#include "Dir.h"

#include <regex>

#include <dirent.h>
#include <sys/stat.h>

/*static*/ const char gem::Dir::DELIM[] = "/";

bool
gem::Dir::scan(
    const std::string& dirname,
    std::function<void(const std::string&, const std::string&)> visitor,
    const std::string& filter,
    bool recursive)
{
    DIR* dir;
    if ((dir = opendir (dirname.c_str())) != nullptr)
    {
        struct dirent* entry;
        struct stat filestat;

        while ((entry = readdir (dir)) != nullptr)
        {
            std::string filename = entry->d_name;
            std::string filepath = dirname + DELIM + filename;

            // If the file is a directory (or is in some way invalid) we'll skip it 
            if (stat( filepath.c_str(), &filestat )) continue;

            if (S_ISDIR( filestat.st_mode ))
            {
                if (filename == "." or filename == "..") continue;
                if (recursive)
                {
                    gem::Dir::scan(filepath, visitor, filter, recursive);
                }
                else
                   continue;
            }

            if (!filter.empty() and !std::regex_match(filename, std::regex(filter)))
                continue;

            visitor(filename, filepath);
        }

        closedir (dir);
    }
    else
    {
        /* could not open directory */
        return false;
    }

    return true;
}

