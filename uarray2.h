/*
 *      uarray2.h
 *      by Lewis Bobrow and John Stewart
 *      February 10th 2020
 *      Assignment: HW2 (iii)
 *
 *      This code declares the UArray2_T struct, as well as functions
 *      associated with the struct (including a function that creates a new
 *      UArray2_T object, a function that frees memory, and functions to get
 *      info about the specific object/its elemetsn and manipulate them
 */

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED
#include "uarray.h"
#include "assert.h"

#define T UArray2_T
typedef struct T *T;

/* Each UArray2_T contains a UArray_T object. This UArray_T can contain
 * UArray_T objects to form a 2D array of UArray_T objects
 */
struct T {
    int width;
    int height;
    int size;
    UArray_T rowArray;
};

/* exported functions */

/* Purpose: UArray2_new instantiates a UArray2_T object, allocates adequate
 *          memory for it, and initializes the struct variables using
 *          the parameters and the Ramsey/Hanson UArray_T function
 * I: Two nonnegative integer values representing the width and
 *    height of the 2D uarray.
 * O: A UArray2_T object
 */
T UArray2_new(int row, int col, int size);

/* Purpose: UArray2_free frees memory allocated for the UArray2_T's rowArray
 *          varaible
 * I: A nonnull pointer to a UArray2_T object
 * O: N/A
 */
void UArray2_free(T *uarray2);

/* Purpose: UArray2_width returns the value for the width of a given UArray2_T
 * I: An existing and initialized UArray2_T object
 * O: An integer representing the UArray2_T's width variable
 */
int UArray2_width(T uarray2);

/* Purpose: UArray2_height returns the value for the height of a given
 *          UArray2_T
 * I: An existing and initailized UArray2_T object
 * O: An integer representing the UArray2_T's height variable
 */
int UArray2_height(T uarray2);

/* Purpose: UArray2_size returns the value for the size of a given element in
 *          a given UArray2_T
 * I: An existing and initailized UArray2_T object
 * O: An integer representing the UArray2_T's size variable
 */
int UArray2_size(T uarray2);

/* Purpose: UArray2_at returns the value for a given element located at
 *          position [i, j] in a given UArray2_T
 * I: An existing and initailized UArray2_T object, and position variables
 *    i and j. i and j are greater than 0 and less than the width and
 *    height of the given UArray2_T respectively
 * O: An void pointer that points to the integer at position [i, j]
 */
void *UArray2_at(T uarray2, int i, int j);

/* Purpose: UArray2_map_row_major applies a certain function to all of the
 *          elements within a given UArray2_T object, iterating through the
 *          object one row at a time.
 * I: An existing and initailized UArray2_T object, an apply function that
 *    takes in the parameters specified below, a void pointer to carry certain
 *    values over between function calls
 * O: N/A
 */
void UArray2_map_row_major(T uarray2,
                           void apply(int i, int j, T uarray2, void *elem, 
                           void *cl), void *cl);

/* Purpose: UArray2_map_col_major applies a certain function to all of the
 *          elements within a given UArray2_T object, iterating through the
 *          object one column at a time.
 * I: An existing and initailized UArray2_T object, an apply function that
 *    takes in the parameters specified below, a void pointer to carry certain
 *    values over between function calls
 * O: N/A
 */
void UArray2_map_col_major(T uarray2, 
                           void apply(int i, int j, T uarray2, void *elem, 
                           void *cl), void *cl);


#undef T
#endif
