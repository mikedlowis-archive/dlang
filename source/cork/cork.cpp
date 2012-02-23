#include "cork.h"

#ifdef DETECT_MEM_LEAKS

// We want to use the real malloc and free in this file
#undef malloc
#undef free

#include <iostream>
#include <exception> // for std::bad_alloc
#include <cstdlib> // for malloc() and free()
#include <string.h>

// Set the namespace
using namespace std;
/******************************************************************************
 * Typedefs
 *****************************************************************************/
typedef struct BlockTableEntry
{
    void * ptr;
    unsigned int size;
    const char* file;
    int line;
    void * next;
} BlockTableEntry_T;

typedef struct BlockTable
{
    unsigned int size;
    BlockTableEntry_T* blocks[TBL_SIZE];
} BlockTable_T;

/******************************************************************************
 * Prototypes
 *****************************************************************************/
void insert_record(void * ptr, BlockTable_T* entry);
void erase_record(void * ptr);

/******************************************************************************
 * Globals
 *****************************************************************************/
unsigned int allocated;
static BlockTable_T Block_Table = { 0, {0} };

/******************************************************************************
 * Function Definitions
 *****************************************************************************/
void insert_record(void * ptr, BlockTableEntry_T* entry)
{
    unsigned int index = ((unsigned int)ptr) % TBL_SIZE;
    BlockTableEntry_T* last  = Block_Table.blocks[ index ];
    BlockTableEntry_T* curr  = last;

    while (curr != NULL)
    {
        if ( curr->ptr == ptr )
        {
            curr->size = entry->size;
            free(entry);
            break;
        }
        last = curr;
        curr = (BlockTableEntry_T*)curr->next;
    }

    if(curr == NULL)
    {
        if (last != NULL)
        {
            last->next = entry;
        }
        else
        {
            Block_Table.blocks[index] = entry;
        }
        Block_Table.size++;
    }
}

void erase_record(void * ptr)
{
    int depth = 0;
    unsigned int index = ((unsigned int)ptr) % TBL_SIZE;
    BlockTableEntry_T* last = Block_Table.blocks[ index ];
    BlockTableEntry_T* curr = last;

    while( curr != NULL )
    {
        depth = 1;
        if( curr->ptr == ptr )
        {
            depth = 2;
            if(last == curr)
            {
                depth = 3;
                Block_Table.blocks[ index ] = (BlockTableEntry_T*)curr->next;
            }
            else
            {
                depth = 4;
                last->next = curr->next;
            }
            free(curr);
            Block_Table.size--;
            break;
        }
        last = curr;
        curr = (BlockTableEntry_T*)curr->next;
    }
}

void Cork_ReportMemoryLeaks(void)
{
    unsigned int index = 0;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "Cork: Memory Allocation Analysis" << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "You have " << Block_Table.size << " Unclaimed objects." << endl;

    for(; index < TBL_SIZE; index++)
    {
        BlockTableEntry_T* entry = Block_Table.blocks[ index ];
        while(entry != NULL)
        {
            cout << "\t" << entry->size << "\t" << entry->ptr;
            if( entry->file != NULL )
            {
                cout << "\t" << entry->line << "\t" << entry->file;
            }
            cout << endl;
            entry = (BlockTableEntry_T*)entry->next;
        }
    }
}

void * operator new (size_t size, string file, unsigned int line)
{
    void * ptr = malloc(size);
    char * fname = (char*)malloc(file.length());
    if(ptr == NULL)
    {
        throw bad_alloc();
    }
    else
    {
        BlockTableEntry_T* entry = (BlockTableEntry_T*)malloc(sizeof(BlockTableEntry_T));
        strcpy( fname, file.c_str() );
        entry->ptr = ptr;
        entry->size = size;
        entry->file = fname;
        entry->line = line;
        entry->next = NULL;
        insert_record(ptr,entry);
    }
    return ptr;
}

void * operator new(size_t size) throw(bad_alloc) {
    void * ptr = malloc(size);
    if(ptr == NULL)
    {
        throw bad_alloc();
    }
    else
    {
        BlockTableEntry_T* entry = (BlockTableEntry_T*)malloc(sizeof(BlockTableEntry_T));
        entry->ptr = ptr;
        entry->size = size;
        entry->file = NULL;
        entry->line = 0;
        entry->next = NULL;
        insert_record(ptr,entry);
    }
    return ptr;
}

void operator delete(void * p) {
    free(p);
    erase_record(p);
}

#endif
