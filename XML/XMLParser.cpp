/*
 *
 * XML syntax validator:
 * http://www.w3schools.com/xml/xml_validator.asp
 *
 * Handling whitespaces:
 * http://www.usingxml.com/Basics/XmlSpace
 */
#include "XMLParser.h"

using namespace gem;

xml::Error
gem::xml::Walker::
walk(
    const xml::string& text,
    ElemStartCb on_element_start,
    ElemEndCb on_element_end
)
{
    bool tag {false};
    //bool skip_ws {true};
    //bool start_tag {false};
    xml::string element;
    xml::string::size_type pos{0};

    //for (auto c : text)
    for (auto ptr = text.begin(); ptr != text.end(); ++ptr, ++pos)
    {
        auto c = *ptr;

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
                        on_element_end(element, pos-element.size()-1);
                    }
                }
                else
                {
                    if (on_element_start != nullptr)
                    {
                        on_element_start(element, pos+1);
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

xml::Error
gem::xml::Parser::
parse(
    const xml::string& text,
    ElemStartCb on_element_start,
    ElemEndCb on_element_end
)
{
    Path path;
    std::list<xml::string::size_type> path_pos;

    xml::Walker walker;

    auto on_start = [&](const xml::string el, xml::string::size_type pos)->void {
        xml::string tag = el;
        path.push_back(tag);
        path_pos.push_back(pos);
    };

    auto on_end = [&](const xml::string el, xml::string::size_type pos)->void {
        xml::string tag = el.substr(1);
        auto expected_tag = path.back();
        auto content_begin = path_pos.back();
        path.pop_back();
        path_pos.pop_back();
        //TODO assert tag == expected_tag
        if (on_element_end != nullptr)
        {
            on_element_end(
                path, tag, content_begin, (pos - content_begin)
            );
        }
    };

    return walker.walk(text, on_start, on_end);
}

