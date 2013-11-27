/**@file
 * @brief     File operations
 * @author    Igor Lesik 2010-2013
 * @copyright 2010-2013 Igor Lesik
 *
 */
#ifndef CGEM_FILE_H_INCLUDED
#define CGEM_FILE_H_INCLUDED

#include <cstdio>
#include <string>
#include <functional>

class File
{
public:
    typedef long int pos_t;

    static const pos_t NPOS = -1;

private:
    FILE* file;

    std::string file_name;

public:

    File() : file (nullptr) {

    }

    ~File() {
        File::close ();
    }

    operator FILE* () {
        return file;
    }

    void close() {
        if (file != nullptr)
        {
            ::fclose (file);
            file = nullptr;
        }
        file_name = "";
    }

    bool open(const std::string& filename, const char* mode) {
        file = ::fopen (filename.c_str(), mode);
        if (file)
            this->file_name = filename;

        return (file != nullptr);
    }

    bool is_open() const {
        return file != nullptr;
    }

    bool is_eof() {
        return ::feof (file);
    }

    long int current_position() {
        return ::ftell (file);
    }

    bool write(const std::string& str)
    {
        return (::fputs (str.c_str(), file) >= 0);
    }

    bool read_line(std::string& str)
    {
        const size_t size = 4096;

        char buffer[size];

        bool got_line = ::fgets (buffer, size, file) != nullptr;

        if (got_line)
        {
            //str.assign (buffer, strlen (buffer) - 1 /*\n*/);
            str = buffer;
        }

        return got_line;
    }

    static
    bool open(const std::string& filename, const char* mode, std::function<bool(File&)> act) {
        File file;
        auto opened = file.open (filename, mode);
        if (not opened)
            return false;

        return act(file);
    }

    static
    bool touch(const std::string& filename) {
        File file;
        return File().open (filename, "w");
    }

    static
    bool is_exist(const std::string& filename) {
        return File().open (filename, "r");
    }

    static
    bool remove(const std::string& filename) {
        return 0 == ::remove (filename.c_str());
    }

    static
    std::string read(const std::string& filename) {
        std::string text;
        File::open (filename, "r", [&text](File& f)->bool{
            std::string line;
            while (f.read_line(line))
            {
                text.append(line);
            }

            return true;
        });

        return text;
    }

#if 0
    bool open_stdout(void) {
        file = stdout;
        m_fileName = "STDOUT";

        return (file != NULL);
    }




    std::string getName(void) {
        return m_fileName;
    };



    void rewind() {
        ::rewind (file);
    }


    size_t fseek(long int offset,
        int origin)
    {
        return ::fseek (file, offset, origin);
    }

#endif
};


template <class T>
bool fread_val(File& file, T& val)
{
    return fread ((void*)&val, sizeof (val), 1, file);
}

template <class T>
bool fread_val(File& file, File::pos_t pos, T& val)
{
    if ((pos != ftell (file)) && (0 != fseek (file, pos, SEEK_SET)))
    {
        return false;
    }

    return fread ((void*)&val, sizeof (val), 1, file);
}

template <class T>
bool fwrite_val(File& file, T& val)
{
    return fwrite ((void*)&val, sizeof (val), 1, file);
}


#endif
