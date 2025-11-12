#pragma once

#include <cstddef>
#include <variant>
#include <string>


namespace super {

    struct uninitialised_t {

        constexpr bool operator==(const uninitialised_t&) const {

            return true;
        }

        constexpr bool operator!=(const uninitialised_t&) const {

            return false;
        }
    };

    class object {

        private:

            using var = std::variant<

                bool,
                double,
                std::string,
                std::nullptr_t,
                uninitialised_t
            >;

            var obj;

        public:

            object();
            object(std::nullptr_t);
            object(bool);
            object(double);
            object(const std::string&);
            object(const char*);
            object(uninitialised_t);

            object(const object&) = default;
            object(object&&) = default;
            object& operator=(const object&) = default;
            object& operator=(object&&) = default;

            ~object() = default;

            bool is_uninitialised(void) const;
            bool is_null(void) const;
            bool is_bool(void) const;
            bool is_double(void) const;
            bool is_string(void) const;

            bool as_bool(void) const;
            double as_double(void) const;
            std::string as_string(void) const;

            std::string to_string(void) const;

            bool operator==(const object&) const;
            bool operator!=(const object&) const;
    };
}






