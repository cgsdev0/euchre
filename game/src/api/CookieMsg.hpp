#include <cstdint>

#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class CookieMsgType : int;
}

namespace API {
    /**
     * Alternative to sending your cookie as a header.
     */


    /**
     * Alternative to sending your cookie as a header.
     */
    struct CookieMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::string session;
        CookieMsgType type = static_cast<CookieMsgType>(0);
    };
}
