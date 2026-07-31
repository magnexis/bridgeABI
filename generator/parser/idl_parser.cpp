#include "parser/idl_parser.hpp"
#include <cctype>
#include <charconv>
#include <string>

namespace bridgeabi::generator {
namespace {
class parser {
public:
  explicit parser(std::string_view text) : text_(text) {}
  bool parse(interface_definition& output, std::string& error) {
    if (!word("namespace") || !identifier(output.name_space) || !consume(';')) return fail(error, "Expected namespace declaration");
    skip(); if (consume('@')) { std::string attribute; if (!identifier(attribute) || attribute != "id" || !consume('(') || !quoted(output.id) || !consume(')')) return fail(error, "Invalid @id attribute"); }
    if (!word("interface") || !identifier(output.name) || !word("version") || !number(output.major) || !consume('.') || !number(output.minor) || !consume('{')) return fail(error, "Expected interface <name> version <major>.<minor> {");
    while (true) { skip(); if (consume('}')) break; if (word("capability")) { capability cap; if (!quoted(cap.id) || !word("version") || !number(cap.major) || !consume('.') || !number(cap.minor) || !consume(';')) return fail(error, "Invalid capability"); output.capabilities.push_back(std::move(cap)); continue; } if (word("threading")) { if (!identifier(output.threading) || !consume(';')) return fail(error, "Invalid threading declaration"); continue; } method item; parse_owner(item.result_owner); if (!type(item.result_type) || !identifier(item.name) || !consume('(')) return fail(error, "Invalid method declaration"); skip(); if (!peek(')')) { while (true) { parameter arg; parse_owner(arg.owner); if (!type(arg.type)) return fail(error, "Invalid method parameter type"); if (consume('?')) arg.nullable = true; if (!identifier(arg.name)) return fail(error, "Invalid method parameter name"); item.parameters.push_back(std::move(arg)); skip(); if (consume(')')) break; if (!consume(',')) return fail(error, "Expected ',' or ')'"); } } else { consume(')'); } if (!consume(';')) return fail(error, "Expected ';' after method"); output.methods.push_back(std::move(item)); }
    skip(); if (position_ != text_.size()) return fail(error, "Unexpected trailing input"); return true;
  }
private:
  void skip() { while (position_ < text_.size() && std::isspace(static_cast<unsigned char>(text_[position_]))) ++position_; }
  bool peek(char value) { skip(); return position_ < text_.size() && text_[position_] == value; }
  bool consume(char value) { if (!peek(value)) return false; ++position_; return true; }
  bool identifier(std::string& value) { skip(); const auto begin = position_; if (position_ == text_.size() || !(std::isalpha(static_cast<unsigned char>(text_[position_])) || text_[position_] == '_')) return false; while (position_ < text_.size() && (std::isalnum(static_cast<unsigned char>(text_[position_])) || text_[position_] == '_' || text_[position_] == '.')) ++position_; value.assign(text_.substr(begin, position_ - begin)); return true; }
  bool word(std::string_view wanted) { const auto saved = position_; std::string actual; if (identifier(actual) && actual == wanted) return true; position_ = saved; return false; }
  bool quoted(std::string& value) { skip(); if (!consume('\"')) return false; const auto begin = position_; while (position_ < text_.size() && text_[position_] != '\"') ++position_; if (position_ == text_.size()) return false; value.assign(text_.substr(begin, position_ - begin)); ++position_; return !value.empty(); }
  bool number(std::uint32_t& value) { skip(); const auto begin = position_; while (position_ < text_.size() && std::isdigit(static_cast<unsigned char>(text_[position_]))) ++position_; if (begin == position_) return false; const auto parsed = std::from_chars(text_.data() + begin, text_.data() + position_, value); return parsed.ec == std::errc{}; }
  bool type(std::string& value) { return identifier(value); }
  void parse_owner(ownership& value) { const auto saved = position_; std::string word_value; if (!identifier(word_value)) return; if (word_value == "borrowed") value = ownership::borrowed; else if (word_value == "owned") value = ownership::owned; else if (word_value == "transferred") value = ownership::transferred; else if (word_value == "shared") value = ownership::shared; else if (word_value == "static") value = ownership::static_lifetime; else if (word_value == "host_owned") value = ownership::host_owned; else if (word_value == "plugin_owned") value = ownership::plugin_owned; else position_ = saved; }
  bool fail(std::string& error, std::string message) { error = "IDL parse error at offset " + std::to_string(position_) + ": " + message; return false; }
  std::string_view text_; std::size_t position_{};
};
}
bool parse_idl(std::string_view input, interface_definition& output, std::string& error) noexcept { try { output = {}; return parser(input).parse(output, error); } catch (...) { error = "IDL parser ran out of memory"; return false; } }
} // namespace bridgeabi::generator
