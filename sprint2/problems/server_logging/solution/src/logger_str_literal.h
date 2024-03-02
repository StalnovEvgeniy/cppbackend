#ifndef LOGGER_STR_LITERAL_H
#define LOGGER_STR_LITERAL_H

namespace logger_utils {

struct LogStrKey {
    LogStrKey() = delete;
    constexpr static auto timestamp{"timestamp"};
    constexpr static auto message{"message"};
    constexpr static auto data{"data"};
    constexpr static auto port{"port"};
    constexpr static auto address{"address"};
    constexpr static auto ip{"ip"};
    constexpr static auto uri{"URI"};
    constexpr static auto method{"method"};
    constexpr static auto response_time{"response_time"};
    constexpr static auto code{"code"};
    constexpr static auto exception{"exception"};
    constexpr static auto content_type{"content_type"};
    constexpr static auto text{"text"};
    constexpr static auto where{"where"};
};

}

#endif // LOGGER_STR_LITERAL_H
