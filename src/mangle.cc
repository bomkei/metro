#include "common.h"
#include "mangle.h"
#include "utils.h"

std::string mangle(std::string name, std::string const& tag, std::vector<std::string> const& arg_types) {
  if (!tag.empty())
    name += "@" + tag;

  if (!arg_types.empty()) {
    name += "$<" + join(",", arg_types) + ">";
  }

  return name;
}

std::string demangle(std::string const& name) {
  TODO_IMPL;
}
