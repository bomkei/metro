#pragma once

namespace Metro {
  struct Token;
}

namespace Metro::AST {
  struct Compare : Base {
    struct Item {
      enum class Kind {
        BiggerLeft,
        BiggerRight,
        BiggerOrEqualLeft,
        BiggerOrEqualRight,
        Equal,
        NotEqual
      };

      Kind    kind;
      Token*  token;
      Base*   ast;

      explicit Item(Item::Kind kind, Token* token, Base* ast)
        : kind(kind),
          token(token),
          ast(ast)
      {
      }
    };

    Base*               first;
    std::vector<Item>   list;

    static Compare* create(Base*& ast);

    Item& append(Item::Kind kind, Token* tok, Base* ast) {
      return list.emplace_back(kind, token, ast);
    }

    explicit Compare(Base* first)
      : Base(Kind::Compare, nullptr),
        first(first)
    {
    }
  };
}