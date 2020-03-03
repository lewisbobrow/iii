#include <stdlib.h>
#include <stdio.h>
#include <pnmrdr.h>
#include <limits.h>
#include "bit2.h"
#include "assert.h"

/* Stack structure used to manage a large amount of operations.
 * Recursion results in stack overflow for very large bit maps
 */
struct Stack {
    int head;
    int max;
    int* array;
};

/* * * * * * * * * * * Function Declarations * * * * * * * * * * */
void pbmread (int i, int j, Bit2_T map, int elem, void *cl);
void store_edges(Bit2_T image, struct Stack* blackedges);
void unblack_edges(Bit2_T image, struct Stack* blackedges);
void pbmwrite(FILE *outputfp, Bit2_T bitmap);
void translate(int i, int j, Bit2_T bit2, int elem, void *cl);
struct Stack* createStack(unsigned max);
void freeStack(struct Stack *array);
void push(struct Stack* blackedges, int elem);
int pop(struct Stack* blackedges);
int isEmpty(struct Stack* blackedges);
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
    }
    else exit(EXIT_FAILURE);

    /* ensuring that the Pnmrdr_T object is properly formatted */
    Pnmrdr_mapdata data = Pnmrdr_data(reader);
    assert(data.type == Pnmrdr_bit);
    assert(data.width != 0);
    assert(data.height != 0);

    /* creating a Bit2_T object and initializing
     * it with the bit values from reader
     */
    Bit2_T bitmap = Bit2_new(data.width, data.height);
    Bit2_map_row_major(bitmap, pbmread, reader);

    /* using a stack to store all the black edge bits that need
     * to be unblacked and then unblacking them
     */
    struct Stack* blackedges = createStack(bitmap->width * bitmap->height);
    store_edges(bitmap, blackedges);
    unblack_edges(bitmap, blackedges);

    /* printing every bit of the bitmap to stdout*/
    pbmwrite(stdout, bitmap);

    /* freeing objects and structures, and closing the input file */
    Bit2_free(&bitmap);
    Pnmrdr_free(&reader);
    freeStack(blackedges);
    fclose(fp);

    exit(EXIT_SUCCESS);
}

/* Purpose: pbmread is used as the apply function in map_row_major. Gets the
 *          value of the next bit in the bit map from the Pnmrdr_T object and
 *          applies it to the specified position in the Bit2_T object
 * I: An existing and initailized Bit2_T object, two integers that are both
 *    nonnegative and less than corresponding width and height properties of
 *    the Bit2_T object, an integer representing the stored value at the
 *    [i, j] position of the bitmap, and a void * to any object. i and j
 *    are nonnegative and less than the Bit2_T's width and height respectively
 * O: N/A
 */
void pbmread (int i, int j, Bit2_T map, int elem, void *cl)
{
    (void) elem;
    assert(i >= 0 && j >= 0);
    assert(i < map->width && j < map->height);
    assert(map);
    Bit2_put(map, i, j, Pnmrdr_get(cl));
}

/* Purpose: store_edges stores all of the black edge pixels in a given image in
 *          a Stack
 * I: An existing and initialized Bit2_T object, a Stack pointer for holding
 *    black edge pixels. The Bit2_T object represents a given image
 * O: N/A
 */
void store_edges(Bit2_T image, struct Stack* blackedges)
{
    assert(image);
    int i, j;
    for (i = 0; i < Bit2_width(image); i++) {
        if(Bit2_get(image, i, 0) == 1) push(blackedges, i);

        if (Bit2_get(image, i, Bit2_height(image) - 1) == 1)
            push(blackedges, 
                 ((Bit2_height(image) - 1) * Bit2_width(image)) + i);
    }

    for (j = 0; j < Bit2_height(image); j++) {
        if (Bit2_get(image, 0, j) == 1)
            push(blackedges, (Bit2_width(image) * j));

        if (Bit2_get(image, (Bit2_width(image) - 1), j) == 1)
            push(blackedges, 
                 (Bit2_width(image) * j) + (Bit2_width(image) - 1));
    }
}

/* Purpose: unblack_edges changes any black edge pixel in an image to a white
 *          pixel, by changing a pixel in the Stack to white, checking if its
 *          neighbors are also black, and adding them to the Stack if so
 * I: An existing and initialized Bit2_T object, A Stack pointer for holding
 *    black edge pixels. The Bit2_T object represents a given image
 * O: N/A
 */
void unblack_edges(Bit2_T image, struct Stack* blackedges)
{
    assert(image);
    while (isEmpty(blackedges) == 0) {
        int cur = pop(blackedges);
        Bit_put(image->array, cur, 0);

        // Checks if the pixel to the right (if there is one) is black
        // Adds to the Stack if so
        if (cur % Bit2_width(image) < Bit2_width(image) - 1) {
            if (Bit_get(image->array, cur + 1) == 1) push(blackedges, cur + 1);
        }

        // Checks if the pixel to the left (if there is one) is black
        // Adds to the Stack if so
        if (cur % Bit2_width(image) > 0) {
            if (Bit_get(image->array, cur - 1) == 1) push(blackedges, cur - 1);
        }

        // Checks if the pixel below (if there is one) is black
        // Adds to the Stack if so
        if (cur < Bit_length(image->array) - Bit2_width(image) - 1) {
            if (Bit_get(image->array, cur + Bit2_width(image)) == 1)
                push(blackedges, cur + Bit2_width(image));
        }

        // Checks if the pixel to above (if there is one) is black
        // Adds to the Stack if so
        if (cur >= Bit2_width(image)) {
            if (Bit_get(image->array, cur - Bit2_width(image)) == 1)
                push(blackedges, cur - Bit2_width(image));
        }
    }
}

/* Purpose: pbmwrite prints the new pbm file, with its edge pixels
 *          unblackened, as well as its information, to a given output
 * I: An output file/stdout, an existing and initialized Bit2_T object.
 *    The Bit2_T object represents a given image/pbm file.
 * O: N/A
 */
void pbmwrite(FILE *outputfp, Bit2_T bitmap)
{
    assert(bitmap);
    fputs("P1\n", outputfp);    // specifies pbm filetype
    int width = Bit2_width(bitmap);
    int height = Bit2_height(bitmap);
    fprintf(outputfp, "%d %d\n", width, height);   // specifies pbm dimensions
    Bit2_map_row_major(bitmap, translate, outputfp);    //prints pixels
    fclose(outputfp);
}

/* Purpose: translate prints the individual pixels of a given image to a
 *          specified output.
 * I: A position represented by [i, j], an existing and initialized Bit2_T
 *    object, an int representing an individual pixel to print out, a void
 *    pointer representing the specified output
 * O: N/A
 */
void translate(int i, int j, Bit2_T bit2, int elem, void *cl)
{
    assert(bit2);
    (void) j;
    fprintf(cl, "%d", elem);    // prints an individual pixel
    if (i == Bit2_width(bit2) - 1) fputc(10, cl);   // adds new line
    else fputc(32, cl);     // adds space
}

/* Purpose: createStack creates a new Stack object and initializes its maximum
 *          length, its head element, and the memory for the Stack itself
 * I: An unsigned int representing the maximum length of the Stack
 * O: A pointer to a new Stack object
 */
struct Stack* createStack(unsigned max)
{
    struct Stack* blackedges = (struct Stack*)malloc(sizeof(struct Stack));
    blackedges->max = max;
    blackedges->head = -1;
    blackedges->array = (int*)malloc(blackedges->max * sizeof(int));
    return blackedges;
}

/* Purpose: freeStack frees the memory allocated for a given Stack object
 * I: A pointer to an existing and initialized Stack object
 * O: N/A
 */
void freeStack(struct Stack *blackedges)
{
    assert(blackedges);
    free(blackedges->array);
    free(blackedges);
}

/* Purpose: isEmpty checks if a given Stack object is empty (has no stored
 *          elems)
 * I: A pointer to an existing and initialized Stack object
 * O: An integer representing whether or not the Stack is empty (1 = y, 0 = n)
 */
int isEmpty(struct Stack* blackedges)
{
    assert(blackedges);
    return blackedges->head == -1;
}

/* Purpose: isFull checks if a given Stack object is full (has used up its
 *          existing memory)
 * I: A pointer to an existing and initialized Stack object
 * O: An integer representing whether or not the Stack is full (1 = y, 0 = n)
 */
int isFull(struct Stack* blackedges)
{
    assert(blackedges);
    return blackedges->head == blackedges->max - 1;
}

/* Purpose: push inserts a new element as the first element in a given Stack
 *          object, if the Stack isn't already full
 * I: A pointer to an existing and initialized Stack object, an integer to be
 *    inserted
 * O: N/A
 */
void push(struct Stack* blackedges, int elem)
{
    assert(blackedges);
    if (isFull(blackedges) == 0) blackedges->array[++blackedges->head] = elem;
}

/* Purpose: pop removes the first element in a given Stack object if the Stack
 *          isn't already empty, and returns it to the user.
 * I: A pointer to an existing and initialized Stack object
 * O: The integer that was removed from the Stack
 */
int pop(struct Stack* blackedges)
{
    assert(blackedges);
    int result = -1;
    if (isEmpty(blackedges) == 0) {
        result = blackedges->array[blackedges->head];
        blackedges->head -= 1;
    }
    return result;
}
