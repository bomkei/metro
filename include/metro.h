#pragma once

#include <algorithm>
#include <cassert>
#include <codecvt>
#include <concepts>
#include <cstdarg>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

extern Driver driver;

Token* tokenize(char const* src);
Node* parse(Token* token);
ObjectType evaluate(Node* node);
Object run_node(Node* node);

void evaluater_reset();
void clear_application();

std::string open_file(std::string const& path);
bool parse_arguments(int argc, char** argv);
int main(int argc, char** argv);

std::string node_to_string(Node* node);
