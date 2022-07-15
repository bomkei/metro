#include <fstream>
#include "Utils.h"
#include "Lexer.h"
#include "Parser.h"
#include "Sema/Analyzer.h"
#include "Evaluator.h"
#include "Application.h"
#include "appInternal.h"

#include "Types/Token.h"
#include "Types/Object.h"
#include "Types/Node.h"

#include "BuiltinFunctions.h"

namespace Metro {

  static Node* newnamespace(std::string_view const& name, std::vector<Node*> const& nodes) {
    //auto node = new 

    return nullptr;
  }

  Node* AppInternal::construct_sysmodules_node() {


    return nullptr;
  }

}