/*
 *      sudoku.c
 *      by Lewis Bobrow and John Stewart
 *      February 10th 2020
 *      Assignment: HW2 (iii)
 *
 *      This code declares and defines various functions used in the sudoku
 *      program. These include functions that store the values from the
 *      graymap input file into a UArray2_T object, and functions that check
 *      the validity of the sudoku puzzle.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>
#include "uarray2.h"
#include "assert.h"


/* struct to pass into closure variable of apply function
 * allowing both properties to be freed
 */
struct info {
    Pnmrdr_T reader;
    FILE *fp;
};

/* * * * * * * * * * * * * * Function Declarations * * * * * * * * * * * * * */
void store_pixel(int i, int j, UArray2_T uarray2, void *elem, void *cl);
void check_rows(int row, int col, UArray2_T uarray2, void *elem, void *cl);
void check_cols(int row, int col, UArray2_T uarray2, void *elem, void *cl);
void check_boxes(UArray2_T uarray2, struct info *imageInfo);
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char *argv[])
{
    Pnmrdr_T reader;
    FILE *fp;

    /* Creating a Pnmrdr_T object from the commandline argument if it exists,
     * otherwise create it from stdin
     */
    if (argc == 2) {
        fp = fopen(argv[1], "r");
        TRY
            reader = Pnmrdr_new(fp);
        EXCEPT(Pnmrdr_Badformat)
            fprintf(stderr, "Image is not the correct format\n");
            exit(EXIT_FAILURE);
        END_TRY;
    } else if (argc == 1) {
        TRY
            reader = Pnmrdr_new(stdin);
        EXCEPT(Pnmrdr_Badformat)
            fprintf(stderr, "Image is not the correct format\n");
            exit(EXIT_FAILURE);
        END_TRY;
    } else exit(EXIT_FAILURE);

    /* ensuring that the Pnmrdr_T object is a graymap and checking that
     * the properties of the object are correct
     */
    Pnmrdr_mapdata data = Pnmrdr_data(reader);
    assert(data.type == Pnmrdr_gray);
    if ((data.width != 9) || (data.height != 9)) {
        Pnmrdr_free(&reader);
        fclose(fp);
        exit(1);
    }
    if (data.denominator != 9) {
        Pnmrdr_free(&reader);
        fclose(fp);
        exit(1);
    }

    /* creating the UArray2 object and storing each value from the
     * graymap inside of it in the correct position.
     * Also create an imageinfo struct containing the FILE * and
     * the Pnmrdr to pass into map_row_major as the closure var.
     * This will allow us to free them if needed.
     */
    UArray2_T sudoku = UArray2_new(9, 9, sizeof(unsigned int));
    struct info* imageInfo = (struct info *) malloc(sizeof(struct info));
    imageInfo->reader = reader;
    imageInfo->fp = fp;
    UArray2_map_row_major(sudoku, store_pixel, imageInfo);

    /* checking that each row, col, and 3x3 box contains 9 distint numbers */
    UArray2_map_row_major(sudoku, check_rows, imageInfo);
    UArray2_map_col_major(sudoku, check_cols, imageInfo);
    check_boxes(sudoku, imageInfo);

    /* freeing the Pnmrdr_T and UArray2_T objects, the struct of the 
     * imageinfo, and closing the input file 
     */
    UArray2_free(&sudoku);
    Pnmrdr_free(&reader);
    free(imageInfo);
    fclose(fp);

    /* if the program has not output 1 at the end of main, then the graymap
     * is a solved sudoku puzzle, so exit with a code of 0
     */
    exit(0);
}

/* Purpose: store_pixel stores a given pixel within a pgm in the matching
 *          position in a UArray2_T object. It then checks if the pixel's
 *          intensity value is 0. If it is, store_pixel exit(1)'s.
 *          Frees cl's properties if exiting with code of 1.
 * I: A position represented by [i, j], an existing and initialized UArray2_T
 *    object, and a void pointer pointing to a Pnmrdr (represents pgm file)
 * O: N/A
 */
void store_pixel(int i, int j, UArray2_T uarray2, void *elem, void *cl)
{
    assert(uarray2);
    (void) elem;
    int temp = Pnmrdr_get(((struct info *)cl)->reader);
    *((int *)UArray2_at(uarray2, i, j)) = temp;
    if (*(int *)UArray2_at(uarray2, i, j) <= 0) {
        UArray2_free(&uarray2);
        Pnmrdr_free(&(((struct info *)cl)->reader));
        fclose(((struct info *)cl)->fp);
        free((struct info *)cl);
        exit(1);
    } 
}

/* Purpose: check_rows checks if the rows within a given Sudoku puzzle are
 *          solved. Solved means no two elements in a row are the same
 *          number. If a given row isn't solved, check_rows exit(1)'s.'
 *          Frees cl's properties if exiting with code of 1.
 * I: An existing and initialized UArray2_T object
 * O: N/A
 */
void check_rows(int row, int col, UArray2_T uarray2, void *elem, void *cl)
{
    assert(uarray2);
    int i;
    for (i = 0; i < uarray2->width; i++) {
        if (UArray2_at(uarray2, i, col) == elem) {
            if (i != row) {
                UArray2_free(&uarray2);
                Pnmrdr_free(&((struct info *)cl)->reader);
                fclose(((struct info *)cl)->fp);
                free((struct info *)cl);
                exit(1);
            } 
        }
    }
}

/* Purpose: check_cols checks if the columns within a given Sudoku puzzle are
 *          solved. Solved means no two elements in a column are the same
 *          number. If a given column isn't solved, check_cols exit(1)'s.'
*           Frees cl's properties if exiting with code of 1.
 * I: An existing and initialized UArray2_T object
 * O: N/A
 */
void check_cols(int row, int col, UArray2_T uarray2, void *elem, void *cl)
{
    assert(uarray2);
    int i;
    for (i = 0; i < uarray2->height; i++) {
        if (UArray2_at(uarray2, row, i) == elem) {
            if (i != col) {
                UArray2_free(&uarray2);
                Pnmrdr_free(&((struct info *)cl)->reader);
                fclose(((struct info *)cl)->fp);
                free((struct info *)cl);
                exit(1);
            }
        }
    }
}

/* Purpose: check_boxes checks if the 3x3 boxes within a given Sudoku puzzle
 *          are solved. Solved means no two elements in a box are the same
 *          number. If a given box isn't solved, check_boxes exit(1)'s.
*           Frees the FILE * and Pnmrdr if exiting with code of 1.
 * I: An existing and initialized UArray2_T object, and a struct containing
 *    a FILE * and a Pnmrdr (to be freed)
 * O: N/A
 */
void check_boxes(UArray2_T uarray2, struct info *imageInfo)
{
    assert(uarray2);
    int row, col, i , j, x, y;

    /* for loops and box checking are based on the
       top left element in each box */
    for (row = 0; row < 9; row += 3) {
        for (col = 0; col < 9; col += 3) {
            int box[9];     /* stores the values in a given box */
            for (i = row; i < row + 3; i++) {
                for (j = col; j < col + 3; j++)
                    box[(3 * (i - row)) + (j - col)]
                        = *((int *)UArray2_at(uarray2, i, j));
            }
            for (x = 0; x < 9; x++) {       /* compares elements in the box */
                for (y = 0; y < 9; y++) {
                    if ((box[y] == box[x]) && (x != y)) {
                        UArray2_free(&uarray2);
                        Pnmrdr_free(&(imageInfo->reader));
                        fclose(imageInfo->fp);
                        free(imageInfo);
                        exit(1);
                    }
                }
            }
        }
    }
}
