
#include "XMLParser.h"

#include <iostream>

std::string xml_text = R"XML(
<?xml version="1.0" ?>

<tag0 >
  <tag1 attr1="attr1">hello</tag1>

  <tag2 attr1="attr1"
        attr2="attr2">world</tag2>

</tag0>
)XML";

using namespace gem;

int main()
{
  {
    xml::Walker walker;

    auto on_start = [](const xml::string el, xml::string::size_type pos)->void {
        std::cout << "on start:<" << el << "> pos=" << pos << "\n";
    };

    auto on_end = [](const xml::string el, xml::string::size_type pos)->void {
        std::cout << "on end:<" << el << "> pos=" << pos << "\n";
    };

    walker.walk(xml_text, on_start, on_end);
  }

    xml::Parser parser;

    auto on_end = [](
        xml::Parser::Path const& path, xml::string const& tag,
        xml::string::size_type content_begin, xml::string::size_type content_size
        )->void
    {
        std::cout << "on end: tag=" << tag
        << " parent=" << path.back()
        << " content=\n" << xml_text.substr(content_begin, content_size)
        << "\n";
    };

    parser.parse(xml_text, /*on_start=*/nullptr, on_end);

    return 0;
}
