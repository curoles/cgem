/*
 *
 * XML syntax validator:
 * http://www.w3schools.com/xml/xml_validator.asp
 *
 * Handling whitespaces:
 * http://www.usingxml.com/Basics/XmlSpace
 */
#pragma once

#include <string>
#include <list>
#include <functional>

namespace gem { namespace xml {

typedef std::string string;

enum class Error
{
    OK = 0
};

class Walker
{
public:
    typedef std::function<void (xml::string, xml::string::size_type)> ElemStartCb;
    typedef std::function<void (xml::string, xml::string::size_type)> ElemEndCb;

    static
    xml::Error walk(
        const xml::string& text,
        ElemStartCb on_element_start = nullptr,
        ElemEndCb on_element_end = nullptr
    );

    static
    bool is_white_space(char c)
    {
        return c == ' ' or c == '\t' or c == '\n';
    }

    static
    bool is_tag_opening(char c)
    {
        return c == '<';
    }

    static
    bool is_tag_closing(char c)
    {
        return c == '>';
    }

};

class Parser
{
public:
    typedef std::list<xml::string> AttrList;
    typedef std::list<xml::string> Path;

    typedef std::function<
        void (Path const& path, xml::string const& tag, AttrList const& attrs)
    > ElemStartCb;
    typedef std::function<
        void (Path const& path, xml::string const& tag,
              xml::string::size_type content_begin, xml::string::size_type content_size)
    > ElemEndCb;

    static
    xml::Error parse(
        const xml::string& text,
        ElemStartCb on_element_start = nullptr,
        ElemEndCb on_element_end = nullptr
    );
};

}} //gem::xml

