//
//https://github.com/boostcon/cppnow_presentations_2012/blob/master/wed/schurr_cpp11_tools_for_class_authors.pdf?raw=true
//
//http://akrzemi1.wordpress.com/2011/05/11/parsing-strings-at-compile-time-part-i/
//
#pragma once

#include <stdexcept>

class StaticString
{
private:
    const char* const p_;
    const std::size_t sz_;

public:

    // ctor
    template<std::size_t SZ>
    constexpr StaticString(const char(&a)[SZ]) : p_(a), sz_(SZ-1) {
        static_assert( SZ >= 1, "not a string literal");
    }

    constexpr StaticString(const StaticString& src, std::size_t pos = 0) :
        p_(src.p_ + pos), sz_(src.sz_ - pos)
    {
        //static_assert (pos <= src.sz_, "out of range");
    }

    constexpr StaticString(const StaticString& src, std::size_t pos, std::size_t sz) :
        p_(src.p_ + pos), sz_((sz < (src.sz_-pos))? sz : (src.sz_ - pos))
    {
        //static_assert (pos <= src.sz_, "out of range");
    }

    // operator []
    constexpr char operator[](std::size_t n) {
        return n < sz_ ? p_[n] : throw std::out_of_range("");
    }

    constexpr std::size_t size() {
        return sz_;
    }

    constexpr operator const char* () {
        return p_;
    }

    constexpr std::size_t compare(const char* s, std::size_t pos = 0, std::size_t counter = 0) {
        return pos == size() ? counter :
               p_[pos] == s[pos] ? compare(s, pos + 1, counter) :
                                   compare(s, pos + 1, counter + 1);
    }

    // operator ==
    constexpr bool operator==(const char* s) {
        return !compare(s);
    }

    static
    constexpr std::size_t count_char(
        const StaticString& str, char c, std::size_t pos = 0, std::size_t counter = 0)
    {
        return pos == str.size() ? counter :
               str[pos] == c ? count_char(str, c, pos + 1, counter + 1) :
                               count_char(str, c, pos + 1, counter);
    }

    static
    constexpr std::size_t count_tokens(
        const StaticString& str, char delim = ' ', std::size_t pos = 0, std::size_t counter = 0)
    {
        return pos == str.size() ? counter :
               str[pos] == delim ? count_tokens(str, delim, pos + 1, counter) :
               (pos == 0 or str[pos-1] == delim) ? count_tokens(str, delim, pos + 1, counter + 1) :
                                   count_tokens(str, delim, pos + 1, counter);
    }

};


