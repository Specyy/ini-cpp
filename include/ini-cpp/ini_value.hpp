#ifndef INICPP_INI_VALUE_H
#define INICPP_INI_VALUE_H 1

#include "config.h"

#include <string>
#include <limits>
#include <stdexcept>
#include <optional>

namespace inicpp {
    class ini_value {
    public:
        constexpr ini_value() noexcept = default;
        constexpr ini_value(const ini_value&) = default;
        constexpr ini_value(ini_value&&) noexcept = default;
        constexpr inline INICPP ini_value(const std::string& data);
        constexpr inline INICPP ini_value(std::string&& data) noexcept;

        constexpr inline INICPP bool has_value() const noexcept { return m_data.has_value(); }
        constexpr inline INICPP operator bool() const noexcept { return has_value(); }
        constexpr inline INICPP bool operator !() const noexcept { return !operator bool(); }

        constexpr inline INICPP std::string& get_value() { return m_data.value(); }
        constexpr inline INICPP const std::string& get_value() const { return m_data.value(); }

        inline INICPP void set_value(const std::string& data) { this->m_data = data; }
        inline INICPP void set_value(std::string&& data) noexcept { this->m_data = std::move(data); }

        template<typename T>
        INICPP T as() const;

        template<typename T>
        inline INICPP operator T() const { return as<T>(); }

        constexpr ini_value& operator =(const ini_value&) = default;
        constexpr ini_value& operator =(ini_value&&) noexcept = default;

        template<typename T>
        INICPP ini_value& operator =(T);
    private:
        std::optional<std::string> m_data;
    };

    constexpr inline INICPP ini_value::ini_value(const std::string& data) : m_data(data) {}
    constexpr inline INICPP ini_value::ini_value(std::string&& data) noexcept : m_data(std::move(data)) {}

    template<>
    inline INICPP const char* ini_value::as() const { return get_value().c_str(); }

    template<>
    inline INICPP const std::string& ini_value::as() const { return get_value(); }

    template<>
    inline INICPP std::string ini_value::as() const { return get_value(); }

    template<>
    inline INICPP unsigned long long ini_value::as() const { return std::stoull(get_value()); }

    template<>
    inline INICPP signed long long ini_value::as() const { return std::stoll(get_value()); }

    template<>
    inline INICPP unsigned long ini_value::as() const { return std::stoul(get_value()); }

    template<>
    inline INICPP signed long ini_value::as() const { return std::stol(get_value()); }

    template<>
    inline INICPP unsigned int ini_value::as() const {
        unsigned long val = as<unsigned long>();
        if (val < std::numeric_limits<unsigned int>::min() || val > std::numeric_limits<unsigned int>::max())
            throw std::out_of_range("stoui");
        return static_cast<unsigned int>(val);
    }

    template<>
    inline INICPP signed int ini_value::as() const { return std::stoi(get_value()); }

    template<>
    inline INICPP unsigned short ini_value::as() const {
        unsigned long val = as<unsigned long>();
        if (val < std::numeric_limits<unsigned short>::min() || val > std::numeric_limits<unsigned short>::max())
            throw std::out_of_range("stous");
        return static_cast<unsigned short>(val);
    }

    template<>
    inline INICPP signed short ini_value::as() const {
        int val = as<int>();
        if (val < std::numeric_limits<short>::min() || val > std::numeric_limits<short>::max())
            throw std::out_of_range("stos");
        return static_cast<short>(val);
    }

    template<>
    inline INICPP char ini_value::as() const {
        if (get_value().length() != 1)
            throw std::length_error("ini_value::get_value()::length() != 1");
        return get_value()[0];
    }

    template<>
    inline INICPP unsigned char ini_value::as() const { return static_cast<unsigned char>(as<char>()); }

    template<>
    inline INICPP signed char ini_value::as() const { return static_cast<signed char>(as<char>()); }

    template<>
    inline INICPP float ini_value::as() const { return std::stof(get_value()); }

    template<>
    inline INICPP double ini_value::as() const { return std::stod(get_value()); }

    template<>
    inline INICPP long double ini_value::as() const { return std::stold(get_value()); }

    template<>
    inline INICPP ini_value& ini_value::operator =(const std::string& str) {
        this->set_value(str);
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(std::string&& str) {
        this->set_value(std::move(str));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(const char* const str) {
        this->set_value(std::string(str));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(unsigned long long num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(signed long long num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(unsigned long num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(signed long num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(unsigned int num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(signed int num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(unsigned short num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(signed short num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(char c) {
        this->set_value(std::string(1, c));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(signed char c) { return operator=(static_cast<char>(c)); }

    template<>
    inline INICPP ini_value& ini_value::operator =(unsigned char c) { return operator=(static_cast<char>(c)); }

    template<>
    inline INICPP ini_value& ini_value::operator =(float num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(double num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline INICPP ini_value& ini_value::operator =(long double num) {
        this->set_value(std::to_string(num));
        return *this;
    }
}

#endif