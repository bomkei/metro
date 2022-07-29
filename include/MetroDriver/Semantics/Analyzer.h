#pragma once

#include <map>
#include <list>
#include "AST.h"

namespace Metro::Semantics {

/*

scope_list に追加されるもの :
  If
  For
  Scope
  Function

*/

  class Analyzer {

    using ScopeList = std::list<AST::Base*>;

    struct Task {
      enum class Kind {
        Variable,
        Callfunc,
        If,
        Let,
        Function
      };

      Kind        kind;
      AST::Base*  ast;
      ScopeList   scope;

      // if function
      std::vector<AST::Base*> last_expr_list;

      explicit Task(Kind kind, AST::Base* ast, ScopeList scope)
        : kind(kind),
          ast(ast),
          scope(scope)
      {
      }
    };

  public:

    Analyzer();

    // ast_if, ast_let, ... を作成する
    void walk(AST::Base* ast);

    //
    void check_type_matches();

    void get_last_expr(std::vector<AST::Base*>& out, AST::Base* ast);

    //
    void analyze();

    AST::Function* find_func(Task& task);
    AST::Base* find_var_defined(Task& task);

    // 型
    ValueType evaltype(AST::Base* ast);

    static Object* create_obj(Token* token);

  private:

    void enter_scope(AST::Base* ast) {
      scope_list.push_front(ast);
    }

    void leave_scope() {
      scope_list.pop_front();
    }

    AST::Base* get_current_scope() {
      return *scope_list.begin();
    }

    auto& add_task(Task::Kind kind, AST::Base* ast) {
      return tasks.emplace_back(kind, ast, scope_list);
    }

    ScopeList scope_list;

    std::vector<Task> tasks;

  };

}
