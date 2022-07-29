#include <cstdarg>
#include "AST.h"
#include "Types.h"
#include "MetroDriver/Compiler.h"
#include "Debug.h"

//
// emit() はデバッグ用です
// 実装するときに emit() をすべて置き換える
//

namespace Metro {

  using ASTKind = AST::Kind;
  using namespace IR;

  void Compiler::emit(char const* fmt, ...) {
    static char buf[0x1000];

    va_list ap;
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    printf("\t%s\n", buf);
  }

  void Compiler::compile(AST::Base* ast) {
    if( !ast ) {
      alert;
      return;
    }

    auto& inst = inst_list.emplace_back();

    switch( ast->kind ) {
      case ASTKind::Value: {
        
      }
    }

  }

}