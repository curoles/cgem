
#include "Parser.h"

#include <iostream>

std::string xml_text = R"XML(
<?xml version="1.0" ?>

< tag0 >
  <tag1 attr1="attr1">hello</tag1>

  <tag2 attr1="attr1"
        attr2="attr2">hello</tag2>

</tag0>
)XML";

using namespace gem;

int main()
{
    xml::Parser parser;

    auto on_start = [](const xml::string el)->void {
        std::cout << "on start:<" << el << ">\n";
    };

    auto on_end = [](const xml::string el)->void {
        std::cout << "on end:<" << el << ">\n";
    };

    parser.parse(xml_text, on_start, on_end);

    return 0;
}
