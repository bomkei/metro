#pragma once

#include <cstdio>
#include "colors.h"

// color in message area
#define  COL_ALERTMSG  COL_WHITE

#if METRO_DEBUG_USE_ALERT
  #include "Debuggers/alert.h"
#else
  #define  alert              0
  #define  alertmsg(...)      0
  #define  alertios(...)      0
  #define  alertwarn(...)     0
  #define  alertctor(__x)     0
  #define  alertdtor(__x)     0
#endif

#if METRO_DEBUG_USE_DEBUGSCOPE
  #define  debug(e...) \
    { e }
#else
  #define  debug(...)  { }
#endif

#if METRO_DEBUG_USE_CRASH
  #define  crash \
    {alert;fprintf(stderr,"\n#crashed at " __FILE__ ":%d\n",__LINE__);exit(1);}
#else
  #define  crash \
    {fprintf(stderr,COL_RED "\n#------------------------------------#" \
    "crashed at %s:%d\nPlease tell to developer.\n" COL_DEFAULT,__FILE__,__LINE__);exit(1);}
#endif

#define  TODO_IMPL  { alert; fprintf(stderr,"\n\n# Not implemented error.\n"); exit(1); }

namespace Metro {
  struct Node;

  namespace Debug {
    std::string node2s(Node* node);
  }
}