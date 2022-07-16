# Memory mapping in union in Node
```c
struct Node {
  union __ndUnion {
    Token*  tok;
    BuiltinFunc const* bifun;

    struct {
      Object* object;
      bool    bval[2];
      Node*   ndval[4];
    };
  };

  NodeKind  kind;
  Token*    token;

  std::vector<Node*>  nodes;

  __ndUnion   uni;
};
```

```
ND_LET:
  let x : _Type = _Expr;
```

| kind              | Token* 0 | Token* 1 | bool 0   | bool 1   | Node* 0   | Node* 1 | Node* 2  | Node* 3   | Object* 0 | BuiltinFunc const* |
|-------------------|----------|----------|----------|----------|-----------|---------|----------|-----------|-----------|--------------------|
| ND_TYPE           | name     |          | isRef    | isConst  |
| ND_ARGUMENT       | name     |          |          |          | type      |
| ND_VALUE          |          |          |          |          |           |         |          |           | object    |
| ND_VARIABLE       | name     |
| ND_CALLFUNC       | name     |
| ND_ADDR           |          |          |          |          | item      |
| ND_LET            | name     |          |          |          | type      | expr    |
| ND_IF             |          |          |          |          | if_true   | expr    | if_false |
| ND_FOR            |          |          |          |          | cond      | code    | init     | inclement |
| ND_FOREACH ?      |
| ND_LOOP ?         |
| ND_WHILE          |          |          |          |          | cond      | code    |
| ND_DO_WHILE ?     |
| ND_SCOPE          |
| ND_EXPR           |
| ND_FUNCTION       | name     |          |          |          | ret_type  | code    |
| ND_BUILTIN_FUNC   |          |          |          |          |           |         |           |           |          | bifun |
| ND_CLASS          | name   | | | | | | | | |  |
| ND_NAMESPACE      | name |

