#ifndef INICPP_ORDERED_MAP_H
#define INICPP_ORDERED_MAP_H 1

#include "detail/ordered_map_iterator.h"

namespace inicpp::detail {
    template<typename K, typename V>
    class ordered_map {
    public:
        typedef typename std::unordered_map<K, V>::key_type key_type;
        typedef typename std::unordered_map<K, V>::mapped_type mapped_type;
        typedef typename std::unordered_map<K, V>::value_type value_type;
        typedef typename std::unordered_map<K, V>::size_type size_type;
        typedef typename std::unordered_map<K, V>::difference_type difference_type;
        typedef typename std::unordered_map<K, V>::hasher hasher;
        typedef typename std::unordered_map<K, V>::key_equal key_equal;
        typedef typename std::unordered_map<K, V>::reference reference;
        typedef typename std::unordered_map<K, V>::const_reference const_reference;
        typedef typename std::unordered_map<K, V>::pointer pointer;
        typedef typename std::unordered_map<K, V>::const_pointer const_pointer;

        typedef ordered_map_iterator<K, V> iterator;
        typedef ordered_map_iterator<K, V const> const_iterator;

        typedef ordered_map_reverse_iterator<K, V> reverse_iterator;
        typedef ordered_map_reverse_iterator<K, V const> const_reverse_iterator;
    private:
        typedef ordered_map<K, V> ordered_map_type;
    public:
        ordered_map() noexcept = default;
        ordered_map(const ordered_map&) noexcept = default;
        ordered_map(ordered_map&&) noexcept = default;

        ordered_map& operator=(const ordered_map&) noexcept = default;
        ordered_map& operator=(ordered_map&&) noexcept = default;

        inline bool empty() const noexcept { return m_map.empty(); }
        inline size_type size() const noexcept { return m_map.size(); }
        inline size_type max_size() const noexcept { return m_map.max_size(); }
        inline void clear() noexcept { return m_map.clear(); }

        inline void push_front(const value_type& value) { insert(cbegin(), value); }
        inline void push_front(value_type&& value) { insert(cbegin(), std::move(value)); }

        inline void push_back(const value_type& value) { insert(cend(), value); }
        inline void push_back(value_type&& value) { insert(cend(), std::move(value)); }

        /**
         * @brief Inserts element(s) into the container. If the container doesn't already contain an element with an equivalent key,
         * the element is inserted at the desired position. If the element already exists within the container, its is relocated and has its value reassigned.
         * @param pos iterator before which the content will be inserted (pos may be the end() iterator)
         * @param value element value to insert
         * @return Returns an iterator to the inserted element
         */
        iterator insert(const_iterator pos, const value_type& value) {
            auto c = m_map.find(value.first);
            if (c == m_map.end()) {
                auto r = m_map.insert(value);
                auto i = m_order.insert(pos.m_order_it, r.first.operator->());
                return iterator(i);
            } else {
                for (auto b = m_order.begin(); b != m_order.end(); ++b) {
                    if (*b == c.operator->()) {
                        c->second = value.second;
                        auto i = m_order.insert(pos.m_order_it, c.operator->());
                        m_order.erase(b);
                        return iterator(i);
                    }
                }
            }
            throw std::runtime_error("ordered_map::insert() failed to insert value");
        }

        /**
         * @brief Inserts element(s) into the container. If the container doesn't already contain an element with an equivalent key,
         * the element is inserted at the desired position. If the element already exists within the container, its is relocated and has its value reassigned.
         * @param pos iterator before which the content will be inserted (pos may be the end() iterator)
         * @param value element value to insert
         * @return Returns an iterator to the inserted element
         */
        iterator insert(const_iterator pos, value_type&& value) {
            auto c = m_map.find(value.first);
            if (c == m_map.end()) {
                auto r = m_map.insert(std::move(value));
                auto i = m_order.insert(pos.m_order_it, r.first.operator->());
                return iterator(i);
            } else {
                for (auto b = m_order.begin(); b != m_order.end(); ++b) {
                    if (*b == c.operator->()) {
                        c->second = std::move(value.second);
                        auto i = m_order.insert(pos.m_order_it, c.operator->());
                        m_order.erase(b);
                        return iterator(i);
                    }
                }
            }
            throw std::runtime_error("ordered_map::insert() failed to insert value");
        }

        /**
         * @brief Erases the specified elements from the container.
         * @param pos iterator to the element to remove
         * @return Iterator following the last removed element.
         */
        inline iterator erase(const_iterator pos) {
            m_map.erase(pos->first);
            auto i = m_order.erase(pos.m_order_it);
            return iterator(i);
        }

        /**
         * @brief Erases the specified elements from the container.
         * @param pos iterator to the element to remove
         * @return Iterator following the last removed element.
         */
        inline iterator remove(const_iterator pos) { return erase(pos); }

        /**
         * @brief Erases the specified elements from the container.
         * @param key key value of the elements to remove
         * @return Number of elements removed (0 or 1).
         */
        inline size_type erase(const key_type& key) { return erase(cbegin(), key); }

        /**
         * @brief Erases the specified elements from the container.
         * @param key key value of the elements to remove
         * @return Number of elements removed (0 or 1).
         */
        inline size_type remove(const key_type& key) { return erase(key); }

        /**
         * @brief Erases the specified elements from the container.
         * @param hint hint as to where the key may be located with respect to the current order
         * @param key key value of the elements to remove
         * @return Number of elements removed (0 or 1).
         */
        size_type erase(const_iterator hint, const key_type& key) {
            auto look = m_map.find(key);
            if (look == m_map.cend()) { return 0; }
            for (const_iterator c = hint; c != cend(); ++c) {
                auto f = m_map.find(c->first);
                if (f == look) {
                    m_map.erase(f);
                    auto i = m_order.erase(c.m_order_it);
                    return 1;
                }
            }
            if (hint != cbegin()) {
                const_iterator c = --hint;
                do {
                    auto f = m_map.find(c->first);
                    if (f == look) {
                        m_map.erase(f);
                        auto i = m_order.erase(c.m_order_it);
                        return 1;
                    }
                    --c;
                } while (c != cbegin());
            }
            return 0;
        }

        /**
         * @brief Erases the specified elements from the container.
         * @param key key value of the elements to remove
         * @param hint hint as to where the key may be located with respect to the current order
         * @return Number of elements removed (0 or 1).
         */
        inline size_type remove(const_iterator hint, const key_type& key) { return erase(hint, key); }

        inline void swap(ordered_map& other) noexcept(noexcept(std::unordered_map<K, V>::swap)) { m_order.swap(other.m_order); m_map.swap(other.m_map); }

        /**
         * @brief Sorts the elements in ascending order. The order of equal elements is preserved. The first version uses operator< to compare the elements,
         * the second version uses the given comparison function comp. If an exception is thrown, the order of elements in *this is unspecified.
         */
        inline void sort() { return sort([](value_type const& a, value_type const& b) { return std::less<value_type>()(a, b); }); }

        /**
         * @brief Sorts the elements in ascending order. The order of equal elements is preserved. The first version uses operator< to compare the elements,
         * the second version uses the given comparison function comp. If an exception is thrown, the order of elements in *this is unspecified.
         * @tparam Compare bool cmp(value_type const& a, value_type const& b);
         * @param comp comparison function object (i.e. an object that satisfies the requirements of Compare) which returns ​true if the first argument is less than (i.e. is ordered before) the second.
         */
        template<class Compare>
        inline void sort(Compare comp) { m_order.sort([&comp](value_type* a, value_type* b) { return comp(*a, *b); }); }

        /**
         * @brief Reverses the order of the elements in the container. No references or iterators become invalidated.
         */
        inline void reverse() noexcept { m_order.reverse(); }

        inline iterator find(const key_type& key) { return find(cbegin(), key); }

        iterator find(const_iterator hint, const key_type& key) {
            auto look = m_map.find(key);
            if (look == m_map.end()) { return end(); }
            for (const_iterator c = hint; c != cend(); ++c) {
                auto f = m_map.find(c->first);
                if (f == look) {
                    return iterator(c.m_order_it);
                }
            }
            if (hint != cbegin()) {
                const_iterator c = --hint;
                do {
                    auto f = m_map.find(c->first);
                    if (f == look) {
                        return iterator(c.m_order_it);
                    }
                    --c;
                } while (c != cbegin());
            }
            return end();
        }

        inline const_iterator find(const key_type& key) const { return find(cbegin(), key); }

        const_iterator find(const_iterator hint, const key_type& key) const {
            auto look = m_map.find(key);
            if (look == m_map.end()) { return end(); }
            for (const_iterator c = hint; c != cend(); ++c) {
                auto f = m_map.find(c->first);
                if (f == look) {
                    return c;
                }
            }
            if (hint != cbegin()) {
                const_iterator c = --hint;
                do {
                    auto f = m_map.find(c->first);
                    if (f == look) {
                        return c;
                    }
                    --c;
                } while (c != cbegin());
            }
            return end();
        }

        mapped_type& at(const key_type& key) {
            auto f = find(key);
            if (f != end()) return f->second;
            throw std::out_of_range("ordered_map::at");
        }

        mapped_type const& at(const key_type& key) const {
            auto f = find(key);
            if (f != cend()) return f->second;
            throw std::out_of_range("ordered_map::at");
        }

        inline mapped_type& operator[](const key_type& key) {
            auto f = find(key);
            if (f != end()) return f->second;
            push_back(value_type(std::move(key), mapped_type()));
            return back();
        }

        inline mapped_type const& operator[](const key_type& key) const { return at(key); }

        inline mapped_type& operator[](key_type&& key) {
            auto f = find(key);
            if (f != end()) return f->second;
            push_back(value_type(std::move(key), mapped_type()));
            return back();
        }

        inline bool contains(const key_type& key) const { return find(key) != cend(); }

        inline mapped_type& back() noexcept { return m_order.back()->second; }
        inline mapped_type const& back() const noexcept { return m_order.back()->second; }

        inline mapped_type& front() noexcept { return m_order.front()->second; }
        inline mapped_type const& front() const noexcept { return m_order.front()->second; }

        inline iterator begin() noexcept { return iterator(m_order.begin()); }
        inline const_iterator begin() const noexcept { return const_iterator(m_order.begin()); }
        inline const_iterator cbegin() const noexcept { return const_iterator(m_order.cbegin()); }

        inline iterator end() noexcept { return iterator(m_order.end()); }
        inline const_iterator end() const noexcept { return const_iterator(m_order.end()); }
        inline const_iterator cend() const noexcept { return const_iterator(m_order.cend()); }

        inline reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        inline const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        inline const_reverse_iterator rcbegin() const noexcept { return const_reverse_iterator(cend()); }

        inline reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
        inline const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        inline const_reverse_iterator rcend() const noexcept { return const_reverse_iterator(cbegin()); }
    private:
        std::list<value_type*> m_order;
        std::unordered_map<K, V> m_map;

        friend struct ordered_map_iterator<K, V>;
        friend struct ordered_map_reverse_iterator<K, V>;
    };
}
#endif