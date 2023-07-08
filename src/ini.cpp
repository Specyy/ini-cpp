#include "ini.hpp"

#include <stdexcept>

namespace inicpp {
    INICPP ini_section& ini::find(const std::string& name) {
        {
            auto f = m_lookup_map.find(name);
            if (f != m_lookup_map.end()) return *f->second;
        }

        ini_section sec(name);
        sec.m_exists = false;
        m_sections.push_back(std::move(sec));
        auto it = --m_sections.end();
        m_lookup_map[it->get_name()] = it;
        return *it;
    }

    INICPP ini_section& ini::find(std::string&& name) {
        {
            auto f = m_lookup_map.find(name);
            if (f != m_lookup_map.end()) return *f->second;
        }

        ini_section sec(std::move(name));
        sec.m_exists = false;
        m_sections.push_back(std::move(sec));
        auto it = --m_sections.end();
        m_lookup_map[it->get_name()] = it;
        return *it;
    }

    INICPP ini_section const& ini::find(const std::string& name) const {
        {
            auto f = m_lookup_map.find(name);
            if (f != m_lookup_map.end()) return *f->second;
        }
        const_cast<ini*>(this)->push_back(ini_section(name));
        this->back().m_exists = false;
        return this->back();
    }

    INICPP ini_section const& ini::find(std::string&& name) const {
        {
            auto f = m_lookup_map.find(name);
            if (f != m_lookup_map.end()) return *f->second;
        }
        const_cast<ini*>(this)->push_back(ini_section(std::move(name)));
        this->back().m_exists = false;
        return this->back();
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

    INICPP template class detail::section_iterator<ini_section>;
    INICPP template class detail::section_iterator<ini_section const>;

    INICPP template class detail::reverse_iterator<detail::section_iterator<ini_section>>;
    INICPP template class detail::reverse_iterator<detail::section_iterator<ini_section const>>;
}