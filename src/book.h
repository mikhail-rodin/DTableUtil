/*
data structs and functions for working with 3D arrays
of type book_t and string buffers
*/
#ifndef BOOK_H
#define BOOK_H
#include <stdlib.h>

typedef struct book_t
{   
    // array of rectangular (row_count*column_count) matrices
    float ***start;            //points to sheet 1, row 1, col 1
    size_t* columns_allocated; // points to an array, value for each sheet
    size_t* columns_used;      // array
    size_t* rows_allocated;    // array
    size_t* rows_used;         // array
    size_t* sheet_count;       // single value; 
        //sheets are allocated on demand, so sheets_allocated = sheets_used
} book_t;


typedef struct stringBuffer_t
{
    char **start;
    size_t* line_length;
    size_t* line_count;
} stringBuffer_t;

book_t book_init(size_t sheets, size_t rows, size_t columns);
book_t book_addSheet(book_t book, size_t columns, size_t rows);
book_t book_expandSheet(book_t book, size_t sheet_index, size_t columns, size_t rows);
void book_delete(book_t book);

stringBuffer_t strBuf_init(size_t line_length, size_t line_count);
stringBuffer_t strBuf_resize(stringBuffer_t buffer, size_t line_length, size_t line_count);
void strBuf_delete(stringBuffer_t buffer);

#endif