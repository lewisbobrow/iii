/*
 *      bit2.h
 *      by Lewis Bobrow and John Stewart
 *      February 10th 2020
 *      Assignment: HW2 (iii)
 *
 *      This code declares the Bit2_T struct, as well as functions associated
 *      with the struct (including a function that creates a new Bit2_T
 *      object, a function that frees memory, and functions to get info
 *      about the specific object/its elemetsn and manipulate them
 */

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED
#include "bit.h"
#include "assert.h"

#define T Bit2_T
typedef struct T *T;

/* Bit2_T is composed of a 1D Hanson Bit Vector */
struct T {
    int width;
    int height;
    Bit_T array;
};

/* exported functions */

/* Purpose: Bit2_new instantiates a Bit2_T object, allocates adequate
 *          memory for it, and initializes the struct variables using
 *          the parameters and the Hanson Bit_new function
 * I: Two nonnegative integer values representing the width and
 *    height of the bit map.
 * O: A Bit2_T object
 */
T Bit2_new(int row, int col);

/* Purpose: Bit2_free frees memory allocated for the Bit2_T's array varaible
 * I: A nonnull pointer to a Bit2_T object
 * O: N/A
 */
void Bit2_free(T *bit2);

/* Purpose: Bit2_width returns the value for the width of a given Bit2_T
 * I: An existing and initialized Bit2_T object
 * O: An integer representing the Bit2_T's width variable
 */
int Bit2_width(T bit2);

/* Purpose: Bit2_height returns the value for the height of a given Bit2_T
 * I: An existing and initailized Bit2_T object
 * O: An integer representing the Bit2_T's height variable
 */
int Bit2_height(T bit2);

/* Purpose: Bit2_size returns the value for the size of any given element in
 *          a given Bit2_T
 * I: An existing and initialized Bit2_T object
 * O: A float representing the Bit2_T's size variable
 */
float Bit2_size(T bit2);

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
int Bit2_get(T bit2, int row, int col);

/* Purpose: Bit2_put places or replaces a certain integer value at position
 *          [row, col] within a given Bit2_T object.
 * I: An existing and initailized Bit2_T object, a [row, column] position
 *    within that object (defined by row and col respectively), the value to
 *    be placed at that position. The positions must be both nonnegative and
 *    less than the Bit2_T object's maximum width and height
 * O: The integer value that was placed at the position represented by the
 *    parameters
 */
int Bit2_put(T bit2, int row, int col, int bit);

/* Purpose: Bit2_map_row_major applies a certain function to all of the
 *          elements within a given Bit2_T object, iterating through the
 *          object one row at a time.
 * I: An existing and initialized Bit2_T object, an apply function that takes
 *    in the parameters specified below, a void pointer to carry certain
 *    values over between function calls
 * O: N/A
 */
void Bit2_map_row_major(T bit2,
                        void apply(int row, int col, T bit2, int elem,
                        void *cl), void *cl);

/* Purpose: Bit2_map_col_major applies a certain function to all of the
 *          elements within a given Bit2_T object, iterating through the
 *          object one row at a time.
 * I: An existing and initailized Bit2_T object, an apply function that takes
      in the parameters specified below, a void pointer to carry certain
      values over between function calls
 * O: N/A
 */
void Bit2_map_col_major(T bit2,
                        void apply(int row, int col, T bit2, int elem,
                        void *cl), void *cl);


#undef T
#endif
