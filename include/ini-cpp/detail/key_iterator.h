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
        struct reverse_key_iterator;

        template<typename K, typename V>
        struct key_iterator {
        public:
            typedef std::remove_reference_t<K> key_type;
            typedef std::remove_reference_t<V> value_type;
            typedef std::pair<key_type const, value_type> pair_type;
        private:
            typedef detail::ordered_map<key_type, std::remove_const_t<value_type>> impl_base_map;
            typedef std::conditional_t<std::is_const_v<value_type>, const impl_base_map, impl_base_map> impl_map;
            typedef std::conditional_t<std::is_const_v<value_type>, typename impl_map::const_iterator, typename impl_map::iterator> impl_iterator;

            typedef key_iterator<K, V> key_iterator_type;
        public:
            key_iterator(const key_iterator_type&) noexcept = default;
            key_iterator(key_iterator_type&&) noexcept = default;
            inline key_iterator(impl_iterator cur, impl_map& map) noexcept : m_cur(cur), m_map(&map) {}
            inline key_iterator(impl_map& map) noexcept : key_iterator(map.begin(), map) {}

            inline bool operator==(const key_iterator_type& other) const noexcept { return this->m_cur == other.m_cur; }
            inline bool operator!=(const key_iterator_type& other) const noexcept { return !operator==(other); }

            key_iterator_type& operator=(const key_iterator_type&) noexcept = default;
            key_iterator_type& operator=(key_iterator_type&&) noexcept = default;

            inline pair_type& operator*() noexcept { return *m_cur; }
            inline pair_type const& operator*() const noexcept { return *m_cur; }

            inline pair_type* operator->() noexcept { return m_cur.operator->(); }
            inline pair_type const* operator->() const noexcept { return m_cur.operator->(); }

            key_iterator_type& operator++() noexcept(std::is_const_v<value_type>) {
                if (m_map) {
                    auto end = m_map->end();
                    if (m_cur == end) return *this;
                    for (++m_cur;m_cur != end;++m_cur) {
                        if (!(m_cur->second)) {
                            if constexpr (!std::is_const_v<value_type>) {
                                auto next = m_map->erase(m_cur);
                                m_cur = --next;
                            }
                        } else break;
                    }
                } else { ++m_cur; }
                return *this;
            }

            inline key_iterator_type operator++(int) noexcept(std::is_const_v<value_type>) {
                key_iterator_type ret = *this;
                ++(*this);
                return ret;
            }

            key_iterator_type& operator--() noexcept(std::is_const_v<value_type>) {
                if (m_map) {
                    auto begin = m_map->begin();
                    if (m_cur == begin) return *this;
                    for (--m_cur; m_cur != begin; --m_cur) {
                        if (!(m_cur->second)) {
                            if constexpr (!std::is_const_v<value_type>) {
                                auto next = m_map->erase(m_cur);
                                m_cur = std::move(next);
                            }
                        } else break;
                    }
                } else { --m_cur; }
                return *this;
            }

            inline key_iterator_type operator--(int) noexcept(std::is_const_v<value_type>) {
                key_iterator_type ret = *this;
                --(*this);
                return ret;
            }
        private:
            impl_iterator m_cur;
            impl_map* const m_map;

            friend class ini_section;
            friend struct reverse_key_iterator<K, V>;
        };

        template<typename K, typename V>
        struct reverse_key_iterator {
            typedef key_iterator<K, V> key_iterator_type;
            typedef typename key_iterator_type::key_type key_type;
            typedef typename key_iterator_type::value_type value_type;
            typedef typename key_iterator_type::pair_type pair_type;
        private:
            typedef reverse_key_iterator<K, V> reverse_key_iterator_type;
        public:
            inline explicit reverse_key_iterator(const key_iterator_type& it) noexcept : m_it(it) {}
            inline explicit reverse_key_iterator(key_iterator_type&& it) noexcept : m_it(std::move(it)) {}

            inline bool operator==(const reverse_key_iterator_type& other) const noexcept { return this->m_it == other.m_it; }
            inline bool operator!=(const reverse_key_iterator_type& other) const noexcept { return !operator==(other); }

            inline pair_type& operator*() noexcept { auto tmp = m_it; return (--tmp).operator *(); }
            inline pair_type const& operator*() const noexcept { auto tmp = m_it; return (--tmp).operator *(); }

            inline pair_type* operator->() noexcept { auto tmp = m_it; return (--tmp).operator->(); }
            inline pair_type const* operator->() const noexcept { auto tmp = m_it; return (--tmp).operator->(); }

            inline reverse_key_iterator_type& operator++() noexcept(noexcept(key_iterator_type::operator++())) { --m_it; return *this; }
            inline reverse_key_iterator_type operator++(int) noexcept(noexcept(key_iterator_type::operator++(int(0)))) { auto ret = m_it--; return reverse_key_iterator(ret); }

            inline reverse_key_iterator_type& operator--() noexcept(noexcept(key_iterator_type::operator--())) { ++m_it; return *this; }
            inline reverse_key_iterator_type operator--(int) noexcept(noexcept(key_iterator_type::operator--(int(0)))) { auto ret = m_it++; return reverse_key_iterator(ret); }
        private:
            key_iterator_type m_it;
        };
    }
}
#endif