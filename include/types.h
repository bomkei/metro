#pragma once

#include <cstdint>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

struct Token;
struct ObjectType;
struct Object;
struct Node;
struct BuiltinFunc;
struct Driver;

template <class T>
using Cache = std::map<Node*, T>;

using ObjectArgs = std::vector<Object>;
