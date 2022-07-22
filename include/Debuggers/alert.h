#pragma once

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

// alertwarn
#define  alertwarn(e...) \
  alertmsg(COL_RED "warning: " #e)

// alertctor
#define  alertctor(_Name_) \
  fprintf(stderr,COL_GREEN"\t#Constructing " \
    COL_CYAN #_Name_ " (%p)" \
    COL_GREEN":" __FILE__":%d\n" COL_DEFAULT,this,__LINE__)