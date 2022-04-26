#include <iostream>
#include <lox.hpp>

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << "Usage: lox [script]" << std::endl;
    return 64;
  } else if (argc == 2) {
    lox::Lox::runFile(argv[1]);
  } else {
    lox::Lox::runPrompt();
  }
  return 0;
}
