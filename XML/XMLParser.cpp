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
                        on_element_end(element.substr(1), pos-element.size()-1);
                    }
                }
                else
                {
                    if (on_element_start != nullptr)
                    {
                        on_element_start(element, pos+1);

                        bool is_decl_tag = element[0] == '?';
                        if (is_decl_tag and element.back() != '?')
                            throw xml::exception("unmatching '?'");

                        bool is_comment = element[0] == '!';

                        bool is_empty_tag = element.back() == '/' or is_decl_tag or is_comment;

                        if (is_empty_tag and on_element_end != nullptr)
                        {
                            on_element_end(element, pos+1);
                        }
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

    auto extract_tag = [](const xml::string& s)->xml::string {
        return s.substr(0, s.find_first_of(" \t\n\r"));
    };

    auto on_start = [&](const xml::string el, xml::string::size_type pos)->void {
        xml::string tag = extract_tag(el);

        if (on_element_start != nullptr)
        {
            AttrList attrs;

            on_element_start(
                path, tag, attrs
            );
        }

        path.push_back(tag);
        path_pos.push_back(pos);
    };

    auto on_end = [&](const xml::string el, xml::string::size_type pos)->void {
        xml::string tag = extract_tag(el);
        auto expected_tag = extract_tag(path.back());
        auto content_begin = path_pos.back();
        path.pop_back();
        path_pos.pop_back();
        if (tag != expected_tag)
            throw xml::exception(std::string("unmatching tags <")+expected_tag+"> <"+tag+">");

        if (on_element_end != nullptr)
        {
            on_element_end(
                path, tag, content_begin, (pos - content_begin)
            );
        }
    };

    return walker.walk(text, on_start, on_end);
}

