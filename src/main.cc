#include <fstream>
#include "metro.h"

int main(int argc, char** argv) {
  
  std::cout << "Hello!" << std::endl;

  std::string source;
  std::ifstream ifs{ "test.txt" };

  // read source
  for( std::string line; std::getline(ifs, line); ) {
    source += line + '\n';
  }

  Lexer lexer{ source };

  auto token = lexer.lex();

  // print token
  for(auto t=token;t->kind!=TOK_END;t=t->next){
    std::cout<<t->str<<std::endl;
  }

  alert;

  Parser parser{ token };

  auto node = parser.parse();

  alert;

  Evaluater eval;
  auto type = eval.eval(node);

  std::cout << type.to_string() << std::endl;

}