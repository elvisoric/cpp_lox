#ifndef LOX_HPP
#define LOX_HPP

#include <string>
#include <fstream>
#include <scanner.hpp>
#include <iostream>
#include <fmt/format.h>
#include <error_handler.hpp>

namespace lox {
struct Lox {
  static std::string getFileContent(const std::string& path) {
    std::ifstream file(path);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    return content;
  }

  static void run(const std::string& content) {
    lox::Scanner scanner{content};
    auto tokens = scanner.scanTokens();
    for (const auto& token : tokens) {
      std::cout << token << std::endl;
    }
  }

  static void runFile(const std::string& filename) {
    auto content = getFileContent(filename);
    run(content);
    if (hadError) std::exit(65);
  }

  static void runPrompt() {
    std::string line;
    std::cout << "> ";
    while (getline(std::cin, line)) {
      run(line);
      hadError = false;
      std::cout << "> ";
    }
  }
  static bool hadError;
};

bool Lox::hadError = false;
}  // namespace lox

#endif  // LOX_HPP
