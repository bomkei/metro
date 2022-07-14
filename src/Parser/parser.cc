#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"
#include "Parser.h"
#include "Utils.h"

Parser::Parser(Token* token)
  : cur(token),
    ate(nullptr)
{
}

Node* Parser::parse() {
  auto node = new Node(ND_SCOPE, nullptr);

  while( check() ) {
    node->append(func());
  }

  return node;
}