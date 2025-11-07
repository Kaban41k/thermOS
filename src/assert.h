#include "kernelpanic.h"

#ifdef DEBUG
#define assert(cond) ((cond) ? (void)(0) : kernel_panic(__FILE__ ", line %d: assert(" #cond ") FAILED", __LINE__))
#else
#define assert(cond) ((void)(0))
#endif