#ifndef LOX_SCANNER_HPP
#define LOX_SCANNER_HPP

#include <string>
#include <vector>
#include <fmt/format.h>
#include <error_handler.hpp>
#include <variant>
#include <unordered_map>

namespace lox {

enum class TokenType {
  // Single character tokens
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character token
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals
  IDENTIFIER,
  STRING,
  NUMBER,

  // Keywords
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  Eof
};

inline std::string to_string(TokenType type) {
  switch (type) {
    case TokenType::LEFT_PAREN:
      return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN:
      return "RIGHT_PAREN";
    case TokenType::LEFT_BRACE:
      return "LEFT_BRACE";
    case TokenType::RIGHT_BRACE:
      return "RIGHT_BRACE";
    case TokenType::COMMA:
      return "COMMA";
    case TokenType::DOT:
      return "DOT";
    case TokenType::MINUS:
      return "MINUS";
    case TokenType::PLUS:
      return "PLUS";
    case TokenType::SEMICOLON:
      return "SEMICOLON";
    case TokenType::SLASH:
      return "SLASH";
    case TokenType::STAR:
      return "STAR";
    case TokenType::BANG:
      return "BANG";
    case TokenType::BANG_EQUAL:
      return "BANG_EQUAL";
    case TokenType::EQUAL:
      return "EQUAL";
    case TokenType::EQUAL_EQUAL:
      return "EQUAL_EQUAL";
    case TokenType::GREATER:
      return "GREATER";
    case TokenType::GREATER_EQUAL:
      return "GREATER_EQUAL";
    case TokenType::LESS:
      return "LESS";
    case TokenType::LESS_EQUAL:
      return "LESS_EQUAL";
    case TokenType::IDENTIFIER:
      return "IDENTIFIER";
    case TokenType::STRING:
      return "STRING";
    case TokenType::NUMBER:
      return "NUMBER";
    case TokenType::AND:
      return "AND";
    case TokenType::CLASS:
      return "CLASS";
    case TokenType::ELSE:
      return "ELSE";
    case TokenType::FALSE:
      return "FALSE";
    case TokenType::FUN:
      return "FUN";
    case TokenType::FOR:
      return "FOR";
    case TokenType::IF:
      return "IF";
    case TokenType::NIL:
      return "NIL";
    case TokenType::OR:
      return "OR";
    case TokenType::PRINT:
      return "PRINT";
    case TokenType::RETURN:
      return "RETURN";
    case TokenType::SUPER:
      return "SUPER";
    case TokenType::THIS:
      return "THIS";
    case TokenType::TRUE:
      return "TRUE";
    case TokenType::VAR:
      return "VAR";
    case TokenType::WHILE:
      return "WHILE";
    case TokenType::Eof:
      return "Eof";
  }
  return "Unknown";
}

using Object = std::variant<std::string, double>;
// struct Object {};
//
// inline std::string to_string(const Object&) { return "Object"; }

struct Token {
  TokenType type;
  std::string lexeme;
  Object literal;
  int line;
  friend std::ostream& operator<<(std::ostream& os, const Token& token) {
    std::string literal = "Object";
    if (token.type == TokenType::STRING)
      literal = std::get<std::string>(token.literal);
    else if (token.type == TokenType::NUMBER)
      literal = std::to_string(std::get<double>(token.literal));
    return os << fmt::format("{} {} {}", to_string(token.type), token.lexeme,
                             literal);
  }
};

using Tokens = std::vector<Token>;

struct Scanner {
  Scanner(const std::string& source) : source_{source} {}
  Tokens scanTokens() {
    while (!isAtEnd()) {
      // We are at the beginning of the next
      // lexeme
      start = current;
      scanToken();
    }
    tokens_.push_back({TokenType::Eof, "", {}, line});
    return tokens_;
  }

  private:
  std::string source_;
  Tokens tokens_;
  int start{0};
  int current{0};
  int line{1};

  std::unordered_map<std::string, TokenType> keywords{
      {"and", TokenType::AND},       {"class", TokenType::CLASS},
      {"else", TokenType::ELSE},     {"false", TokenType::FALSE},
      {"for", TokenType::FOR},       {"fun", TokenType::FUN},
      {"if", TokenType::IF},         {"nil", TokenType::NIL},
      {"or", TokenType::OR},         {"print", TokenType::PRINT},
      {"return", TokenType::RETURN}, {"super", TokenType::SUPER},
      {"this", TokenType::THIS},     {"true", TokenType::TRUE},
      {"var", TokenType::VAR},       {"while", TokenType::WHILE}};

  bool isAtEnd() { return current >= source_.size(); }

  char advance() { return source_[current++]; }

  void addToken(TokenType type, const Object& literal) {
    std::string text = source_.substr(start, current - start);
    tokens_.push_back({type, text, literal, line});
  }

  void addToken(TokenType type) { addToken(type, {}); }

  bool match(char expected) {
    if (isAtEnd()) return false;
    if (source_[current] != expected) return false;
    ++current;
    return true;
  }

  char peek() {
    if (isAtEnd()) return '\0';
    return source_[current];
  }

  char peekNext() {
    if (current + 1 >= source_.size()) return '\0';
    return source_[current + 1];
  }

  void string() {
    while (peek() != '"' && !isAtEnd()) {
      if (peek() == '\n') ++line;
      advance();
    }

    if (isAtEnd()) {
      lox::error(line, "Unterminated string.");
      return;
    }

    // The closing ".
    advance();
    auto value = source_.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
  }

  bool isDigit(char c) { return c >= '0' && c <= '9'; }
  bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }
  bool isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

  void number() {
    while (isDigit(peek())) advance();
    // Look for a  fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
      // Consume "."
      advance();
      while (isDigit(peek())) advance();
    }

    double value = std::stod(source_.substr(start, current - start));
    addToken(TokenType::NUMBER, value);
  }

  void identifier() {
    while (isAlphaNumeric(peek())) advance();
    auto value = source_.substr(start, current - start);
    if (keywords.count(value) > 0) {
      addToken(keywords[value]);
    } else {
      addToken(TokenType::IDENTIFIER);
    }
  }

  void scanToken() {
    auto c = advance();
    switch (c) {
      case '(':
        addToken(TokenType::LEFT_PAREN);
        break;
      case ')':
        addToken(TokenType::RIGHT_PAREN);
        break;
      case '{':
        addToken(TokenType::LEFT_BRACE);
        break;
      case '}':
        addToken(TokenType::RIGHT_BRACE);
        break;
      case ',':
        addToken(TokenType::COMMA);
        break;
      case '.':
        addToken(TokenType::DOT);
        break;
      case '-':
        addToken(TokenType::MINUS);
        break;
      case '+':
        addToken(TokenType::PLUS);
        break;
      case ';':
        addToken(TokenType::SEMICOLON);
        break;
      case '*':
        addToken(TokenType::STAR);
        break;
      case '!':
        addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
      case '=':
        addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
      case '<':
        addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
      case '>':
        addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;
      case '/':
        if (match('/')) {
          // A comment goes until the end of the line
          while (peek() != '\n' && !isAtEnd()) advance();
        } else {
          addToken(TokenType::SLASH);
        }
        break;
      case ' ':
      case '\r':
      case '\t':
        break;
      case '\n':
        ++line;
        break;
      case '"':
        string();
        break;
      default:
        if (isDigit(c)) {
          number();
        } else if (isAlpha(c)) {
          identifier();
        } else {
          lox::error(
              line,
              fmt::format("Unexpected character. char repr: {} int repr: {}", c,
                          int(c)));
        }
        break;
    }
  }
};

}  // namespace lox

#endif  // LOX_SCANNER_HPP
