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
