// C++ translation of core/src/avm1/globals/date.rs
//! AVM1 Date object - ECMA-262 Date implementation

#ifndef RUFFLE_CORE_AVM1_GLOBALS_DATE_H
#define RUFFLE_CORE_AVM1_GLOBALS_DATE_H

#include <memory>
#include <cstdint>
#include <optional>
#include <cmath>
#include <chrono>
#include <format>
#include <string>
#include <cell>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC> class UpdateContext;
    template<typename GC> class Mutation;
    template<typename GC> class Gc;
    template<typename T> class AvmString;

    namespace avm1 {
        template<typename T> class Activation;
        template<typename T> class Object;
        template<typename T> class Error;
        template<typename T> class Value;
        template<typename T> class NativeObject;

        namespace property_decl {
            template<typename T> class DeclContext;
            struct StaticDeclarations;
            template<typename T> class SystemClass;
        }
    }

    namespace locale {
        int64_t get_current_date_time();
        int32_t get_timezone();
    }
}
}

namespace ruffle {
namespace core {
namespace avm1 {
namespace globals {

/// Date and time, represented by milliseconds since epoch
class Date {
public:
    static constexpr int32_t MS_PER_SECOND = 1000;
    static constexpr int32_t SECONDS_PER_MINUTE = 60;
    static constexpr int32_t MS_PER_MINUTE = MS_PER_SECOND * SECONDS_PER_MINUTE;
    static constexpr int32_t MINUTES_PER_HOUR = 60;
    static constexpr int32_t MS_PER_HOUR = MS_PER_MINUTE * MINUTES_PER_HOUR;
    static constexpr int32_t HOURS_PER_DAY = 24;
    static constexpr int32_t MS_PER_DAY = MS_PER_HOUR * HOURS_PER_DAY;

    static constexpr std::array<std::array<uint16_t, 13>, 2> MONTH_OFFSETS = {{
        {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
        {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366} // Leap year
    }};

    Date() : time_(std::nan("")) {}
    explicit Date(double time) : time_(time) {}

    /// Create from specified date and time
    static Date new_(
        double year,
        double month,
        double date,
        double hour,
        double minute,
        double second,
        double millisecond
    );

    /// Create from current date and time
    static Date now();

    /// Get milliseconds since epoch
    double time() const { return time_; }

    /// Check if valid
    bool is_valid() const { return std::isfinite(time_); }

    /// ECMA-262 Day - Get days since epoch
    double day() const {
        return std::floor(time_ / static_cast<double>(MS_PER_DAY));
    }

    /// ECMA-262 TimeWithinDay - Get milliseconds within day
    double time_within_day(uint8_t swf_version) const {
        if (swf_version > 7) {
            return std::fmod(time_, static_cast<double>(MS_PER_DAY));
        } else {
            return std::fmod(time_, static_cast<double>(MS_PER_DAY));
        }
    }

    /// ECMA-262 DayFromYear - Return days passed since epoch to January 1st of year
    static double day_from_year(double year) {
        return (365.0 * (year - 1970.0))
            + std::floor((year - 1969.0) / 4.0)
            - std::floor((year - 1901.0) / 100.0)
            + std::floor((year - 1601.0) / 400.0);
    }

    /// ECMA-262 TimeFromYear - Return January 1st of year
    static Date from_year(int32_t year) {
        return Date(static_cast<double>(MS_PER_DAY) * day_from_year(static_cast<double>(year)));
    }

    /// ECMA-262 YearFromTime - Get year
    int32_t year() const;

    /// Determine whether or not year is a leap year
    static constexpr bool is_leap_year(int32_t year) {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }

    /// ECMA-262 InLeapYear
    bool in_leap_year() const {
        return is_leap_year(year());
    }

    /// ECMA-262 MonthFromTime - Get month (0-11)
    int32_t month() const;

    /// ECMA-262 DayWithinYear - Get days within year (0-365)
    int32_t day_within_year() const {
        return static_cast<int32_t>(day() - day_from_year(static_cast<double>(year())));
    }

    /// ECMA-262 DateFromTime - Get days within month (1-31)
    int32_t date() const;

    /// ECMA-262 WeekDay - Get days within week (0-6)
    int32_t week_day() const {
        auto result = static_cast<int32_t>(std::fmod(day() + 4.0, 7.0));
        return result < 0 ? result + 7 : result;
    }

    /// ECMA-262 LocalTZA - Get local timezone adjustment in milliseconds
    int32_t local_tza(bool /*is_utc*/) const {
        // TODO: Honor is_utc flag
        return locale::get_timezone() * MS_PER_SECOND;
    }

    /// ECMA-262 LocalTime - Convert from UTC to local timezone
    Date local() const {
        return Date(time_ + static_cast<double>(local_tza(true)));
    }

    /// ECMA-262 UTC - Convert from local timezone to UTC
    Date utc() const {
        return Date(time_ - static_cast<double>(local_tza(false)));
    }

    /// Get timezone offset in minutes
    double timezone_offset() const {
        return (time_ - local().time_) / static_cast<double>(MS_PER_MINUTE);
    }

    /// ECMA-262 HourFromTime - Get hours (0-23)
    int32_t hours() const;

    /// ECMA-262 MinFromTime - Get minutes (0-59)
    int32_t minutes() const;

    /// ECMA-262 SecFromTime - Get seconds (0-59)
    int32_t seconds() const;

    /// ECMA-262 msFromTime - Get milliseconds (0-999)
    int32_t milliseconds() const;

    /// ECMA-262 MakeTime
    static double make_time(
        double hours,
        double minutes,
        double seconds,
        double milliseconds
    );

    /// ECMA-262 MakeDay
    static double make_day(double year, double month, double date);

    /// ECMA-262 MakeDate - Create from days since epoch and milliseconds within day
    static Date make_date(double day, double time) {
        return Date(day * static_cast<double>(MS_PER_DAY) + time);
    }

    /// ECMA-262 TimeClip
    Date clip() const {
        constexpr double LIMIT = 100'000'000.0 * static_cast<double>(MS_PER_DAY);
        if (!is_valid() || std::abs(time_) > LIMIT) {
            return invalid();
        }
        return Date(std::floor(time_));
    }

    /// Get invalid date
    static constexpr Date invalid() {
        return Date(std::nan(""));
    }

private:
    double time_;
};

/// Helper for remainder with Euclidean division
inline int32_t rem_euclid_i32(double lhs, int32_t rhs) {
    auto result = static_cast<int32_t>(std::fmod(lhs, static_cast<double>(rhs)));
    return result < 0 ? result + rhs : result;
}

// Inline method implementations

inline Date Date::new_(
    double year,
    double month,
    double date,
    double hour,
    double minute,
    double second,
    double millisecond
) {
    if (year < 100.0) {
        year += 1900.0;
    }
    return make_date(
        make_day(year, month, date),
        make_time(hour, minute, second, millisecond)
    );
}

inline Date Date::now() {
    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
    return Date(static_cast<double>(millis));
}

inline int32_t Date::year() const {
    auto d = day();
    // Binary search to find the largest year such that from_year(year) <= *this
    int32_t low = static_cast<int32_t>(std::floor(d / (d < 0 ? 365.0 : 366.0))) + 1970;
    int32_t high = static_cast<int32_t>(std::ceil(d / (d < 0 ? 366.0 : 365.0))) + 1970;

    while (low < high) {
        int32_t pivot = static_cast<int32_t>((static_cast<double>(low) + static_cast<double>(high)) / 2.0);
        if (from_year(pivot) <= *this) {
            if (from_year(pivot + 1) > *this) {
                return pivot;
            }
            low = pivot + 1;
        } else {
            high = pivot - 1;
        }
    }
    return low;
}

inline int32_t Date::month() const {
    auto day = day_within_year();
    auto in_leap = in_leap_year();

    for (int32_t i = 0; i < 11; ++i) {
        if (day < MONTH_OFFSETS[in_leap ? 1 : 0][static_cast<size_t>(i + 1)]) {
            return i;
        }
    }
    return 11;
}

inline int32_t Date::date() const {
    auto month = this->month();
    auto month_offset = MONTH_OFFSETS[in_leap_year() ? 1 : 0][static_cast<size_t>(month)];
    return day_within_year() - static_cast<int32_t>(month_offset) + 1;
}

inline int32_t Date::hours() const {
    return rem_euclid_i32(
        std::floor((time_ + 0.5) / static_cast<double>(MS_PER_HOUR)),
        HOURS_PER_DAY
    );
}

inline int32_t Date::minutes() const {
    return rem_euclid_i32(
        std::floor(time_ / static_cast<double>(MS_PER_MINUTE)),
        MINUTES_PER_HOUR
    );
}

inline int32_t Date::seconds() const {
    return rem_euclid_i32(
        std::floor(time_ / static_cast<double>(MS_PER_SECOND)),
        SECONDS_PER_MINUTE
    );
}

inline int32_t Date::milliseconds() const {
    return rem_euclid_i32(time_, static_cast<double>(MS_PER_SECOND));
}

inline double Date::make_time(
    double hours,
    double minutes,
    double seconds,
    double milliseconds
) {
    // TODO: Round towards zero instead of floor
    hours = std::floor(hours);
    minutes = std::floor(minutes);
    seconds = std::floor(seconds);
    milliseconds = std::floor(milliseconds);

    return hours * static_cast<double>(MS_PER_HOUR)
        + minutes * static_cast<double>(MS_PER_MINUTE)
        + seconds * static_cast<double>(MS_PER_SECOND)
        + milliseconds;
}

inline double Date::day_from_month(double year, double month) {
    auto y = static_cast<int32_t>(year);
    auto m = static_cast<int32_t>(std::floor(month));

    if (m < 0 || m >= 12) {
        return std::nan("");
    }

    auto is_leap = is_leap_year(y);
    auto month_offset = MONTH_OFFSETS[is_leap ? 1 : 0][static_cast<size_t>(m)];
    return day_from_year(year) + static_cast<double>(month_offset);
}

inline double Date::make_day(double year, double month, double date) {
    // TODO: Round towards zero instead of floor
    year = std::floor(year);
    month = std::floor(month);
    date = std::floor(date);

    year += std::floor(month / 12.0);
    month = std::fmod(month, 12.0);
    if (month < 0) month += 12.0;

    return day_from_month(year, month) + date - 1.0;
}

/// Format Date as string
inline std::string date_to_string(const Date& date) {
    if (!date.is_valid()) {
        return "Invalid Date";
    }

    static constexpr const char* DAYS_OF_WEEK[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    static constexpr const char* MONTHS[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    auto timezone_offset = static_cast<int32_t>(-date.timezone_offset());
    auto sign = timezone_offset < 0 ? '-' : '+';
    auto abs_offset = std::abs(timezone_offset);
    auto hours = abs_offset / Date::MINUTES_PER_HOUR;
    auto mins = abs_offset % Date::MINUTES_PER_HOUR;

    return std::format(
        "{} {} {} {:02}:{:02}:{:02} GMT{}{:02}{:02} {}",
        DAYS_OF_WEEK[date.week_day()],
        MONTHS[date.month()],
        date.date(),
        date.hours(),
        date.minutes(),
        date.seconds(),
        sign,
        hours,
        mins,
        date.year()
    );
}

// Method indices for property dispatch
namespace date_method {
    constexpr uint16_t GET_FULL_YEAR = 0;
    constexpr uint16_t GET_YEAR = 1;
    constexpr uint16_t GET_MONTH = 2;
    constexpr uint16_t GET_DATE = 3;
    constexpr uint16_t GET_DAY = 4;
    constexpr uint16_t GET_HOURS = 5;
    constexpr uint16_t GET_MINUTES = 6;
    constexpr uint16_t GET_SECONDS = 7;
    constexpr uint16_t GET_MILLISECONDS = 8;
    constexpr uint16_t SET_FULL_YEAR = 9;
    constexpr uint16_t SET_MONTH = 10;
    constexpr uint16_t SET_DATE = 11;
    constexpr uint16_t SET_HOURS = 12;
    constexpr uint16_t SET_MINUTES = 13;
    constexpr uint16_t SET_SECONDS = 14;
    constexpr uint16_t SET_MILLISECONDS = 15;
    constexpr uint16_t GET_TIME = 16;
    constexpr uint16_t SET_TIME = 17;
    constexpr uint16_t GET_TIMEZONE_OFFSET = 18;
    constexpr uint16_t TO_STRING = 19;
    constexpr uint16_t SET_YEAR = 20;
    constexpr uint16_t CONSTRUCTOR = 256;

    constexpr uint16_t UTC = 257;
    constexpr uint16_t GET_UTC_FULL_YEAR = 128 + GET_FULL_YEAR;
    constexpr uint16_t GET_UTC_YEAR = 128 + GET_YEAR;
    constexpr uint16_t GET_UTC_MONTH = 128 + GET_MONTH;
    constexpr uint16_t GET_UTC_DATE = 128 + GET_DATE;
    constexpr uint16_t GET_UTC_DAY = 128 + GET_DAY;
    constexpr uint16_t GET_UTC_HOURS = 128 + GET_HOURS;
    constexpr uint16_t GET_UTC_MINUTES = 128 + GET_MINUTES;
    constexpr uint16_t GET_UTC_SECONDS = 128 + GET_SECONDS;
    constexpr uint16_t GET_UTC_MILLISECONDS = 128 + GET_MILLISECONDS;
    constexpr uint16_t SET_UTC_FULL_YEAR = 128 + SET_FULL_YEAR;
    constexpr uint16_t SET_UTC_MONTH = 128 + SET_MONTH;
    constexpr uint16_t SET_UTC_DATE = 128 + SET_DATE;
    constexpr uint16_t SET_UTC_HOURS = 128 + SET_HOURS;
    constexpr uint16_t SET_UTC_MINUTES = 128 + SET_MINUTES;
    constexpr uint16_t SET_UTC_SECONDS = 128 + SET_SECONDS;
    constexpr uint16_t SET_UTC_MILLISECONDS = 128 + SET_MILLISECONDS;
}

/// Create the Date class
///
/// @param context The declaration context
/// @param super_proto The superclass prototype
/// @return SystemClass for Date
template<typename GC>
property_decl::SystemClass<GC> create_date_class(
    property_decl::DeclContext<GC>* context,
    Object<GC> super_proto
);

/// Date constructor
///
/// @param activation The current activation
/// @param this_obj The this object
/// @param args Constructor arguments
/// @return Result value or error
template<typename GC>
Result<Value<GC>, Error<GC>> date_constructor(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::span<double>& args
);

/// Date() invoked without new returns current date and time as a string
///
/// @param activation The current activation
/// @param this_obj The this object
/// @param args Method arguments
/// @return Result value or error
template<typename GC>
Result<Value<GC>, Error<GC>> date_function(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::span<Value<GC>>& args
);

/// ECMA-262 Date.UTC
///
/// @param args UTC arguments
/// @return Result value or error
template<typename GC>
Result<Value<GC>, Error<GC>> date_utc(const std::span<double>& args);

/// Method handler for Date
///
/// @param activation The current activation
/// @param this_obj The this object
/// @param args Method arguments
/// @param index The method index
/// @return Result value or error
template<typename GC>
Result<Value<GC>, Error<GC>> date_method_handler(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::span<Value<GC>>& args,
    uint16_t index
);

// Template method implementations

template<typename GC>
Result<Value<GC>, Error<GC>> date_constructor(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::span<double>& args
) {
    Date date;

    if (args.empty()) {
        date = Date::now();
        if (activation->swf_version() > 7) {
            date = Date(std::round(date.time()));
        }
    } else if (args.size() == 1) {
        date = Date(args[0]);
    } else {
        double year = args[0];
        double month = args[1];
        double d = args.size() > 2 ? args[2] : 1.0;
        double hour = args.size() > 3 ? args[3] : 0.0;
        double minute = args.size() > 4 ? args[4] : 0.0;
        double second = args.size() > 5 ? args[5] : 0.0;
        double millisecond = args.size() > 6 ? args[6] : 0.0;

        date = Date::new_(year, month, d, hour, minute, second, millisecond).utc();
    }

    this_obj.set_native(
        activation->gc(),
        NativeObject<GC>::date(Gc<GC, std::cell::Cell<Date>>::create(
            activation->gc(),
            std::cell::Cell<Date>(date)
        ))
    );

    return Ok(Value<GC>::from_object(this_obj));
}

template<typename GC>
Result<Value<GC>, Error<GC>> date_function(
    Activation<GC>* activation,
    Object<GC> /*this_obj*/,
    const std::span<Value<GC>>& /*args*/
) {
    auto date_str = date_to_string(Date::now().local());
    return Ok(Value<GC>::from_string(AvmString<GC>::new_utf8(activation->gc(), date_str)));
}

template<typename GC>
Result<Value<GC>, Error<GC>> date_utc(const std::span<double>& args) {
    if (args.size() <= 1) {
        return Ok(Value<GC>::undefined());
    }

    double year = args[0];
    double month = args[1];
    double d = args.size() > 2 ? args[2] : 1.0;
    double hour = args.size() > 3 ? args[3] : 0.0;
    double minute = args.size() > 4 ? args[4] : 0.0;
    double second = args.size() > 5 ? args[5] : 0.0;
    double millisecond = args.size() > 6 ? args[6] : 0.0;

    auto date = Date::new_(year, month, d, hour, minute, second, millisecond);
    return Ok(Value<GC>::from_f64(date.time()));
}

template<typename GC>
Result<Value<GC>, Error<GC>> date_method_handler(
    Activation<GC>* activation,
    Object<GC> this_obj,
    const std::span<Value<GC>>& args,
    uint16_t index
) {
    using namespace date_method;

    // Handle constructor and UTC
    if (index == CONSTRUCTOR) {
        std::vector<double> double_args;
        for (size_t i = 0; i < std::min(args.size(), size_t(7)); ++i) {
            if (args[i] == Value<GC>::undefined()) break;
            auto result = args[i].coerce_to_f64(activation);
            if (result.is_ok()) {
                double_args.push_back(result.unwrap());
            }
        }
        return date_constructor(activation, this_obj, double_args);
    }

    if (index == UTC) {
        std::vector<double> double_args;
        for (size_t i = 0; i < std::min(args.size(), size_t(7)); ++i) {
            if (args[i] == Value<GC>::undefined()) break;
            auto result = args[i].coerce_to_f64(activation);
            if (result.is_ok()) {
                double_args.push_back(result.unwrap());
            }
        }
        return date_utc<GC>(double_args);
    }

    // Get the date from the object
    auto native = this_obj.native();
    if (!native.is_date()) {
        return Ok(Value<GC>::undefined());
    }

    auto date_ref = native.date();
    auto date = date_ref.get().get();

    // Handle getTime, setTime, getTimezoneOffset directly
    if (index == GET_TIME) {
        return Ok(Value<GC>::from_f64(date.time()));
    }

    if (index == SET_TIME) {
        double timestamp = args.empty() ? std::nan("") : args[0].coerce_to_f64(activation).unwrap_or(std::nan(""));
        auto new_date = Date(timestamp).clip();
        date_ref.get().set(new_date);
        return Ok(Value<GC>::from_f64(new_date.time()));
    }

    if (index == GET_TIMEZONE_OFFSET) {
        return Ok(Value<GC>::from_f64(date.timezone_offset()));
    }

    // Map method index to local counterpart
    bool is_utc = index >= 128;
    if (is_utc) {
        index -= 128;
    }

    // Return NaN for invalid dates on getters
    bool is_get = (index >= GET_FULL_YEAR && index <= GET_MILLISECONDS)
        || index == GET_TIME || index == GET_TIMEZONE_OFFSET;

    if (is_get && std::isnan(date.time())) {
        return Ok(Value<GC>::from_f64(std::nan("")));
    }

    // Handle setYear() using setFullYear()
    bool is_set_year = index == SET_YEAR;
    if (is_set_year) {
        index = SET_FULL_YEAR;
    }

    auto get_arg = [&](uint16_t i) -> std::optional<double> {
        if (i < index || i - index >= args.size()) {
            return std::nullopt;
        }
        auto result = args[i - index].coerce_to_f64(activation);
        return result.is_ok() ? std::optional(result.unwrap()) : std::optional(std::nan(""));
    };

    auto current_date = is_utc ? date : date.local();

    auto set_date = [&](double day, double time) -> double {
        auto new_date = Date::make_date(day, time);
        if (!is_utc) {
            new_date = new_date.utc();
        }
        new_date = new_date.clip();
        date_ref.get().set(new_date);
        return new_date.time();
    };

    switch (index) {
        case GET_FULL_YEAR:
            return Ok(Value<GC>::from_f64(static_cast<double>(current_date.year())));

        case GET_YEAR:
            return Ok(Value<GC>::from_f64(static_cast<double>(current_date.year() - 1900)));

        case GET_MONTH:
            return Ok(Value<GC>::from_f64(static_cast<double>(current_date.month())));

        case GET_DATE:
            return Ok(Value<GC>::from_f64(static_cast<double>(current_date.date())));

        case GET_DAY:
            return Ok(Value<GC>::from_f64(static_cast<double>(current_date.week_day())));

        case GET_HOURS:
            return Ok(Value<GC>::from_f64(static_cast<double>(current_date.hours())));

        case GET_MINUTES:
            return Ok(Value<GC>::from_f64(static_cast<double>(current_date.minutes())));

        case GET_SECONDS:
            return Ok(Value<GC>::from_f64(static_cast<double>(current_date.seconds())));

        case GET_MILLISECONDS:
            return Ok(Value<GC>::from_f64(static_cast<double>(current_date.milliseconds())));

        case SET_FULL_YEAR:
        case SET_MONTH:
        case SET_DATE: {
            auto year = get_arg(SET_FULL_YEAR).value_or(static_cast<double>(current_date.year()));
            if (is_set_year && year >= 0.0 && year <= 99.0) {
                year += 1900.0;
            }

            auto month = get_arg(SET_MONTH);
            if (index == SET_MONTH && !month.has_value()) {
                month = 0.0;
            }
            month = month.value_or(static_cast<double>(current_date.month()));

            auto d = get_arg(SET_DATE).value_or(static_cast<double>(current_date.date()));

            return Ok(Value<GC>::from_f64(set_date(
                Date::make_day(year, month, d),
                current_date.time_within_day(activation->swf_version())
            )));
        }

        case SET_HOURS:
        case SET_MINUTES:
        case SET_SECONDS:
        case SET_MILLISECONDS: {
            auto hours = get_arg(SET_HOURS).value_or(static_cast<double>(current_date.hours()));
            auto minutes = get_arg(SET_MINUTES).value_or(static_cast<double>(current_date.minutes()));
            auto seconds = get_arg(SET_SECONDS).value_or(static_cast<double>(current_date.seconds()));
            auto milliseconds = get_arg(SET_MILLISECONDS).value_or(static_cast<double>(current_date.milliseconds()));

            if (index == SET_MINUTES) {
                minutes = static_cast<double>(static_cast<int32_t>(minutes));
                seconds = static_cast<double>(static_cast<int32_t>(seconds));
                milliseconds = static_cast<double>(static_cast<int32_t>(milliseconds));
            }

            return Ok(Value<GC>::from_f64(set_date(
                current_date.day(),
                Date::make_time(hours, minutes, seconds, milliseconds)
            )));
        }

        case TO_STRING: {
            auto date_str = date_to_string(date);
            return Ok(Value<GC>::from_string(AvmString<GC>::new_utf8(activation->gc(), date_str)));
        }

        default:
            return Ok(Value<GC>::undefined());
    }
}

} // namespace globals
} // namespace avm1
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM1_GLOBALS_DATE_H
