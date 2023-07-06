#ifndef INICPP_INI_SECTION_H
#define INICPP_INI_SECTION_H 1

#include "config.h"
#include "ini_value.hpp"
#include "detail/key_iterator.h"
#include "detail/ordered_map.h"

namespace inicpp {
    class ini;
    class ini_section {
    public:
        typedef detail::key_iterator<std::string, ini_value> iterator;
        typedef detail::key_iterator<std::string, const ini_value> const_iterator;

        typedef detail::reverse_key_iterator<std::string, ini_value> reverse_iterator;
        typedef detail::reverse_key_iterator<std::string, const ini_value> const_reverse_iterator;
    public:
        ini_section() = default;
        inline INICPP ini_section(const std::string& name);
        inline INICPP ini_section(std::string&& name) noexcept;

        inline INICPP ini_value& find(const std::string& key) { return m_data[key]; }

        inline INICPP const ini_value& find(const std::string& key) const {
            if (!contains(key)) { throw std::invalid_argument("ini_section::find()"); }
            return m_data.find(key)->second;
        }

        inline INICPP ini_value& insert(const std::string& key, const ini_value& value) { m_data[key] = value; return m_data[key]; }
        inline INICPP ini_value& insert(const std::string& key, ini_value&& value) { m_data[key] = std::move(value); return m_data[key]; }
        inline INICPP ini_value& insert(std::string&& key, const ini_value& value) { m_data[std::move(key)] = value; return m_data[key]; }
        inline INICPP ini_value& insert(std::string&& key, ini_value&& value) { m_data[std::move(key)] = std::move(value); return m_data[key]; }

        inline INICPP ini_value replace(const std::string& key, const ini_value& value) {
            if (!contains(key)) { throw std::invalid_argument("ini_section::replace()"); }
            auto f = m_data.find(key);
            ini_value old = f->second;
            f->second = value;
            return old;
        }

        inline INICPP ini_value replace(const std::string& key, ini_value&& value) {
            if (!contains(key)) { throw std::invalid_argument("ini_section::replace()"); }
            auto f = m_data.find(key);
            ini_value old = f->second;
            f->second = std::move(value);
            return old;
        }

        inline INICPP bool remove(const std::string& key) {
            if (!contains(key)) { return false; }
            m_data.erase(key);
            return true;
        }

        inline INICPP bool contains(const std::string& key) const { return m_data.find(key) != m_data.end() && (m_data.find(key)->second); }

        inline INICPP void clear() noexcept { return m_data.clear(); }

        inline INICPP ini_value& operator[](const std::string& key) { return find(key); }
        inline INICPP const ini_value& operator[](const std::string& key) const { return find(key); }

        inline INICPP const std::string& get_name() const noexcept { return this->m_name; }
        inline INICPP void set_name(const std::string& name) { this->m_name = name; }
        inline INICPP void set_name(std::string&& name) noexcept { this->m_name = std::move(name); }

        inline INICPP operator bool() const noexcept { return m_exists; }
        inline INICPP bool operator !() const noexcept { return !operator bool(); }

        inline INICPP iterator begin() noexcept { return iterator(m_data.begin(), m_data); }
        inline INICPP const_iterator begin() const noexcept { return const_iterator(m_data.begin(), m_data); }
        inline INICPP const_iterator cbegin() const noexcept { return const_iterator(m_data.begin(), m_data); }
        inline INICPP iterator end() noexcept { return iterator(m_data.end(), m_data); }
        inline INICPP const_iterator end() const noexcept { return const_iterator(m_data.end(), m_data); }
        inline INICPP const_iterator cend() const noexcept { return const_iterator(m_data.end(), m_data); }

        inline INICPP reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        inline INICPP const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        inline INICPP const_reverse_iterator rcbegin() const noexcept { return const_reverse_iterator(cend()); }
        inline INICPP reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
        inline INICPP const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        inline INICPP const_reverse_iterator rcend() const noexcept { return const_reverse_iterator(cbegin()); }
    private:
        std::string m_name;
        detail::ordered_map<std::string, ini_value> m_data;
        bool m_exists = true;
        
        friend class ini;
    };

    inline INICPP ini_section::ini_section(const std::string& name) : m_name(name) {}
    inline INICPP ini_section::ini_section(std::string&& name) noexcept : m_name(name) {}
}

#endif