/*
 *      uarray2.c
 *      by Lewis Bobrow and John Stewart
 *      February 10th 2020
 *      Assignment: HW2 (iii)
 *
 *      This code includes the function definitions for all the functions
 *      declared in uarray2.h
 */

#include <stdlib.h>
#include <stdio.h>
#include "uarray2.h"
#include "uarray.h"

/* Purpose: UArray2_new instantiates a UArray2_T object, allocates adequate
 *          memory for it, and initializes the struct variables using
 *          the parameters and the Ramsey/Hanson UArray_T function
 * I: Two nonnegative integer values representing the width and
 *    height of the 2D uarray.
 * O: A UArray2_T object
 */
UArray2_T UArray2_new(int row, int col, int size)
{
    assert(row >= 0 && col >= 0);
    UArray2_T uarray2 = (UArray2_T)malloc(4 * sizeof(UArray2_T));

    uarray2->width = row;
    uarray2->height = col;
    uarray2->size = size;

    /* Iterates through each element of uarray2's rowArray and defines a new
     * new UArray_T object. This allows 2D indexing
     */
    uarray2->rowArray = UArray_new(row, 2 * sizeof(UArray_T));
    int i;
    for(i = 0; i < row; i++) {    //cast UArray_at as a variable type
        UArray_T colArray = UArray_new(col, size);
        *(UArray_T *)UArray_at(uarray2->rowArray, i) = colArray;
    }

    return uarray2;
}

/* Purpose: UArray2_free frees memory allocated for the UArray2_T's rowArray
 *          varaible
 * I: A nonnull pointer to a UArray2_T object
 * O: N/A
 */
void UArray2_free(UArray2_T *uarray2)
{
    // assert(uarray2);
    int i;

    /* frees all of the UArray_T objects within rowArray */
    for(i = 0; i < (*uarray2)->width; i++)
        UArray_free((UArray_T *)UArray_at((*uarray2)->rowArray, i));

    UArray_free(&(*uarray2)->rowArray);
    free(*uarray2);
}

/* Purpose: UArray2_width returns the value for the width of a given UArray2_T
 * I: An existing and initialized UArray2_T object
 * O: An integer representing the UArray2_T's width variable
 */
int UArray2_width(UArray2_T uarray2)
{
    assert(uarray2);
    return uarray2->width;
}

/* Purpose: UArray2_height returns the value for the height of a given
 *          UArray2_T
 * I: An existing and initailized UArray2_T object
 * O: An integer representing the UArray2_T's height variable
 */
int UArray2_height(UArray2_T uarray2)
{
    assert(uarray2);
    return uarray2->height;
}

/* Purpose: UArray2_size returns the value for the size of a given element in
 *          a given UArray2_T
 * I: An existing and initailized UArray2_T object
 * O: An integer representing the UArray2_T's size variable
 */
int UArray2_size(UArray2_T uarray2)
{
    assert(uarray2);
    return uarray2->size;
}

/* Purpose: UArray2_at returns the value for a given element located at
 *          position [i, j] in a given UArray2_T
 * I: An existing and initailized UArray2_T object, and position variables
 *    i and j. i and j are greater than 0 and less than the width and
 *    height of the given UArray2_T respectively
 * O: An void pointer that points to the integer at position [i, j]
 */
void *UArray2_at(UArray2_T uarray2, int i, int j)
{
    assert(uarray2);
    assert(i >= 0 && j >= 0);
    assert(i < UArray2_width(uarray2) && j < UArray2_height(uarray2));
    return UArray_at(*(UArray_T *)UArray_at(uarray2->rowArray, i), j);
}

/* Purpose: UArray2_map_row_major applies a certain function to all of the
 *          elements within a given UArray2_T object, iterating through the
 *          object one row at a time.
 * I: An existing and initailized UArray2_T object, an apply function that
 *    takes in the parameters specified below, a void pointer to carry certain
 *    values over between function calls
 * O: N/A
 */
void UArray2_map_row_major(UArray2_T uarray2,
                           void apply(int i, int j, UArray2_T uarray2,
                           void *elem, void *cl), void *cl)
{
    assert(uarray2);
    int i, j;       // [i, j] represents [row position, col position]
    for (j = 0; j < uarray2->height; j++) {
        for (i = 0; i < uarray2->width; i++) {
            /* col position is getting bigger faster */
            apply(i, j, uarray2, UArray2_at(uarray2, i, j), cl);
        }
    }
}

/* Purpose: UArray2_map_col_major applies a certain function to all of the
 *          elements within a given UArray2_T object, iterating through the
 *          object one column at a time.
 * I: An existing and initailized UArray2_T object, an apply function that
 *    takes in the parameters specified below, a void pointer to carry certain
 *    values over between function calls
 * O: N/A
 */
void UArray2_map_col_major(UArray2_T uarray2,
                           void apply(int i, int j, UArray2_T uarray2,
                           void *elem, void *cl), void *cl)
{
    assert(uarray2);
    int i, j;       // [i, j] represents [row position, col position]
    for (i = 0; i < uarray2->width; i++) {
        for (j = 0; j < uarray2->height; j++) {
            /* row position is getting bigger faster */
            apply(i, j, uarray2, UArray2_at(uarray2, i, j), cl);
        }
    }
}
