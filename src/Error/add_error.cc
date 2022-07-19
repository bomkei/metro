#include <vector>
#include "Application.h"
#include "Error.h"

namespace Metro::Error {
  std::vector<ErrorContext> contexts;

  void add_error(Token* token, std::string const& msg) {
    
  }

  void add_error(Node* node, std::string const& msg) {

  }
}