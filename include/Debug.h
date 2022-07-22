#pragma once

#include <cstdio>
#include "colors.h"

// color in message area
#define  COL_ALERTMSG  COL_WHITE


#if METRO_DEBUG
  // alert
  #define  alert  \
    fprintf(stderr,COL_MAGENTA "\t#alert " __FILE__ ":%d\n" COL_DEFAULT,__LINE__)
  
  // alertmsg
  #define  alertmsg(e...) \
    fprintf(stderr,COL_YELLOW "\t#message: " \
      COL_ALERTMSG #e COL_YELLOW " :from " __FILE__ ":%d\n" COL_DEFAULT,__LINE__)
  
  // alertios
  #define  alertios(e...) \
    (std::cerr<<COL_YELLOW"\t#message: " \
      COL_ALERTMSG<<e<<COL_YELLOW" :from " __FILE__ ":"<<__LINE__<<"\n" COL_DEFAULT)

  // alertctor
  #define  alertctor(_Name_) \
    fprintf(stderr,COL_GREEN"\t#Constructing " \
      COL_CYAN #_Name_ " (%p)" \
      COL_GREEN":" __FILE__":%d\n" COL_DEFAULT,this,__LINE__)

  // debug
  #define  debug(e...) \
    { e }

  // crash
  #define  crash \
    {alert;fprintf(stderr,"\n#crashed at " __FILE__ ":%d\n",__LINE__);exit(1);}
#else
  #define  alert              0
  #define  alertmsg(...)      0
  #define  alertios(...)      0
  #define  alertctor(__x)     0
  #define  alertdtor(__x)     0
  #define  debug(...)         { }
  #define  crash              {fprintf(stderr,COL_RED "\n#------------------------------------#" \
                                "crashed at %s:%d\nPlease tell to developer.\n" COL_DEFAULT,__FILE__,__LINE__);exit(1);}
#endif

#define  TODO_IMPL  { alert; fprintf(stderr,"\n\n# Not implemented error.\n"); exit(1); }

namespace Metro {
  struct Node;

  namespace Debug {
    std::string node2s(Node* node);
  }
}