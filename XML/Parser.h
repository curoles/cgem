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
#include <functional>

namespace gem { namespace xml {

typedef std::string string;

enum class Error
{
    OK = 0
};

class Parser
{
public:
    typedef std::function<void (xml::string)> ElemStartCb;
    typedef std::function<void (xml::string)> ElemEndCb;

    xml::Error parse(
        const xml::string& text,
        ElemStartCb on_element_start = nullptr,
        ElemEndCb on_element_end = nullptr
    );

    bool is_white_space(char c)
    {
        return c == ' ' or c == '\t' or c == '\n';
    }

    bool is_tag_opening(char c)
    {
        return c == '<';
    }

    bool is_tag_closing(char c)
    {
        return c == '>';
    }

};

}} //gem::xml

using namespace gem;

xml::Error
gem::xml::Parser::
parse(
    const xml::string& text,
    ElemStartCb on_element_start,
    ElemEndCb on_element_end
)
{
    bool tag {false};
    //bool skip_ws {true};
    //bool start_tag {false};
    xml::string element;

    for (auto c : text)
    {
        //if (skip_ws and is_white_space(c)) continue;

        if (tag)
        {
            if (is_tag_closing(c))
            {
                tag = false;
                //skip_ws = true;//FIXME
                // TODO validate syntax
                bool is_end_tag = element[0] == '/';
                if (is_end_tag)
                {
                    if (on_element_end != nullptr)
                    {
                        on_element_end(element);
                    }
                }
                else
                {
                    if (on_element_start != nullptr)
                    {
                        on_element_start(element);
                    }
                }
            }
            else
            {
                element.push_back(c);
            }
        }
        else
        {
            if (is_tag_opening(c))
            {
                tag = true;
                //skip_ws = false;
                element.erase();
            }
        }
    }

    return xml::Error::OK;
}

