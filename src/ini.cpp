#include "ini.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include "parser_exception.hpp"

template<typename CharT, typename Traits>
typename std::basic_string_view<CharT, Traits>::size_type count(const std::basic_string_view<CharT, Traits> str, const std::basic_string_view<CharT, Traits> delim) noexcept {
    typedef typename std::basic_string_view<CharT, Traits>::size_type size_type;

    size_type count = 0;

    const size_type delim_len = delim.length();

    if (delim_len > 0) {
        for (size_type begin = str.find(delim); begin != std::basic_string_view<CharT, Traits>::npos; begin = str.find(delim, begin += delim_len)) {
            count++;
        }
    }

    return count;
}

template<typename CharT, typename Traits, typename Alloc>
inline typename std::basic_string_view<CharT, Traits>::size_type count(const std::basic_string<CharT, Traits, Alloc>& str, const std::basic_string_view<CharT, Traits> delim) {
    return count<CharT, Traits>(std::basic_string_view<CharT, Traits>(str.data(), str.length()), delim);
}

template<typename CharT, typename Traits, typename Alloc>
inline typename std::basic_string_view<CharT, Traits>::size_type count(const std::basic_string_view<CharT, Traits> str, const std::basic_string<CharT, Traits, Alloc>& delim) {
    return count<CharT, Traits>(str, std::basic_string_view<CharT, Traits>(delim.data(), delim.length()));
}

template<typename CharT, typename Traits, typename Alloc>
inline typename std::basic_string_view<CharT, Traits>::size_type count(const std::basic_string<CharT, Traits>& str, const std::basic_string<CharT, Traits, Alloc>& delim) {
    return count<CharT, Traits>(std::basic_string_view<CharT, Traits>(str.data(), str.length()), std::basic_string_view<CharT, Traits>(delim.data(), delim.length()));
}


template<typename CharT, typename Traits>
std::vector<std::basic_string_view<CharT, Traits>> split(const std::basic_string_view<CharT, Traits> str,
    const std::basic_string_view<CharT, Traits> delim) {
    typedef std::basic_string_view<CharT, Traits> string_view_type;
    typedef std::vector<string_view_type> vector_type;
    typedef typename std::vector<string_view_type>::size_type size_type;

    vector_type tokens;

    tokens.reserve(count(str, delim) + 1);

    const size_type delim_len = delim.length();

    size_type last = 0;
    if (delim_len > 0) {
        for (size_type begin = str.find(delim); begin != std::basic_string_view<CharT, Traits>::npos; begin += delim_len, last = begin, begin = str.find(delim, begin)) {
            tokens.emplace_back(&str[last], begin - last);
        }
    }
    tokens.emplace_back(&str[last], str.length() - last);

    return tokens;
}

template<typename CharT, typename Traits, typename Alloc>
inline std::vector<std::basic_string_view<CharT, Traits>> split(const std::basic_string<CharT, Traits, Alloc>& str,
    const std::basic_string<CharT, Traits, Alloc>& delim) {
    return split<CharT, Traits>(std::basic_string_view<CharT, Traits>(str.data(), str.length()), std::basic_string_view<CharT, Traits>(delim.data(), delim.length()));
}

template<typename CharT, typename Traits, typename Alloc>
inline std::vector<std::basic_string_view<CharT, Traits>> split(const std::basic_string<CharT, Traits, Alloc>& str,
    const std::basic_string_view<CharT, Traits> delim) {
    return split<CharT, Traits>(std::basic_string_view<CharT, Traits>(str.data(), str.length()), delim);
}

template<typename CharT, typename Traits, typename Alloc>
inline std::vector<std::basic_string_view<CharT, Traits>> split(const std::basic_string_view<CharT, Traits> str,
    const std::basic_string<CharT, Traits, Alloc>& delim) {
    return split<CharT, Traits>(str, std::basic_string_view<CharT, Traits>(delim.data(), delim.length()));
}

template<typename CharT, typename Traits>
constexpr inline bool starts_with(const std::basic_string_view<CharT, Traits> str, const std::basic_string_view<CharT, Traits> start) {
    return str.length() >= start.length() && Traits::compare(str.data(), start.data(), start.length()) == 0;
}

template<typename CharT, typename Traits, typename Alloc>
constexpr inline bool starts_with(const std::basic_string<CharT, Traits, Alloc>& str, const std::basic_string_view<CharT, Traits> start) {
    return starts_with(std::basic_string_view<CharT, Traits>(str.data(), str.length()), start);
}

template<typename CharT, typename Traits, typename Alloc>
constexpr inline bool starts_with(const std::basic_string_view<CharT, Traits> str, const std::basic_string<CharT, Traits, Alloc>& start) {
    return starts_with(str, std::basic_string_view<CharT, Traits>(start.data(), start.length()));
}

template<typename CharT, typename Traits, typename Alloc>
constexpr inline bool starts_with(const std::basic_string<CharT, Traits, Alloc>& str, const std::basic_string<CharT, Traits, Alloc>& start) {
    return starts_with(std::basic_string_view<CharT, Traits>(str.data(), str.length()), std::basic_string_view<CharT, Traits>(start.data(), start.length()));
}

template<typename CharT, typename Traits>
constexpr inline bool ends_with(const std::basic_string_view<CharT, Traits> str, const std::basic_string_view<CharT, Traits> end) {
    return str.length() >= end.length() && Traits::compare(str.data() + str.length() - end.length(), end.data(), end.length()) == 0;
}

template<typename CharT, typename Traits, typename Alloc>
constexpr inline bool ends_with(const std::basic_string<CharT, Traits, Alloc>& str, const std::basic_string_view<CharT, Traits> end) {
    return ends_with(std::basic_string_view<CharT, Traits>(str.data(), str.length()), end);
}

template<typename CharT, typename Traits, typename Alloc>
constexpr inline bool ends_with(const std::basic_string_view<CharT, Traits> str, const std::basic_string<CharT, Traits, Alloc>& end) {
    return ends_with(str, std::basic_string_view<CharT, Traits>(end.data(), end.length()));
}

template<typename CharT, typename Traits, typename Alloc>
constexpr inline bool ends_with(const std::basic_string<CharT, Traits, Alloc>& str, const std::basic_string<CharT, Traits, Alloc>& end) {
    return ends_with(std::basic_string_view<CharT, Traits>(str.data(), str.length()), std::basic_string_view<CharT, Traits>(end.data(), end.length()));
}

template<typename CharT, typename Traits, typename Alloc>
inline std::basic_string_view<CharT, Traits> ltrim(std::basic_string<CharT, Traits, Alloc> const& s) {
    auto cur = s.begin();
    for (; cur != s.end() && std::isspace(int(*cur)); ++cur);
    return std::basic_string_view<CharT, Traits>(&*cur, s.end() - cur);
}

template<typename CharT, typename Traits, typename Alloc>
inline std::basic_string_view<CharT, Traits> rtrim(std::basic_string<CharT, Traits, Alloc> const& s) {
    auto cur = s.rbegin();
    for (; cur != s.rend() && std::isspace(int(*cur)); ++cur);
    return std::basic_string_view<CharT, Traits>(&*s.begin(), s.rend() - cur);
}

template<typename CharT, typename Traits>
inline std::basic_string_view<CharT, Traits> ltrim(std::basic_string_view<CharT, Traits> s) {
    auto cur = s.begin();
    for (; cur != s.end() && std::isspace(int(*cur)); ++cur);
    return std::basic_string_view<CharT, Traits>(&*cur, s.end() - cur);
}

template<typename CharT, typename Traits>
inline std::basic_string_view<CharT, Traits> rtrim(std::basic_string_view<CharT, Traits> s) {
    auto cur = s.rbegin();
    for (; cur != s.rend() && std::isspace(int(*cur)); ++cur);
    return std::basic_string_view<CharT, Traits>(&*s.begin(), s.rend() - cur);
}

template<typename CharT, typename Traits>
inline std::basic_string_view<CharT, Traits> trim(std::basic_string_view<CharT, Traits> s) {
    s = ltrim(s);
    s = rtrim(s);
    return s;
}

template<typename CharT, typename Traits, typename Alloc>
inline std::basic_string_view<CharT, Traits> trim(std::basic_string<CharT, Traits, Alloc> const& s) {
    auto s1 = ltrim(s);
    s1 = rtrim(s1);
    return s1;
}

namespace inicpp {
    INICPP ini_section& ini::find(const std::string& name) {
        {
            auto f = m_lookup_map.find(name);
            if (f != m_lookup_map.end()) return *f->second;
        }

        this->push_back(ini_section(name));
        auto last = --end();
        last->m_exists = false;
        return *last;
    }

    INICPP ini_section& ini::find(std::string&& name) {
        {
            auto f = m_lookup_map.find(name);
            if (f != m_lookup_map.end()) return *f->second;
        }

        this->push_back(ini_section(name));
        auto last = --end();
        last->m_exists = false;
        return *last;
    }

    INICPP ini_section const& ini::find(const std::string& name) const {
        {
            auto f = m_lookup_map.find(name);
            if (f != m_lookup_map.end()) return *f->second;
        }
        const_cast<ini*>(this)->push_back(ini_section(name));
        auto last = --end();
        last->m_exists = false;
        return *last;
    }

    INICPP ini_section const& ini::find(std::string&& name) const {
        {
            auto f = m_lookup_map.find(name);
            if (f != m_lookup_map.end()) return *f->second;
        }
        const_cast<ini*>(this)->push_back(ini_section(std::move(name)));
        auto last = --end();
        last->m_exists = false;
        return *last;
    }


    INICPP bool ini::contains(const std::string& name) const {
        auto f = m_lookup_map.find(name);
        return f != m_lookup_map.end() && (*f->second);
    }

    INICPP ini_section& ini::insert(const_iterator pos, ini_section const& sec) {
        auto f = m_lookup_map.find(sec.get_name());
        if (f != m_lookup_map.end()) {
            *f->second = sec;
        } else {
            auto i = m_sections.insert(pos.m_cur, sec);
            f = m_lookup_map.insert({ i->get_name(), i }).first;
        }

        f->second->m_exists = true;
        f->second->m_ini = this;
        return *f->second;
    }

    INICPP ini_section& ini::insert(const_iterator pos, ini_section&& sec) {
        auto f = m_lookup_map.find(sec.get_name());
        if (f != m_lookup_map.end()) {
            *f->second = std::move(sec);
        } else {
            auto i = m_sections.insert(pos.m_cur, std::move(sec));
            f = m_lookup_map.insert({ i->get_name(), i }).first;
        }

        f->second->m_exists = true;
        f->second->m_ini = this;
        return *f->second;
    }

    INICPP void ini::read(std::istream& in) {
        // temporary objects are removed on read
        for (auto b = this->m_sections.begin(); b != this->m_sections.end();) {
            if (!b->m_exists) {
                b->m_exists = true;
                b = erase(const_iterator(b, m_sections)).m_cur;
            } else { b->m_exists = false; ++b; }
        }

        if (!in.eof()) {
            std::string line;
            size_t line_number = 0;
            bool read_sec = true;
            std::string_view trimmed_line;
            while (in) {
                if (read_sec) {
                    if (!std::getline(in, line)) return;
                    line_number++;
                    trimmed_line = trim(line);
                }

                if (trimmed_line.length() == 0) continue;
                if (trimmed_line[0] == '[') {
                    std::size_t comment_handle_pos = std::string_view::npos;

                    for (std::string const& comment_handle : m_comment_handles) {
                        std::size_t const old_pos = comment_handle_pos;
                        comment_handle_pos = std::min(trimmed_line.find(comment_handle, 1), old_pos);
                    }

                    std::size_t const end_section_pos = trimmed_line.find(']', 1);
                    std::string_view name = trim(trimmed_line.substr(1, std::min(comment_handle_pos, end_section_pos) - 1));

                    if (name.length() == 0) {
                        // error, expected section name
                        in.setstate(std::ios_base::failbit);
                        throw parser_exception(std::to_string(line_number) + ":1 Expected valid section name");
                        return;
                    } else if (end_section_pos > comment_handle_pos || end_section_pos == std::string_view::npos) {
                        // error, expected ']' to end section name
                        in.setstate(std::ios_base::failbit);
                        throw parser_exception(std::to_string(line_number) + " Expected ']' before end of line");
                        return;
                    }

                    ini_section& sec = (*this)[std::string(name)];
                    sec.m_exists = true;
                    sec.m_ini = this;

                    if (end_section_pos != trimmed_line.length() - 1) {
                        size_t next_valid_pos = end_section_pos + 1;
                        for (;next_valid_pos < trimmed_line.length() && std::isspace(trimmed_line[next_valid_pos]); next_valid_pos++);
                        if (next_valid_pos != comment_handle_pos) {
                            // error, unexpected character `trimmed_line[next_valid_pos]`
                            throw parser_exception(std::to_string(line_number) + ":" + std::to_string(next_valid_pos + 1) + " Unexpected character");
                            in.setstate(std::ios_base::failbit);
                            return;
                        }
                    }

                    read_sec = false;
                    for (line_number++; std::getline(in, line); line_number++) {
                        trimmed_line = trim(line);
                        if (trimmed_line.length() == 0) continue;
                        if (trimmed_line[0] == '[') break;
                        comment_handle_pos = std::string_view::npos;

                        for (std::string const& comment_handle : m_comment_handles) {
                            std::size_t const old_pos = comment_handle_pos;
                            comment_handle_pos = std::min(trimmed_line.find(comment_handle, 1), old_pos);
                        }
                        std::string_view accessible_line = trimmed_line.substr(0, comment_handle_pos);
                        auto delim_pos = accessible_line.find(this->m_delim);
                        if (delim_pos == std::string_view::npos) {
                            // error, expected delimeter
                            in.setstate(std::ios_base::failbit);
                            throw parser_exception(std::to_string(line_number) + " Expected delimeter before end of line");
                            return;
                        }
                        sec[std::string(trim(accessible_line.substr(0, delim_pos)))].set_value(std::string(trim(accessible_line.substr(delim_pos + 1))));
                    }
                } else {
                    bool found = false;
                    for (auto const& comment_handle : m_comment_handles) {
                        if (found = starts_with(trimmed_line, comment_handle)) break;
                    }
                    if (!found) {
                        // error, unexpected character `trimmed_line[0]`
                        in.setstate(std::ios_base::failbit);
                        throw parser_exception(std::to_string(line_number) + ":1 Unexpected character");
                        return;
                    }
                }
            }
        }
    }

    INICPP void ini::write(std::ostream& out) const {
        for (auto const& section : *this) {
            out << '[' << section.get_name() << "]\n";
            for (auto const& value : section) {
                out << value.first << m_delim << value.second.get_value() << '\n';
            }
            out << '\n';
        }
    }
}