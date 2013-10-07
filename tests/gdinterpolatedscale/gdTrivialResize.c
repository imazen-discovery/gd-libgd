#include <stdio.h>

#include "gd.h"
#include "gdtest.h"

/* Test gdImageScale() with bicubic interpolation on a simple
 * all-white image. */

gdImagePtr mkwhite(int x, int y)
{
    gdImagePtr im;

	im = gdImageCreateTrueColor(x, y);
	gdImageFilledRectangle(im, 0, 0, x-1, y-1,
                           gdImageColorExactAlpha(im, 255, 255, 255, 0));

    gdTestAssert(im != NULL);

    gdImageSetInterpolationMethod(im, GD_BICUBIC);    // FP interp'n

    return im;
}/* mkwhite*/


/* Fill with almost-black. */
void mkblack(gdImagePtr ptr) {
    gdImageFilledRectangle(ptr, 0, 0, ptr->sx - 1, ptr->sy - 1,
                           gdImageColorExactAlpha(ptr, 2, 2, 2, 0));
}


#define CLOSE_ENOUGH 10

void scaletest(int x, int y, int nx, int ny)
{
    gdImagePtr im, imref, tmp, same;

	imref = mkwhite(x, y);
    im = mkwhite(x, y);
    tmp = gdImageScale(im, nx, ny);
    same = gdImageScale(tmp, x, y);

    gdTestAssert(gdMaxPixelDiff(im, same) < CLOSE_ENOUGH);
    mkblack(tmp);
    gdTestAssert(gdMaxPixelDiff(imref, same) < CLOSE_ENOUGH);

    gdImageDestroy(im);
    gdImageDestroy(tmp);
    gdImageDestroy(same);
}/* scaletest*/

void do_test(int x, int y, int nx, int ny)
{
	gdImagePtr im, imref, tmp;
    gdImagePtr same, same2;

	im = mkwhite(x, y);
    imref = mkwhite(x, y);

    same = gdImageScale(im, x, y);

    /* Trivial resize should be a straight copy. */
    gdTestAssert(im != same);
    gdTestAssert(gdMaxPixelDiff(im, same) == 0);
    gdTestAssert(gdMaxPixelDiff(imref, same) == 0);

    /* Ensure that modifying im doesn't modify same (i.e. make sure
     * they're not accidentally sharing the same pixel buffer.) */
    mkblack(im);
    gdTestAssert(gdMaxPixelDiff(imref, same) == 0);

    gdImageDestroy(same);
    gdImageDestroy(im);

    /* These currently fail due to the "black border" bug. */

    /* Scale horizontally, vertically and both. */
    scaletest(x, y, nx, y);
    scaletest(x, y, x, ny);
    scaletest(x, y, nx, ny);
}

int main(int argc, char **argv)
{

    do_test(300, 300, 600, 600);
    do_test(3200, 2133, 640, 427);

    return gdNumFailures();
}
