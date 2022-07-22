#pragma once

#include <cstdio>
#include "colors.h"

#if METRO_DEBUG
  #define  alert              fprintf(stderr,COL_MAGENTA "\t#alert " __FILE__ ":%d\n" COL_DEFAULT,__LINE__)
  #define  alertmsg(e...)     fprintf(stderr,COL_RED "\t#message: %s :" __FILE__ ":%d\n" COL_DEFAULT,#e,__LINE__)
  #define  alertios(e...)     (std::cerr<<COL_RED "\t#message: " << e << " :" __FILE__ ":" << __LINE__ << "\n" COL_DEFAULT)
  #define  crash              { alert; fprintf(stderr,"\n\n# application has been crashed.\n"); exit(1); }
#else
  #define  alert              0
  #define  alertmsg(...)      0
  #define  alertios(...)      0
  #define  crash            { fprintf(stderr,"\t#crash: %s:%d\n",__FILE__,__LINE__); exit(1); }
#endif

#define  TODO_IMPL  { alert; fprintf(stderr,"\n\n# Not implemented error.\n"); exit(1); }

namespace Metro {
  struct Node;

  namespace Debug {
    std::string node2s(Node* node);
  }
}