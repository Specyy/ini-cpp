#ifndef INICPP_PARSER_EXCEPTION_H
#define INICPP_PARSER_EXCEPTION_H 1

#include "config.h"

#include <stdexcept>

namespace inicpp {
    class parser_exception : public std::runtime_error {
    public:
        inline parser_exception(const char* const what);
        inline parser_exception(const std::string& what);
        parser_exception(const parser_exception&) noexcept = default;
        parser_exception& operator=(const parser_exception&) noexcept = default;
    };

    inline parser_exception::parser_exception(const char* const what) : std::runtime_error(what) {}
    inline parser_exception::parser_exception(const std::string& what) : std::runtime_error(what) {}
}

#endif