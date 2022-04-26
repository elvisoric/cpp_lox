#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include <string>
#include <fmt/format.h>
#include <iostream>

namespace lox {
inline void report(int line, const std::string& where,
                   const std::string& message) {
  std::cout << fmt::format("[line {} Error {}: {}", line, where, message)
            << std::endl;
}

inline void error(int line, const std::string& message) {
  report(line, "", message);
}

}  // namespace lox

#endif  // ERROR_HANDLER_HPP
