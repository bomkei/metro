#pragma once

#define  BIT(N)   (1 << N)

#define  inlineSS(e...) \
  ({ std::stringstream _xss_v; _xss_v << e; _xss_v.str(); })
