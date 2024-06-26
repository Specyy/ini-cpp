#ifndef INICPP_INI_VALUE_H
#define INICPP_INI_VALUE_H 1

#include "config.h"

#include <string>
#include <limits>
#include <stdexcept>
#include <optional>

namespace inicpp {
    class ini;
    class ini_section;
    class ini_value {
    public:
        constexpr ini_value() noexcept = default;
        constexpr ini_value(const ini_value&) = default;
        constexpr ini_value(ini_value&&) noexcept = default;
        constexpr inline ini_value(const std::string& data);
        constexpr inline ini_value(std::string&& data) noexcept;

        constexpr inline bool has_value() const noexcept { return m_data.has_value(); }
        constexpr inline explicit operator bool() const noexcept { return has_value(); }
        constexpr inline bool operator !() const noexcept { return !operator bool(); }

        constexpr inline const std::string& get_value() const { return m_data.value(); }

        INICPP void set_value(const std::string& data);
        INICPP void set_value(std::string&& data) noexcept;

        template<typename T>
        T as() const;

        template<typename T>
        inline operator T() const { return as<T>(); }

        constexpr ini_value& operator =(const ini_value&) = default;
        constexpr ini_value& operator =(ini_value&&) noexcept = default;

        template<typename T>
        ini_value& operator =(T);
    private:
        std::optional<std::string> m_data;
        ini_section* m_section = nullptr;

        friend class ini_section;
        friend class ini;
    };

    constexpr inline ini_value::ini_value(const std::string& data) : m_data(data) {}
    constexpr inline ini_value::ini_value(std::string&& data) noexcept : m_data(std::move(data)) {}

    template<>
    inline const char* ini_value::as() const { return get_value().c_str(); }

    template<>
    inline const std::string& ini_value::as() const { return get_value(); }

    template<>
    inline std::string ini_value::as() const { return get_value(); }

    template<>
    inline unsigned long long ini_value::as() const { return std::stoull(get_value()); }

    template<>
    inline signed long long ini_value::as() const { return std::stoll(get_value()); }

    template<>
    inline unsigned long ini_value::as() const { return std::stoul(get_value()); }

    template<>
    inline signed long ini_value::as() const { return std::stol(get_value()); }

    template<>
    inline unsigned int ini_value::as() const {
        unsigned long val = as<unsigned long>();
        if (val < std::numeric_limits<unsigned int>::min() || val > std::numeric_limits<unsigned int>::max())
            throw std::out_of_range("stoui");
        return static_cast<unsigned int>(val);
    }

    template<>
    inline signed int ini_value::as() const { return std::stoi(get_value()); }

    template<>
    inline unsigned short ini_value::as() const {
        unsigned long val = as<unsigned long>();
        if (val < std::numeric_limits<unsigned short>::min() || val > std::numeric_limits<unsigned short>::max())
            throw std::out_of_range("stous");
        return static_cast<unsigned short>(val);
    }

    template<>
    inline signed short ini_value::as() const {
        int val = as<int>();
        if (val < std::numeric_limits<short>::min() || val > std::numeric_limits<short>::max())
            throw std::out_of_range("stos");
        return static_cast<short>(val);
    }

    template<>
    inline char ini_value::as() const {
        if (get_value().length() != 1)
            throw std::length_error("ini_value::get_value()::length() != 1");
        return get_value()[0];
    }

    template<>
    inline unsigned char ini_value::as() const { return static_cast<unsigned char>(as<char>()); }

    template<>
    inline signed char ini_value::as() const { return static_cast<signed char>(as<char>()); }

    template<>
    inline float ini_value::as() const { return std::stof(get_value()); }

    template<>
    inline double ini_value::as() const { return std::stod(get_value()); }

    template<>
    inline long double ini_value::as() const { return std::stold(get_value()); }

    template<>
    inline ini_value& ini_value::operator =(const std::string& str) {
        this->set_value(str);
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(std::string&& str) {
        this->set_value(std::move(str));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(const char* const str) {
        this->set_value(std::string(str));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(unsigned long long num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(signed long long num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(unsigned long num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(signed long num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(unsigned int num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(signed int num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(unsigned short num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(signed short num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(char c) {
        this->set_value(std::string(1, c));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(signed char c) { return operator=(static_cast<char>(c)); }

    template<>
    inline ini_value& ini_value::operator =(unsigned char c) { return operator=(static_cast<char>(c)); }

    template<>
    inline ini_value& ini_value::operator =(float num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(double num) {
        this->set_value(std::to_string(num));
        return *this;
    }

    template<>
    inline ini_value& ini_value::operator =(long double num) {
        this->set_value(std::to_string(num));
        return *this;
    }
}

#endif