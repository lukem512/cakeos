/* CakeOS */

#ifndef PAGING_H
#define PAGING_H

typedef struct page
{
    unsigned int present    : 1;   // Page present in memory
    unsigned int rw         : 1;   // Read-only if clear, readwrite if set
    unsigned int user       : 1;   // Supervisor level only if clear
    unsigned int accessed   : 1;   // Has the page been accessed since last refresh?
    unsigned int dirty      : 1;   // Has the page been written to since last refresh?
    unsigned int unused     : 7;   // Amalgamation of unused and reserved bits
    unsigned int frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    /**
       Array of pointers to pagetables.
    **/
    page_table_t *tables[1024];
    /**
       Array of pointers to the pagetables above, but gives their *physical*
       location, for loading into the CR3 register.
    **/
    unsigned int tablesPhysical[1024];

    /**
       The physical address of tablesPhysical. This comes into play
       when we get our kernel heap allocated and the directory
       may be in a different location in virtual memory.
    **/
    unsigned int physicalAddr;
} page_directory_t;

#include <system.h>

/**
   Sets up the environment, page directories etc and
   enables paging.
**/
void init_paging(unsigned int memorysz);

/**
   Causes the specified page directory to be loaded into the
   CR3 register.
**/
void switch_page_directory(page_directory_t *new);

/**
   Retrieves a pointer to the page required.
   If make == 1, if the page-table in which this page should
   reside isn't created, create it!
**/
page_t *get_page(u32int address, int make, page_directory_t *dir);

/**
   Handler for page faults.
**/
void page_fault(registers_t* r);

/**
   Makes a copy of a page directory.
**/
page_directory_t *clone_directory(page_directory_t *src);

/**
   Allocates a physical frame
**/
void alloc_frame(page_t *page, int is_kernel, int is_writeable);

/**
   Frees frame
**/
void free_frame(page_t *page);

/**
   Maps physical pages
**/
void map_pages(long addr, long size, int rw, int user);

/**
   Maps virtual pages
**/
void virtual_map_pages(long addr, long size, int rw, int user);

#endif
