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


  

}