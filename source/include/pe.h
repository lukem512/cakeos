/* CakeOS */

#ifndef PE_H
#define PE_H

typedef struct _IMAGE_DOS_HEADER {  // DOS .EXE header
    unsigned short e_magic;         // Magic number (Should be MZ)
    unsigned short e_cblp;          // Bytes on last page of file
    unsigned short e_cp;            // Pages in file
    unsigned short e_crlc;          // Relocations
    unsigned short e_cparhdr;       // Size of header in paragraphs
    unsigned short e_minalloc;      // Minimum extra paragraphs needed
    unsigned short e_maxalloc;      // Maximum extra paragraphs needed
    unsigned short e_ss;            // Initial (relative) SS value
    unsigned short e_sp;            // Initial SP value
    unsigned short e_csum;          // Checksum
    unsigned short e_ip;            // Initial IP value
    unsigned short e_cs;            // Initial (relative) CS value
    unsigned short e_lfarlc;        // File address of relocation table
    unsigned short e_ovno;          // Overlay number
    unsigned short e_res[4];        // Reserved words
    unsigned short e_oemid;         // OEM identifier (for e_oeminfo)
    unsigned short e_oeminfo;       // OEM information; e_oemid specific
    unsigned short e_res2[10];      // Reserved words
    long   e_lfanew;        // File address of new exe header
} PEHeader_t, *PPEHeader_t;

typedef struct _IMAGE_FILE_HEADER {
    unsigned short  Machine;
    unsigned short  NumberOfSections;
    unsigned long   TimeDateStamp;
    unsigned long   PointerToSymbolTable;
    unsigned long   NumberOfSymbols;
    unsigned short  SizeOfOptionalHeader;
    unsigned short  Characteristics;
} PEFileHeader_t, *PPEFileHeader_t;

typedef struct _IMAGE_OPTIONAL_HEADER {
    unsigned short  Magic;
    unsigned char   MajorLinkerVersion;
    unsigned char   MinorLinkerVersion;
    unsigned long   SizeOfCode;
    unsigned long   SizeOfInitializedData;
    unsigned long   SizeOfUninitializedData;
    unsigned long   AddressOfEntryPoint;			//This is where we call our program
    unsigned long   BaseOfCode;
    unsigned long   BaseOfData;
    //A few more NT fields here
} PEOptionalHeader_t, *PPEOptionalHeader_t;

#endif
