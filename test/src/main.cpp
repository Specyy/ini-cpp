#include <iostream>
#include <ini-cpp/ini.hpp>
#include <fstream>

int main() {
    std::istringstream input(R"(
        [section]
        key1=value1 ; this is a comment
        key2=3.14159265358979 
        
        # This is another comment
        key3=!
        
        // This is a third comment
    )");

    inicpp::ini ini;
    input >> ini;

    auto& section1 = ini["section1"];
    if (section1) {
        std::cout << "section1 exists\n";
    } else {
        std::cout << "section1 does not exist\n";
    }

    auto& section = ini["section"];
    if (section) {
        if (section["key1"]) {
            std::cout << "key1 is: " << section["key1"].as<const std::string&>() << "\n";
        }

        if (section["key2"]) {
            std::cout << "key2 is: " << (double)section["key2"] << "\n";
        }

        if (section["key3"]) {
            std::cout << "key3 is: " << section["key3"].as<char>() << "\n";
        }

        section["key4"] = 2.718281828459045f;
    }

    section1["key5"] = 64;

    std::cout << "\n" << ini << std::flush;
}