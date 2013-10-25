#ifndef GD_INTERN_H
#define GD_INTERN_H

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif
#define MIN3(a,b,c) ((a)<(b)?(MIN(a,c)):(MIN(b,c)))
#ifndef MAX
#define MAX(a,b) ((a)<(b)?(b):(a))
#endif
#define MAX3(a,b,c) ((a)<(b)?(MAX(b,c)):(MAX(a,c)))


typedef enum {
    HORIZONTAL,
    VERTICAL,
} gdAxis;

/* Convert a double (containing a color channel value) to an unsigned
 * char, rounding to the nearest integer and clamping the result
 * between 0 and 255.  The absolute value of clr must be less than the
 * maximum value of an unsigned short. */
static inline unsigned char
uchar_clamp(double clr) {
	unsigned short result;

	assert(fabs(clr) <= SHRT_MAX);

	/* Casting a negative float to an unsigned short is undefined.
	 * However, casting a float to a signed truncates toward zero and
	 * casting a negative signed value to an unsigned of the same size
	 * results in a bit-identical value (assuming twos-complement
	 * arithmetic).	 This is what we want: all legal negative values
	 * for clr will be greater than 255. */

	/* Convert and clamp. */
	result = (unsigned short)(short)(clr + 0.5);
	if (result > 255) {
		result = (clr < 0) ? 0 : 255;
	}/* if */

	return result;
}/* uchar_clamp*/


#ifndef MAXPATHLEN
# ifdef PATH_MAX
#  define MAXPATHLEN PATH_MAX
# elif defined(MAX_PATH)
#  define MAXPATHLEN MAX_PATH
# else
#  if defined(__GNU__)
#   define MAXPATHLEN 4096
#  else
#   define MAXPATHLEN 256    /* Should be safe for any weird systems that do not define it */
#  endif
# endif
#endif

#ifdef HAVE_STDINT_H
# include <stdint.h>
#else
# if defined(HAVE_INTTYPES_H)
#  include <inttypes.h>
# else
#  include "msinttypes/inttypes.h"
# endif
#endif

#endif

