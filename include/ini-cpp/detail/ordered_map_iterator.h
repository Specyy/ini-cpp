#ifndef INICPP_ORDERED_MAP_ITERATOR_H
#define INICPP_ORDERED_MAP_ITERATOR_H 1

#include <unordered_map>
#include <list>
#include <type_traits>
#include <utility>
#include <functional>
#include <stdexcept>

namespace inicpp::detail {
    template<typename K, typename V>
    class ordered_map;

    template<typename K, typename V>
    struct ordered_map_reverse_iterator;

    template<typename K, typename V>
    struct ordered_map_iterator {
    private:
        typedef std::unordered_map<K, std::remove_const_t<std::remove_reference_t<V>>> base_unordered_map_type;
        typedef std::conditional_t<std::is_const_v<std::remove_reference_t<V>>, base_unordered_map_type, base_unordered_map_type const>
            unordered_map_type;

        typedef ordered_map_iterator<K, V> ordered_map_iterator_type;
        typedef ordered_map<K, std::remove_const_t<std::remove_reference_t<V>>> base_ordered_map_type;

        typedef typename std::list<typename base_ordered_map_type::value_type*>::const_iterator iterator_type;
    public:
        typedef std::conditional_t<std::is_const_v<std::remove_reference_t<V>>, base_ordered_map_type const, base_ordered_map_type> ordered_map_type;
        typedef std::conditional_t<std::is_const_v<std::remove_reference_t<V>>, typename ordered_map_type::value_type const, typename ordered_map_type::value_type> value_type;
        typedef value_type& reference_type;
        typedef value_type* pointer_type;
    public:
        template<typename K1, typename = std::enable_if_t<std::is_const_v<std::remove_reference_t<V>> && std::is_same_v<K1, K>>>
        inline ordered_map_iterator(const ordered_map_iterator<K1, std::remove_const_t<std::remove_reference_t<V>>>& other) noexcept
            : m_order_it(other.m_order_it) {}

        template<typename K1, typename = std::enable_if_t<std::is_const_v<std::remove_reference_t<V>> && std::is_same_v<K1, K>>>
        inline ordered_map_iterator(ordered_map_iterator<K1, std::remove_const_t<std::remove_reference_t<V>>>&& other) noexcept
            : m_order_it(std::move(other.m_order_it)) {}

        inline ordered_map_iterator(iterator_type it) noexcept
            : m_order_it(it) {}

        ordered_map_iterator(const ordered_map_iterator&) noexcept = default;
        ordered_map_iterator(ordered_map_iterator&&) noexcept = default;

        constexpr ordered_map_iterator& operator=(const ordered_map_iterator&) noexcept = default;
        constexpr ordered_map_iterator& operator=(ordered_map_iterator&&) noexcept = default;

        inline reference_type operator*() const { return **m_order_it; }
        inline pointer_type operator->() const { return *m_order_it; }

        inline ordered_map_iterator_type& operator++() noexcept { ++m_order_it; return *this; }
        inline ordered_map_iterator_type operator++(int) noexcept {
            auto ret = *this;
            ++*this;
            return ret;
        }

        inline ordered_map_iterator_type& operator--() noexcept { --m_order_it; return *this; }
        inline ordered_map_iterator_type operator--(int) noexcept {
            auto ret = *this;
            --*this;
            return ret;
        }

        inline bool operator==(const ordered_map_iterator_type& other) const noexcept { return m_order_it == other.m_order_it; }
        inline bool operator!=(const ordered_map_iterator_type& other) const noexcept { return !operator==(other); }
    private:
        iterator_type m_order_it;

        friend class ordered_map<K, std::remove_const_t<std::remove_reference_t<V>>>;

        friend struct ordered_map_reverse_iterator<K, V>;
        friend struct ordered_map_reverse_iterator<K, V const>;

        friend struct ordered_map_iterator<K, std::remove_const_t<std::remove_reference_t<V>>>;
        friend struct ordered_map_iterator<K, V const>;
    };

    template<typename K, typename V>
    struct ordered_map_reverse_iterator {
    private:
        typedef ordered_map_iterator<K, V> ordered_map_iterator_type;
        typedef ordered_map_reverse_iterator<K, V> ordered_map_reverse_iterator_type;
    public:
        typedef typename ordered_map_iterator_type::value_type value_type;
        typedef typename ordered_map_iterator_type::reference_type reference_type;
        typedef typename ordered_map_iterator_type::pointer_type pointer_type;
    public:
        inline explicit ordered_map_reverse_iterator(ordered_map_iterator_type& it) noexcept : m_it(it) {}
        inline explicit ordered_map_reverse_iterator(ordered_map_iterator_type&& it) noexcept : m_it(std::move(it)) {}

        ordered_map_reverse_iterator(const ordered_map_reverse_iterator&) noexcept = default;
        ordered_map_reverse_iterator(ordered_map_reverse_iterator&&) noexcept = default;

        constexpr ordered_map_reverse_iterator& operator=(const ordered_map_reverse_iterator&) noexcept = default;
        constexpr ordered_map_reverse_iterator& operator=(ordered_map_reverse_iterator&&) noexcept = default;

        inline reference_type operator*() const { auto tmp = m_it; return (--tmp).operator *(); }
        inline pointer_type operator->() const { auto tmp = m_it; return (--tmp).operator ->(); }

        inline ordered_map_reverse_iterator_type& operator++() noexcept { --m_it; return *this; }
        inline ordered_map_reverse_iterator_type operator++(int) noexcept {
            auto ret = m_it--;
            return ordered_map_reverse_iterator_type(std::move(ret));
        }

        inline ordered_map_reverse_iterator_type& operator--() noexcept { ++m_it; return *this; }
        inline ordered_map_reverse_iterator_type operator--(int) noexcept {
            auto ret = m_it++;
            return ordered_map_reverse_iterator_type(std::move(ret));
        }

        inline bool operator==(const ordered_map_reverse_iterator_type& other) const noexcept { return m_it == other.m_it; }
        inline bool operator!=(const ordered_map_reverse_iterator_type& other) const noexcept { return m_it != other.m_it; }
    private:
        ordered_map_iterator_type m_it;
    };
}

#endif