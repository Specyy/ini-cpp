#ifndef INICPP_INI_SECTION_H
#define INICPP_INI_SECTION_H 1

#include "config.h"
#include "ini_value.hpp"
#include "detail/key_iterator.h"
#include "detail/ordered_map.h"
#include "detail/reverse_iterator.h"

#include <stdexcept>

namespace inicpp {
    class ini;
    class ini_value;

    class ini_section {
    public:
        typedef detail::key_iterator<std::string, ini_value> iterator;
        typedef detail::key_iterator<std::string, const ini_value> const_iterator;

        typedef detail::reverse_iterator<iterator> reverse_iterator;
        typedef detail::reverse_iterator<const_iterator> const_reverse_iterator;
    public:
        ini_section() = default;
        inline INICPP ini_section(const std::string& name);
        inline INICPP ini_section(std::string&& name);

        inline INICPP bool empty() const noexcept { return m_data.empty() || begin() == end(); }

        /**
         * @brief Attempts to find a key within the ini section. If the key does not exist, an invalid @c ini_value is returned.
         * @param key The key to find
         * @return A valid or invalid ini_value
         */
        inline INICPP ini_value& find(const std::string& key) {
            auto& v = m_data[key];
            v.m_section = this;
            return v;
        }

        /**
         * @brief Attempts to find a key within the ini section. If the key does not exist, an invalid @c ini_value is returned.
         * @param key The key to find
         * @return A valid or invalid ini_value
         */
        inline INICPP ini_value& find(std::string&& key) {
            auto& v = m_data[std::move(key)];
            v.m_section = this;
            return v;
        }

        /**
         * @brief Attempts to find a key within the ini section. If the key does not exist, an invalid @c ini_value is returned.
         * @param key The key to find
         * @return A valid or invalid ini_value
         */
        INICPP const ini_value& find(const std::string& key) const;

        /**
         * @brief Attempts to find a key within the ini section. If the key does not exist, an invalid @c ini_value is returned.
         * @param key The key to find
         * @return A valid or invalid ini_value
         */
        INICPP const ini_value& find(std::string&& key) const;

        inline INICPP void push_front(const typename detail::ordered_map<std::string, ini_value>::value_type& value) { insert(cbegin(), value); }

        inline INICPP void push_front(typename detail::ordered_map<std::string, ini_value>::value_type&& value) { insert(cbegin(), std::move(value)); }

        inline INICPP void push_back(const typename detail::ordered_map<std::string, ini_value>::value_type& value) { insert(cend(), value); }

        inline INICPP void push_back(typename detail::ordered_map<std::string, ini_value>::value_type&& value) { insert(cend(), std::move(value)); }

        /**
         * @brief Associates a key with a particular value. If the underlying key already exists, its value is overridden.
         * @param key The key to find
         * @param value The value to assign
         * @return The assigned value
         */
        inline INICPP ini_value& insert(const_iterator pos, const typename detail::ordered_map<std::string, ini_value>::value_type& value) {
            auto i = m_data.insert(pos.m_cur, value);
            i->second.m_section = this;
            m_exists = m_exists || i->second.has_value();
            return i->second;
        }

        /**
        * @brief Associates a key with a particular value. If the underlying key already exists, its value is overridden.
        * @param key The key to find
        * @param value The value to assign
        * @return The assigned value
        */
        inline INICPP ini_value& insert(const_iterator pos, typename detail::ordered_map<std::string, ini_value>::value_type&& value) {
            auto i = m_data.insert(pos.m_cur, std::move(value));
            i->second.m_section = this;
            m_exists = m_exists || i->second.has_value();
            return i->second;
        }

        /**
         * @brief Replaces the value of a key inside the ini section. If the key does not exist, an exception of type @c std::invalid_argument is thrown.
         * @param key The key to find
         * @param value The value to replace
         * @return The old value associated with this key
         */
        inline INICPP ini_value replace(const std::string& key, const ini_value& value) {
            if (!contains(key)) { throw std::invalid_argument("ini_section::replace"); }
            auto f = m_data.find(key);
            ini_value old = f->second;
            f->second = value;
            f->second.m_section = this;
            return old;
        }

        /**
         * @brief Replaces the value of a key inside the ini section. If the key does not exist, an exception of type @c std::invalid_argument is thrown.
         * @param key The key to find
         * @param value The value to replace
         * @return The old value associated with this key
         */
        inline INICPP ini_value replace(const std::string& key, ini_value&& value) {
            if (!contains(key)) { throw std::invalid_argument("ini_section::replace"); }
            auto f = m_data.find(key);
            ini_value old = f->second;
            f->second = std::move(value);
            f->second.m_section = this;
            return old;
        }

        /**
         * @brief Removes a key from the ini section
         * @param key The key to remove
         * @return True if the element has been removed, false if it did not exist
         */
        inline INICPP bool remove(const std::string& key) {
            if (!contains(key)) { return false; }
            m_data.erase(key);
            return true;
        }

        /**
         * @brief Checks whether the section contains the specified key
         * @param key Name of the key to check
         * @return True if the sections contains the underlying key, false otherwise.
         */
        inline INICPP bool contains(const std::string& key) const {
            auto f = m_data.find(key);
            return f != m_data.end() && (f->second);
        }

        /**
         * @brief Clears the contents of the section.
         */
        inline INICPP void clear() noexcept { return m_data.clear(); }

        /**
         * @brief Retrieves the name of the ini section
         * @return The name of the section
         */
        inline INICPP const std::string& get_name() const noexcept { return this->m_name; }

        /**
         * @brief Changes the name of the ini section. If a section with the corresponding name already exists, an exception of type @c std::invalid_argument is thrown.
         * No exception is thrown if this section is not bound to an ini file.
         * @param name The new name of the section
         */
        INICPP void set_name(const std::string& name);

        /**
         * @brief Changes the name of the ini section. If a section with the corresponding name already exists, an exception of type @c std::invalid_argument is thrown.
         * No exception is thrown if this section is not bound to an ini file.
         * @param name The new name of the section
         */
        INICPP void set_name(std::string&& name);

        /**
         * @brief Checks whether the ini section is in a valid state (whether it exists inside the ini file). This function returns true for a default constructed @c ini_section.
         * @return True if the object is valid, false otherwise
         */
        inline INICPP explicit operator bool() const noexcept { return m_exists; }
        inline INICPP bool operator !() const noexcept { return !operator bool(); }

        inline INICPP ini_value& operator[](const std::string& key) { return find(key); }
        inline INICPP ini_value& operator[](std::string&& key) { return find(std::move(key)); }

        inline INICPP const ini_value& operator[](const std::string& key) const { return find(key); }
        inline INICPP const ini_value& operator[](std::string&& key) const { return find(std::move(key)); }

        inline INICPP typename detail::ordered_map<std::string, ini_value>::value_type& front() noexcept { return *begin(); }
        inline INICPP typename detail::ordered_map<std::string, ini_value>::value_type const& front() const noexcept { return *begin(); }

        inline INICPP typename detail::ordered_map<std::string, ini_value>::value_type& back() noexcept { return *--end(); }
        inline INICPP typename detail::ordered_map<std::string, ini_value>::value_type const& back() const noexcept { return *--end(); }

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
        mutable detail::ordered_map<std::string, ini_value> m_data;
        mutable bool m_exists = true;
        ini* m_ini = nullptr;

        friend class ini;
        friend class ini_value;
    };

    inline INICPP ini_section::ini_section(const std::string& name) : m_name(name) {}
    inline INICPP ini_section::ini_section(std::string&& name) : m_name(std::move(name)) {}

    extern INICPP template class detail::key_iterator<std::string, ini_value>;
    extern INICPP template class detail::key_iterator<std::string, const ini_value>;

    extern INICPP template class detail::reverse_iterator<detail::key_iterator<std::string, ini_value>>;
    extern INICPP template class detail::reverse_iterator<detail::key_iterator<std::string, const ini_value>>;

    extern INICPP template class detail::ordered_map<std::string, ini_value>;
}

#endif