#include "AST.h"
#include "Types.h"
#include "Semantics/Analyzer.h"
#include "GC.h"
#include "Error.h"

namespace Metro::Semantics {
  using ASTKind   = AST::Kind;
  using TypeCon   = TypeContext;

  void Analyzer::check_symbols() {

  }

  std::tuple<Analyzer::ScopeContext*, TypeCon*> Analyzer::find_var(std::string_view name) {
    for( auto&& scope_ast : scopelist ) {
      auto& scopeContext = scopemap[scope_ast];

      switch( scopeContext.ast->kind ) {
        case ASTKind::Scope: {
          auto scope = (AST::Scope*)scopeContext.ast;

          for( size_t i = 0; i < scope->elems.size(); i++ ) {
            auto elem = scope->elems[i];

            if( i >= scopeContext.cur_index ) {
              break;
            }

            if( elem && elem->kind == ASTKind::Let && ((AST::Let*)elem)->name == name ) {
              auto& type = scopeContext.variable_types[name];

              type.defined = elem;

              return { &scopeContext, &type };
            }
          }

          break;
        }
      }
    }

    return { };
  }

  void Analyzer::append_assign(TypeCon& type, AST::Base* ast) {
    auto ctx = walk(ast);

    for( auto&& i : type.assignmented ) {
      if( !walk(i).equals(ctx) ) {
        Error::add_error(ErrorKind::TypeMismatch, ast->token, "type mismatch");
        return;
      }
    }

    type.assignmented.emplace_back(ast);
  }

  Analyzer::ScopeContext& Analyzer::get_cur_scope() {
    return scopemap[*scopelist.begin()];
  }
}