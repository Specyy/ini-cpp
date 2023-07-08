#include "ini_section.hpp"
#include "ini.hpp"

namespace inicpp {
    INICPP void ini_section::set_name(const std::string& name) {
        if (this->m_name != name) {
            if (this->m_ini) {
                if (this->m_ini->contains(name)) throw std::invalid_argument("ini_section::set_name");

                auto pos = this->m_ini->m_lookup_map.find(this->m_name);
                this->m_ini->m_lookup_map[this->m_name = name] = pos->second;
                this->m_ini->m_lookup_map.erase(pos);
            } else { this->m_name = name; }
        }
    }

    INICPP void ini_section::set_name(std::string&& name) {
        if (this->m_name != name) {
            if (this->m_ini) {
                if (this->m_ini->contains(name)) throw std::invalid_argument("ini_section::set_name");

                auto pos = this->m_ini->m_lookup_map.find(this->m_name);
                this->m_ini->m_lookup_map[this->m_name = std::move(name)] = pos->second;
                this->m_ini->m_lookup_map.erase(pos);
            } else { this->m_name = std::move(name); }
        }
    }

    INICPP const ini_value& ini_section::find(const std::string& key) const {
        {
            auto f = m_data.find(key);
            if (f != m_data.end()) return f->second;
        }
        const_cast<ini_section*>(this)->push_back({ key, ini_value() });
        return this->back().second;
    }

    INICPP const ini_value& ini_section::find(std::string&& key) const {
        {
            auto f = m_data.find(key);
            if (f != m_data.end()) return f->second;
        }
        const_cast<ini_section*>(this)->push_back({ std::move(key), ini_value() });
        return this->back().second;
    }

    INICPP template class detail::key_iterator<std::string, ini_value>;
    INICPP template class detail::key_iterator<std::string, const ini_value>;

    INICPP template class detail::reverse_iterator<detail::key_iterator<std::string, ini_value>>;
    INICPP template class detail::reverse_iterator<detail::key_iterator<std::string, const ini_value>>;

    INICPP template class detail::ordered_map<std::string, ini_value>;
}