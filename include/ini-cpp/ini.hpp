#ifndef INICPP_INI_H
#define INICPP_INI_H 1

#include "config.h"

#include "ini_section.hpp"
#include "detail/reverse_iterator.h"
#include "detail/section_iterator.h"

#include <unordered_map>
#include <list>
#include <type_traits>

namespace inicpp {
    class ini {
    public:
        typedef detail::section_iterator<ini_section> iterator;
        typedef detail::section_iterator<ini_section const> const_iterator;

        typedef detail::reverse_iterator<iterator> reverse_iterator;
        typedef detail::reverse_iterator<const_iterator> const_reverse_iterator;
    public:
        ini() = default;
        ini(const ini&) = default;
        ini(ini&&) = default;

        ini& operator=(const ini&) = default;
        ini& operator=(ini&&) = default;

        inline bool empty() const noexcept { return m_sections.empty() || begin() == end(); }

        inline void clear() noexcept { m_lookup_map.clear(); m_sections.clear(); }

        inline void push_front(const ini_section& value) { insert(cbegin(), value); }

        inline void push_front(ini_section&& value) { insert(cbegin(), std::move(value)); }

        inline void push_back(const ini_section& value) { insert(cend(), value); }

        inline void push_back(ini_section&& value) { insert(cend(), std::move(value)); }

        INICPP ini_section& find(const std::string& name);
        INICPP ini_section& find(std::string&& name);

        INICPP ini_section const& find(const std::string& name) const;
        INICPP ini_section const& find(std::string&& name) const;

        inline INICPP ini_section& operator[](const std::string& key) { return find(key); }
        inline INICPP ini_section& operator[](std::string&& key) { return find(std::move(key)); }

        inline INICPP const ini_section& operator[](const std::string& key) const { return find(key); }
        inline INICPP const ini_section& operator[](std::string&& key) const { return find(std::move(key)); }

        INICPP bool contains(const std::string& name) const;

        // Does not update previously created iterators
        INICPP ini_section& insert(const_iterator pos, ini_section const& sec);
        INICPP ini_section& insert(const_iterator pos, ini_section&& sec);

        inline INICPP ini_section& back() noexcept { return *--end(); }

        inline INICPP ini_section const& back() const noexcept { return *--end(); }

        inline INICPP ini_section& front() noexcept { return *begin(); }

        inline INICPP ini_section const& front() const noexcept { return *begin(); }

        inline INICPP iterator begin() noexcept { return iterator(m_sections.begin(), m_sections); }

        inline INICPP const_iterator begin() const noexcept { return const_iterator(m_sections.begin(), m_sections); }

        inline INICPP const_iterator cbegin() const noexcept { return const_iterator(m_sections.cbegin(), m_sections); }

        inline INICPP iterator end() noexcept { return iterator(m_sections.end(), m_sections); }

        inline INICPP const_iterator end() const noexcept { return const_iterator(m_sections.end(), m_sections); }

        inline INICPP const_iterator cend() const noexcept { return const_iterator(m_sections.cend(), m_sections); }

        inline INICPP reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

        inline INICPP const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

        inline INICPP const_reverse_iterator rcbegin() const noexcept { return const_reverse_iterator(cend()); }

        inline INICPP reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

        inline INICPP const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

        inline INICPP const_reverse_iterator rcend() const noexcept { return const_reverse_iterator(cbegin()); }

    private:
        mutable std::unordered_map<std::string, typename std::list<ini_section>::iterator> m_lookup_map;
        mutable std::list<ini_section> m_sections;

        friend class ini_section;
    };

    extern INICPP template class detail::section_iterator<ini_section>;
    extern INICPP template class detail::section_iterator<ini_section const>;

    extern INICPP template class detail::reverse_iterator<detail::section_iterator<ini_section>>;
    extern INICPP template class detail::reverse_iterator<detail::section_iterator<ini_section const>>;
}

#endif