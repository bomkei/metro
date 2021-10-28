#include <fstream>
#include "common.h"
#include "token.h"
#include "object.h"
#include "AST.h"
#include "error.h"
#include "driver.h"

static constexpr auto HELP_STRING =
  R"(Metro: A Typed Script - Language

built at )" BUILT_DATE R"(

usage:
  $ metro [options...] script.metro

options:
  -h, -help          show this test
  -v, --version      show version informations
  -unsafe            disable safety mode in running script file. (do not prevent stack overflow, r/w invalid memory, etc...)
  -optimize          give ultra optimize to syntax tree
)"

#if __DEBUG__
  "\nwarning: some options is not implemented."
#endif

  ;

Driver driver;

void evaluater_reset();
Token* tokenize(char const*);
Node* parse(Token*);
ObjectType evaluate(Node*);
Object run_node(Node*);

void clear_application() {
  evaluater_reset();
}

std::string open_file(std::string const& path) {
  std::ifstream ifs{path};
  std::string ret, line;

  if (ifs.fail()) {
    fatal_error("cannot open file: " + path);
  }

  while (std::getline(ifs, line)) {
    ret += line + '\n';
  }

  return ret;
}

bool parse_arguments(int argc, char** argv) {
  driver.app_path = *argv;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "-h" || arg == "-help") {
      std::cout << HELP_STRING << std::endl;
      exit(0);
    } else if (arg == "-v" || arg == "--version") {
      std::cout << VERSION << std::endl;
      exit(0);
    } else if (arg == "-unsafe") {
      driver.unsafe = true;
    } else if (arg == "-optimize") {
      driver.optimize = true;
    } else {
      driver.script_path.emplace_back(arg);
    }
  }

  return true;
}

int main(int argc, char** argv) {
  if (!parse_arguments(argc, argv)) {
    fatal_error("invalid arguments");
  }

  if (driver.script_path.empty()) {
    fatal_error("no input files.");
  }

  for (auto&& path : driver.script_path) {
    auto& src = driver.source_code;

    driver.source_code = open_file(path);

    for (i64 i = 0; i < src.length() - 1; i++) {
      if (src.substr(i, 2) == "//") {
        for (; i < src.length(); i++) {
          auto& ch = src[i];

          if (ch == '\n')
            break;

          ch = ' ';
        }
      } else if (src.substr(i, 2) == "/*") {
        for (; i < src.length() - 2; i++) {
          if (src.substr(i, 2) == "*/") {
            src[i] = src[i + 1] = ' ';
            i += 2;
            break;
          }

          src[i] = ' ';
        }
      }
    }

    clear_application();

    alert;
    auto token = tokenize(driver.source_code.c_str());
    check_error();

    // for( auto tk = token; tk && tk->str; tk = tk->next ) {
    //   std::cout << tk->get_string() << std::endl;
    // }

    alert;
    auto node = parse(token);
    check_error();

    // std::cout << node_to_string(node) << std::endl;

    alert;
    evaluate(node);
    check_error();

    alert;
    run_node(node);

    driver.cur_script_index++;
  }

  return 0;
}