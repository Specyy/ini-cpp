#include "ini_value.hpp"
#include "ini_section.hpp"

namespace inicpp {
    INICPP void ini_value::set_value(const std::string& data) {
        this->m_data = data;
        if (m_section) m_section->m_exists = true;
    }

    INICPP void ini_value::set_value(std::string&& data) noexcept {
        this->m_data = std::move(data);
        if (m_section) m_section->m_exists = true;
    }
}
