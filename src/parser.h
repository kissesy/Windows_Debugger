#ifndef __H_PARSER_
#define __H_PARSER_

#define IMAGE_SIZEOF_SHORT_NAME 8
#include <stdio.h>

//#pragma pack(push, 1)

typedef struct _DOS_Header 
 {
// short is 2 bytes, long is 4 bytes
     char signature[2];
     short lastsize;
     short nblocks;
     short nreloc;
     short hdrsize;
     short minalloc;
     unsigned short maxalloc;
     short ss; // 2 byte value
     short sp; // 2 byte value
     short checksum;
     short ip; // 2 byte value
     short cs; // 2 byte value
     short relocpos;
     short noverlay;
     short reserved1[4];
     short oem_id;
     short oem_info;
     short reserved2[10];
     long  e_lfanew; // Offset to the 'PE\0\0' signature relative to the beginning of the file
 }DOS_Header;

typedef struct _Print_DOS_HEADER
{
	unsigned short imformation[30]; 
	long e_lfanew; 
}Print_DOS_Header; 

typedef struct _COFF_Header
 {
	long signature;             
    short Machine;
    short NumberOfSections; 
    long TimeDateStamp; 
    long PointerToSymbolTable; 
    long NumberOfSymbols; 
    short SizeOfOptionalHeader; 
    short Characteristics;
 }COFF_Header; 

typedef struct _data_directory
{
	long VirtualAddress;
	long Size; 
}date_directory;



//////////////////////////////////////64bit////////////////////////////////////////////
typedef struct _64PE_OptHeader
 {
/* 64 bit version of the PE Optional Header also known as IMAGE_OPTIONAL_HEADER64
char is 1 byte
short is 2 bytes
long is 4 bytes
long long is 8 bytes
*/
    short signature; //decimal number 267 for 32 bit, 523 for 64 bit, and 263 for a ROM image. 
    char MajorLinkerVersion; 
    char MinorLinkerVersion;
    long SizeOfCode;
    long SizeOfInitializedData;
    long SizeOfUninitializedData;
    long AddressOfEntryPoint;  //The RVA of the code entry point
    long BaseOfCode;
    /*The next 21 fields are an extension to the COFF optional header format*/
    long long ImageBase;
    long SectionAlignment;
    long FileAlignment;
    short MajorOSVersion;
    short MinorOSVersion;
    short MajorImageVersion;
    short MinorImageVersion;
    short MajorSubsystemVersion;
    short MinorSubsystemVersion;
    long Win32VersionValue;
    long SizeOfImage;
    long SizeOfHeaders;
    long Checksum;
    short Subsystem;
    short DLLCharacteristics;
    long long SizeOfStackReserve;
    long long SizeOfStackCommit;
    long long SizeOfHeapReserve;
    long long SizeOfHeapCommit;
    long LoaderFlags;
    long NumberOfRvaAndSizes;
    struct _data_directory* DataDirectory;     //Can have any number of elements, matching the number in NumberOfRvaAndSizes.
 }PE_OptHeader64;                                        //However, it is always 16 in PE files.
///////////////////////////////32bit/////////////////////////////////////////////
typedef struct _PE_OptHeader
 {
/* 32 bit version of the PE Optional Header also known as IMAGE_OPTIONAL_HEADER
char is 1 byte
short is 2 bytes
long is 4 bytes
*/
    short signature; //decimal number 267 for 32 bit, 523 for 64 bit, and 263 for a ROM image. 
    char MajorLinkerVersion; 
    char MinorLinkerVersion;
    long SizeOfCode;
    long SizeOfInitializedData;
    long SizeOfUninitializedData;
    long AddressOfEntryPoint;  //The RVA of the code entry point
    long BaseOfCode;
    long BaseOfData;
    /*The next 21 fields are an extension to the COFF optional header format*/
    long ImageBase;
    long SectionAlignment;
    long FileAlignment;
    short MajorOSVersion;
    short MinorOSVersion;
    short MajorImageVersion;
    short MinorImageVersion;
    short MajorSubsystemVersion;
    short MinorSubsystemVersion;
    long Win32VersionValue;
    long SizeOfImage;
    long SizeOfHeaders;
    long Checksum;
    short Subsystem;
    short DLLCharacteristics;
    long SizeOfStackReserve;
    long SizeOfStackCommit;
    long SizeOfHeapReserve;
    long SizeOfHeapCommit;
    long LoaderFlags;
    long NumberOfRvaAndSizes;
    struct _data_directory* DataDirectory; //malloc sizeof(struct _data_directory)* NumberOfRvaAndSizes; 
	//struct _data_directory DataDirectory[NumberOfRvaAndSizes];     //Can have any number of elements, matching the number in NumberOfRvaAndSizes.
 }PE_OptHeader;                                        //However, it is always 16 in PE files.

typedef struct _IMAGE_SECTION_HEADER 
 {
// short is 2 bytes
// long is 4 bytes
  char Name[IMAGE_SIZEOF_SHORT_NAME]; // IMAGE_SIZEOF_SHORT_NAME is 8 bytes
  union {
    long PhysicalAddress;
    long VirtualSize;
  } Misc;
  long  VirtualAddress;
  long  SizeOfRawData;
  long  PointerToRawData;
  long  PointerToRelocations;
  long  PointerToLinenumbers;
  short NumberOfRelocations;
  short NumberOfLinenumbers;
  long  Characteristics;
 }IMAGE_SECTION_HEADER;

//#pragma pack(pop)

#endif


unsigned char* file_to_heap(const char* name, int* file_vol); 
void error_msg_print(const char* msg, int error_code); 
int control_function(const char* file_name); 
//int check_pe(FILE* file_pointer);
void pe_header_parser(FILE* , DOS_Header*, COFF_Header*, PE_OptHeader*, PE_OptHeader64*, int* );
void print_pe_format_imformation(FILE*, DOS_Header*, COFF_Header*, PE_OptHeader*, PE_OptHeader64*, IMAGE_SECTION_HEADER*, int);
void pe_section_parser(FILE* , IMAGE_SECTION_HEADER* , int );
