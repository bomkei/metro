#pragma once

std::string mangle(std::string name, std::string const& tag, std::vector<std::string> const& arg_types);
std::string demangle(std::string const& name);
