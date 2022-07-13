#include "Application.h"
#include "Utils.h"

int _Alert(char const* f, int n) {
  return METRO_DEBUG ? fprintf(stderr,"\t#alert %s:%d\n",f,n) : 0;
}

void _Crash(char const* f, int n) {
  fprintf(stderr,"crashed!! %s:%d\n",f,n);
  exit(1);
}
