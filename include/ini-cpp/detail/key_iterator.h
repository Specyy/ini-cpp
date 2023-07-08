#ifndef INICPP_KEY_ITERATOR_H
#define INICPP_KEY_ITERATOR_H 1

#include "detail/ordered_map.h"

#include <stdexcept>
#include <type_traits>
#include <iterator>
#include <utility>

namespace inicpp {
    class ini_section;

    namespace detail {
        template<typename K, typename V>
        struct key_iterator {
        public:
            typedef std::remove_reference_t<K> key_type;
            typedef std::remove_reference_t<V> value_type;
            typedef std::conditional_t<std::is_const_v<value_type>, std::pair<key_type const, std::remove_const_t<value_type>> const,
                std::pair<key_type const, std::remove_const_t<value_type>>> pair_type;

            typedef pair_type& reference_type;
            typedef pair_type* pointer_type;
        private:
            typedef detail::ordered_map<key_type, std::remove_const_t<value_type>> impl_base_map;
            typedef std::conditional_t<std::is_const_v<value_type>, const impl_base_map, impl_base_map> impl_map;
            typedef std::conditional_t<std::is_const_v<value_type>, typename impl_map::const_iterator, typename impl_map::iterator> impl_iterator;

            typedef key_iterator<K, V> key_iterator_type;
        public:
            key_iterator(const key_iterator_type&) noexcept = default;
            key_iterator(key_iterator_type&&) noexcept = default;
            inline key_iterator(impl_iterator cur, impl_iterator begin, impl_iterator end) noexcept : m_cur(cur), m_begin(begin), m_end(end) { if (m_cur != end && !(m_cur->second)) { operator++(); m_begin = m_cur; } }
            inline key_iterator(impl_iterator cur, impl_map& map) noexcept : key_iterator(cur, map.begin(), map.end()) {}
            inline key_iterator(impl_map& map) noexcept : key_iterator(map.begin(), map) {}

            inline bool operator==(const key_iterator_type& other) const noexcept { return this->m_cur == other.m_cur; }
            inline bool operator!=(const key_iterator_type& other) const noexcept { return !operator==(other); }

            key_iterator_type& operator=(const key_iterator_type&) noexcept = default;
            key_iterator_type& operator=(key_iterator_type&&) noexcept = default;

            inline reference_type operator*() const noexcept { return *m_cur; }
            inline pointer_type operator->() const noexcept { return m_cur.operator->(); }

            key_iterator_type& operator++() noexcept {
                if (m_cur != m_end) {
                    for (++m_cur; m_cur != m_end; ++m_cur) {
                        if ((m_cur->second)) break;
                    }
                }

                return *this;
            }

            inline key_iterator_type operator++(int) noexcept {
                key_iterator_type ret = *this;
                ++(*this);
                return ret;
            }

            key_iterator_type& operator--() noexcept(std::is_const_v<value_type>) {
                if (m_cur != m_begin) {
                    for (--m_cur; m_cur != m_begin; --m_cur) {
                        if ((m_cur->second)) break;
                    }
                }
                return *this;
            }

            inline key_iterator_type operator--(int) noexcept(std::is_const_v<value_type>) {
                key_iterator_type ret = *this;
                --(*this);
                return ret;
            }
        private:
            impl_iterator m_cur;
            impl_iterator m_begin;
            const impl_iterator m_end;

            friend class inicpp::ini_section;
        };
    }
}
#endif