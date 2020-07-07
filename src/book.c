// implements functions declared in book.h
#include "book.h"
#include "constants.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

book_t book_init(size_t sheets, size_t rows, size_t columns)
{
/* if memory allocated sucessfully, returns a struct with a set of pointers.
it not, sets corresponding size values (columns_allocated[i] and rows_allocated[j])
to 0 so that the code domwnstream does not attempt to write there.
*/
    int sheet_i =0, col_i=0;
    book_t book;

    book.sheet_count=(size_t*)malloc(sizeof(size_t)); // one value, not an array
    // create arrays for storing size (j*k) of each sheet
    book.columns_allocated=(size_t*)malloc(sheets*sizeof(size_t));
    book.columns_used=(size_t*)malloc(sheets*sizeof(size_t));
    book.rows_allocated=(size_t*)malloc(sheets*sizeof(size_t));
    book.rows_used=(size_t*)malloc(sheets*sizeof(size_t));

    for(sheet_i=0;sheet_i<sheets;sheet_i++)
    {
        book.columns_used[sheet_i]=0;
        book.rows_used[sheet_i]=0;
    }

    book.start=(float***)malloc(sheets*sizeof(float**)); //array of sheets
    for(sheet_i=0;sheet_i<sheets;sheet_i++)
    {
        //array of columns on every sheet_i
        if((book.start[sheet_i]=(float**)malloc(columns*sizeof(float*)))==NULL)
        {
            book.columns_allocated[sheet_i]=0;
            #ifdef DEBUG
            printf("Couldn't allocate memory for column %i in sheet %i", &col_i, &sheet_i);
            #endif
        }
        else{
            book.columns_allocated[sheet_i]=columns;
        };
        book.rows_allocated[sheet_i]=rows; // first assume memory is OK
        for(col_i=0; col_i<columns; col_i++)
        {
            // an array of rows for every column on every sheet_i
            if((book.start[sheet_i][col_i]=(float*)calloc(rows, sizeof(float)))==NULL)
            {
                // if at least one column is unallocated, then the whole sheet_i is marked as blank
                book.rows_allocated[sheet_i]=0;
            };
        };
    };
    return book;
};

book_t book_addSheet(book_t book, size_t columns, size_t rows)
{
    int col_i=0, row_i=0;
    size_t new_sheet_count = *book.sheet_count+1;
    
    // NB: last sheet index is new_sheet_count-1

    book.sheet_count=(size_t*)malloc(sizeof(size_t));
    *book.sheet_count=new_sheet_count;

    book.columns_allocated=
        (size_t*)realloc(book.columns_allocated, new_sheet_count*sizeof(size_t));
    book.columns_allocated[new_sheet_count-1]=columns; // -1 because it's zero based
   
    book.columns_used=
        (size_t*)realloc(book.columns_used, new_sheet_count*sizeof(size_t));
    book.columns_used[new_sheet_count-1]=0;

    book.rows_allocated=
        (size_t*)realloc(book.rows_allocated, new_sheet_count*sizeof(size_t));
    book.rows_allocated[new_sheet_count-1]=rows;
    book.rows_used=
        (size_t*)realloc(book.rows_used, new_sheet_count*sizeof(size_t));
    book.rows_used[new_sheet_count-1]=0;


    if((book.start=
        (float***)realloc(book.start, 
                        new_sheet_count*sizeof(float**)))==NULL)
    {
        #ifdef DEBUG
        printf("Couldn't reallocate memory");
        #endif
    }
    else
    {
        if((book.start[new_sheet_count-1]=
            (float**)malloc(columns*sizeof(float*)))==NULL)
        {
            #ifdef DEBUG
            printf("Couldn't reallocate memory");
            #endif
        }
        else
        {
            for(col_i=0; col_i<columns; col_i++)
            {
                if((book.start[new_sheet_count-1][col_i]=
                    (float*)malloc(rows*sizeof(float)))==NULL)
                {
                    #ifdef DEBUG
                    printf("Couldn't reallocate memory");
                    #endif
                }
            }
        }
    }
    return book;
};

book_t book_expandSheet(book_t book, size_t sheet_index, size_t columns, size_t rows)
{
    int col_i = 0;
    if(sheet_index >= book.sheet_count)
    {
        // no sheet at index
    }
    if(columns <= book.columns_allocated[sheet_index] | 
        rows <= book.rows_allocated[sheet_index])
    {
        // can't shrink a sheet
    }
    if((book.start[sheet_index]=
       (float**)realloc(book.start[sheet_index],columns*sizeof(float*)))==NULL)
    {
        #ifdef DEBUG
        printf("Couldn't reallocate memory");
        #endif
    }
    else
    {
        book.columns_allocated[sheet_index]=columns;
        for(col_i=0; col_i<book.columns_allocated[sheet_index];col_i++)
        {
            if((book.start[sheet_index][col_i]=
                (float*)realloc(book.start[sheet_index][col_i], 
                rows*sizeof(float)))==NULL)
            {
                #ifdef DEBUG
                printf("Couldn't reallocate memory");
                break;
                exit;
                #endif
            }
        }
        book.rows_allocated[sheet_index]=rows;
        return book;
    }
};

void book_delete(book_t book)
{
    int sheet_i=0, col_i=0;

    for(sheet_i=0; sheet_i<*book.sheet_count; sheet_i++)
    {
        // first delete all the cols on this sheet
        for(col_i=0; col_i<book.columns_allocated[sheet_i]; col_i++)
        {
            free(book.start[sheet_i][col_i]);
        }

        // now delete the sheet (col array) itself
        free(book.start[sheet_i]);
    }
    free(book.start);
    return 0;
}

stringBuffer_t strBuf_init(size_t line_length, size_t line_count)
{
    stringBuffer_t buf;
    buf.start = NULL;
    buf.line_count=0;
    buf.line_length=0;

    int line_i=0;

    if((buf.line_count=malloc(sizeof(size_t)))==NULL)
    {
        #ifdef DEBUG
        printf("Couldn't allocate memory for line_count");
        #endif
        exit(1);
    }
    if((buf.line_length=malloc(sizeof(size_t)))==NULL)
    {
        #ifdef DEBUG
        printf("Couldn't allocate memory for line_length");
        #endif
        exit(1);
    }
    if((buf.start=malloc(line_count*sizeof(char*)))==NULL)
    {
        #ifdef DEBUG
        printf("Couldn't allocate memory for string buffer");
        #endif
    }
    else
    {
        *buf.line_count=line_count;
        for(line_i=0; line_i<line_count; line_i++)
        {
            if((buf.start[line_i]=malloc(line_length*sizeof(char)))==NULL)
            {
                #ifdef DEBUG
                printf("Couldn't allocate memory for line %i", &line_i);
                #endif
                break;
                exit(1);
            }
        }
        *buf.line_length=line_length;
    }
    return buf;
}

stringBuffer_t strBuf_resize(stringBuffer_t buffer, size_t line_length, size_t line_count)
{
    // pass a small line length so that individual lines don't get reallocated
    if((buffer.start=realloc(buffer.start, line_count*sizeof(char*)))==NULL)
    {
        #ifdef DEBUG
        printf("Couldn't reallocate memory for a buffer");
        #endif
        exit(1);
    }
    else
    {
        *buffer.line_count=line_count;
        if(line_length>*buffer.line_length)
        {
            for(size_t line_i=0; line_i<*buffer.line_count; line_i++)
            {
                buffer.start[line_i]=
                    realloc(buffer.start[line_i], line_length*sizeof(char));
            }
        }
    }
    return buffer;
}

void strBuf_delete(stringBuffer_t buffer)
{
    for(size_t line_i=0; line_i<*buffer.line_count; line_i++)
    {
        free(buffer.start[line_i]);
    }
    free(buffer.start);
    return 0;
}