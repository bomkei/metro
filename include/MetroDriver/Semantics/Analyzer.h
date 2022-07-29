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

    struct WalkedTemp {
      AST::Base*  ast;
      ScopeList   scope;

      // if function
      std::vector<AST::Base*> last_expr;

      WalkedTemp(AST::Base* ast, ScopeList scope)
        : ast(ast),
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

    AST::Function* find_func(WalkedTemp* tmp);
    AST::Base* find_var_defined(WalkedTemp* tmp);

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

    auto& push_temp(std::vector<WalkedTemp>& _d, AST::Base* ast) {
      return _d.emplace_back(ast, scope_list);
    }

    ScopeList scope_list;

    std::vector<WalkedTemp>
      tmp_variable,
      tmp_if,
      tmp_let,
      tmp_callfunc,
      tmp_function;

  };

}


/*
#include <map>
#include <list>
#include "AST.h"
#include "TypeContext.h"

namespace Metro::Semantics {
  class Analyzer {
    using TypeCon = TypeContext;

    struct ScopeContext {
      AST::Base*  ast;
      size_t      cur_index = 0;

      std::map<std::string_view, TypeContext>  variable_types;
    };

  public:
    Analyzer();
    ~Analyzer();

    TypeCon walk(AST::Base* ast);
    TypeCon& walk_lval(AST::Base* ast);

    TypeCon mustbe_evaluated(AST::Base* ast);

    bool is_valid_operator(AST::Kind op_kind, TypeCon const& left, TypeCon const& right);

    void check_symbols();

  private:

    void append_assign(TypeCon& type, AST::Base* ast);

    std::tuple<ScopeContext*, TypeCon*> find_var(std::string_view name);
    AST::Function* find_func(std::string_view name);

    ScopeContext& get_cur_scope();


    std::list<AST::Base*> walking;
    std::list<AST::Base*> scopelist;
    std::map<AST::Base*, TypeCon> caches;
    std::map<AST::Base*, ScopeContext> scopemap;
  };
}
*/