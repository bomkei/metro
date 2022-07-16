#pragma once

#include <cstdio>

#if METRO_DEBUG
  #define  alert  fprintf(stderr,"\t#alert " __FILE__ ":%d\n",__LINE__)
  #define  crash  { alert; fprintf(stderr,"\n\n# application has been crashed.\n"); exit(1); }
#else
  #define  alert  0
  #define  crash  exit(1);
#endif

#define  TODO_IMPL  { alert; fprintf(stderr,"\n\n# Not implemented error.\n"); exit(1); }


