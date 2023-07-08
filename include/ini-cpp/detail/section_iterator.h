#ifndef INICPP_DETAIL_SECTION_ITERATOR_H
#define INICPP_DETAIL_SECTION_ITERATOR_H 1

#include <type_traits>
#include <list>

namespace inicpp {
    class ini;
    namespace detail {
        template<typename T>
        struct section_iterator {
        public:
            typedef std::remove_reference_t<T> value_type;
            typedef value_type& reference_type;
            typedef value_type* pointer_type;
        private:
            typedef std::list<std::remove_const_t<value_type>> impl_base_list;
            typedef std::conditional_t<std::is_const_v<value_type>, impl_base_list const, impl_base_list> impl_list;
            typedef std::conditional_t<std::is_const_v<value_type>, typename impl_list::const_iterator, typename impl_list::iterator> impl_iterator;

            typedef section_iterator<T> section_iterator_type;
        public:
            section_iterator(const section_iterator_type&) noexcept = default;
            section_iterator(section_iterator_type&&) noexcept = default;
            inline section_iterator(impl_iterator cur, impl_iterator begin, impl_iterator end) noexcept : m_cur(cur), m_begin(begin), m_end(end) { if (m_cur != end && !(*m_cur)) { operator++(); m_begin = m_cur; } }
            inline section_iterator(impl_iterator cur, impl_list& list) noexcept : section_iterator(cur, list.begin(), list.end()) {}
            inline section_iterator(impl_list& list) noexcept : section_iterator_type(list.begin(), list) {}

            inline bool operator==(const section_iterator_type& other) const noexcept { return this->m_cur == other.m_cur; }
            inline bool operator!=(const section_iterator_type& other) const noexcept { return !operator==(other); }

            section_iterator_type& operator=(const section_iterator_type&) noexcept = default;
            section_iterator_type& operator=(section_iterator_type&&) noexcept = default;

            reference_type operator*() const noexcept { return *m_cur; }
            pointer_type operator->() const noexcept { return m_cur.operator->(); }

            section_iterator_type& operator++() noexcept {
                if (m_cur != m_end) {
                    for (++m_cur; m_cur != m_end;++m_cur) {
                        if ((*m_cur)) break;
                    }
                }

                return *this;
            }

            inline section_iterator_type operator++(int) noexcept {
                section_iterator_type ret = *this;
                ++(*this);
                return ret;
            }

            section_iterator_type& operator--() noexcept {
                if (m_cur != m_begin) {
                    for (--m_cur; m_cur != m_begin; --m_cur) {
                        if ((*m_cur)) break;
                    }
                }
                return *this;
            }

            inline section_iterator_type operator--(int) noexcept {
                section_iterator_type ret = *this;
                --(*this);
                return ret;
            }
        private:
            impl_iterator m_cur;
            impl_iterator m_begin;
            const impl_iterator m_end;

            friend class inicpp::ini;
        };
    }
}
#endif