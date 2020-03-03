/*
 *      bit2.c
 *      by Lewis Bobrow and John Stewart
 *      February 10th 2020
 *      Assignment: HW2 (iii)
 *
 *      This includes the function definitions for all of the functions
 *      declared in bit2.h
 */

#include <stdlib.h>
#include <stdio.h>
#include "bit2.h"

/* Purpose: Bit2_new instantiates a Bit2_T object, allocates adequate
 *          memory for it, and initializes the struct variables using
 *          the parameters and the Hanson Bit_new function
 * I: Two nonnegative integer values representing the width and
 *    height of the bit map.
 * O: A Bit2_T object
 */
Bit2_T Bit2_new(int row, int col)
{
    assert(row >= 0 && col >= 0);

    Bit2_T bit2 = (Bit2_T) malloc(2 * sizeof(Bit2_T));
    bit2->width = row;
    bit2->height = col;

    /* single 1D array with row * col positions to represent a 2D array */
    bit2->array = Bit_new(row * col);

    return bit2;
}

/* Purpose: Bit2_free frees memory allocated for the Bit2_T's array varaible
 * I: A nonnull pointer to a Bit2_T object
 * O: N/A
 */
void Bit2_free(Bit2_T *bit2)
{
    assert(bit2 && *bit2);
    Bit_free(&((*bit2)->array));
    free(*bit2);
}

/* Purpose: Bit2_width returns the value for the width of a given Bit2_T
 * I: An existing and initialized Bit2_T object
 * O: An integer representing the Bit2_T's width variable
 */
int Bit2_width(Bit2_T bit2)
{
    assert(bit2);
    return bit2->width;
}

/* Purpose: Bit2_height returns the value for the height of a given Bit2_T
 * I: An existing and initailized Bit2_T object
 * O: An integer representing the Bit2_T's height variable
 */
int Bit2_height(Bit2_T bit2)
{
    assert(bit2);
    return bit2->height;
}

/* Purpose: Bit2_size returns the value for the size of any given element in
 *          a given Bit2_T
 * I: An existing and initialized Bit2_T object
 * O: A float representing the Bit2_T's size variable
 */
float Bit2_size(Bit2_T bit2)
{
    (void) bit2;
    return 0.125;
}

/* Purpose: Bit2_get retrieves the value in the specified row and col
 *          position in the bit map by accessing the corresponding index
 *          in the 1D Bit Vector
 * I: An existing and initialized Bit2_T object, and a [row, column] position
 *    within that object (defined by row and col respectively). These
 *    positions must be both nonnegative and less than the max width and
 *    height defined by the Bit2_T's object's properties
 * O: The integer value at the position in the bitmap represented
 *    by the parameters
 */
int Bit2_get(Bit2_T bit2, int row, int col)
{
    assert(bit2);
    assert(row < Bit2_width(bit2) && row >= 0);
    assert(col < Bit2_height(bit2) && col >= 0);
    return Bit_get(bit2->array, (bit2->width * col) + row);
}

/* Purpose: Bit2_put places or replaces a certain integer value at position
 *          [row, col] within a given Bit2_T object.
 * I: An existing and initailized Bit2_T object, a [row, column] position
 *    within that object (defined by row and col respectively), the value to
 *    be placed at that position. The positions must be both nonnegative and
 *    less than the Bit2_T object's maximum width and height
 * O: The integer value that was placed at the position represented by the
 *    parameters
 */
int Bit2_put(Bit2_T bit2, int row, int col, int bit)
{
    assert(bit2);
    assert(row < Bit2_width(bit2) && row >= 0);
    assert(col < Bit2_height(bit2) && col >= 0);
    return Bit_put(bit2->array, (bit2->width * col) + row, bit);
}

/* Purpose: Bit2_map_row_major applies a certain function to all of the
 *          elements within a given Bit2_T object, iterating through the
 *          object one row at a time.
 * I: An existing and initialized Bit2_T object, an apply function that takes
 *    in the parameters specified below, a void pointer to carry certain
 *    values over between function calls
 * O: N/A
 */
void Bit2_map_row_major(Bit2_T bit2,
                        void apply(int row, int col, Bit2_T bit2, int elem,
                        void *cl), void *cl)
{
    assert(bit2);
    int i, j;       // [i, j] represents [row position, col position]
    for (j = 0; j < bit2->height; j++) {
        for (i = 0; i < bit2->width; i++) {
            /* col position is getting bigger faster */
            apply(i, j, bit2, Bit2_get(bit2, i, j), cl);
        }
    }
}

/* Purpose: Bit2_map_col_major applies a certain function to all of the
 *          elements within a given Bit2_T object, iterating through the
 *          object one column at a time.
 * I: An existing and initailized Bit2_T object, an apply function that takes
      in the parameters specified below, a void pointer to carry certain
      values over between function calls
 * O: N/A
 */
void Bit2_map_col_major(Bit2_T bit2,
                        void apply(int row, int col, Bit2_T bit2, int elem,
                        void *cl), void *cl)
{
    assert(bit2);
    int i, j;       // [i, j] represents [row position, col position]
    for (i = 0; i < bit2->width; i++) {
        for (j = 0; j < bit2->height; j++) {
            /* row position is getting bigger faster */
            apply(i, j, bit2, Bit2_get(bit2, i, j), cl);
        }
    }
}
