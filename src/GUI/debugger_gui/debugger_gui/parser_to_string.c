#include "parser.h"


void parser_to_string_pe32_header(char(*print_string)[100], Collect_Struct* collect_struct, HWND hWnd)
{
	int i = 0;
	int getval = 0;
	int offset = 0;
	collect_struct->file_offset = 0; 
	char* description_pe_optional_header[30] = { "Magic", "Major Linker Version", "Minor Linker Version", "Size of Code", "Size of Inialtalized Data", 
		"Size of Uninitalized Data", "Address of Entry Point", "Base of Code", "Base of Data", "Image Base", 
		"Section Alignment", "File Alignment", "Major O/S Version", "Minor O/S Version", "Major Image Version", 
		"Minor Image Version", "Major Subsystem Version", "Minor Subsystem Version", "Win32 Version Value", 
		"Size of Image", "Size of Headers", "Checksum", "Subsystemem", "DLL Characteristics", "Size of Stack Reverse", 
		"Size of Stack Commit", "SIze of Heap Reverse", "Size of Heap Commit", "Loader Flags", "Number of Data Directories" };
	for (i = 0; i < 100; i++)
	{
		memset(print_string[i], 0, 100);
	}
	char debug[20]; 
	for (i = 0; i < collect_struct->lines - collect_struct->pe_option_header.NumberOfRvaAndSizes; i++)
	{
		getval = pe32_header_getval(collect_struct, i, &offset); 
		sprintf(print_string[i], "%08X        %08X        %s", collect_struct->file_offset, getval, description_pe_optional_header[i]);
		collect_struct->file_offset += offset;
	}
	//MessageBox(hWnd, "2", "1", MB_OK);
	int index = 0;
	//MessageBox(hWnd, debug, "1", MB_OK);
	//strcpy(print_string[i], "pFile(Virtual)->pFile(Size)        Data(Virtual)->Data(Size)"); 
	for (i = collect_struct->lines - collect_struct->pe_option_header.NumberOfRvaAndSizes; i < collect_struct->lines; i++)
	{
		sprintf(print_string[i], "%08X   ->   %08X        %08X(Virtual)   ->   %08X(Size)        ",collect_struct->file_offset, collect_struct->file_offset+sizeof(collect_struct->pe_option_header.DataDirectory[index].VirtualAddress), collect_struct->pe_option_header.DataDirectory[index].VirtualAddress, collect_struct->pe_option_header.DataDirectory[index].Size);
		collect_struct->file_offset += sizeof(collect_struct->pe_option_header.DataDirectory[index].VirtualAddress) + sizeof(collect_struct->pe_option_header.DataDirectory[index].Size);
		index++; 
		//sprintf(debug, "%d", i);
		//MessageBox(hWnd, debug, "1", MB_OK);
	}
}

void parser_to_string_pe64_header(char(*print_string)[100], Collect_Struct* collect_struct, HWND hWnd)
{
	int getval = 0;
	int offset = 0;
	collect_struct->file_offset = 0;
	char* description_pe_optional_header[29] = { "Magic", "Major Linker Version", "Minor Linker Version", "Size of Code", "Size of Inialtalized Data",
		"Size of Uninitalized Data", "Address of Entry Point", "Base of Code", "Image Base",
		"Section Alignment", "File Alignment", "Major O/S Version", "Minor O/S Version", "Major Image Version",
		"Minor Image Version", "Major Subsystem Version", "Minor Subsystem Version", "Win32 Version Value",
		"Size of Image", "Size of Headers", "Checksum", "Subsystemem", "DLL Characteristics", "Size of Stack Reverse",
		"Size of Stack Commit", "SIze of Heap Reverse", "Size of Heap Commit", "Loader Flags", "Number of Data Directories" };
	for (int i = 0; i < 100; i++)
	{
		memset(print_string[i], 0, 100);
	}
	//char debug[20];
	for (int i = 0; i < collect_struct->lines - collect_struct->pe_option_header64.NumberOfRvaAndSizes; i++)
	{
		getval = pe32_header_getval(collect_struct, i, &offset);
		sprintf(print_string[i], "%08X        %08X        %s", collect_struct->file_offset, getval, description_pe_optional_header[i]);
		collect_struct->file_offset += offset;
	}
	//MessageBox(hWnd, "1", "1", MB_OK);
	int index = 0;
	
	for (int i = collect_struct->lines - collect_struct->pe_option_header64.NumberOfRvaAndSizes; i < collect_struct->lines; i++)
	{
		sprintf(print_string[i], "%08X   ->   %08X        %08X(Virtual)   ->   %08X(Size)        ", collect_struct->file_offset, collect_struct->file_offset + sizeof(collect_struct->pe_option_header64.DataDirectory[index].VirtualAddress), collect_struct->pe_option_header64.DataDirectory[index].VirtualAddress, collect_struct->pe_option_header64.DataDirectory[index].Size);
		collect_struct->file_offset += sizeof(collect_struct->pe_option_header64.DataDirectory[index].VirtualAddress) + sizeof(collect_struct->pe_option_header64.DataDirectory[index].Size);
		index++; 
		//sprintf(debug, "%d", index);
		//MessageBox(hWnd, debug, debug, MB_OK);
	}
}

void parser_to_string_coff_header(char(*print_string)[100], Collect_Struct* collect_struct, HWND hWnd)
{
	int getval = 0; 
	int offset = 0; 
	collect_struct->file_offset = 0; 
	char* description_coff_header[8] = { "Signature", "Machine", "Number of Sections", "Time Data Stamp", "Pointer to Symbol Table",
		"Number of Symbols", "Size of Optional Header", "Characteristics" };
	for (int i = 0; i < 100; i++)
	{
		memset(print_string[i], 0, 100); 
	}
	//MessageBox(hWnd, "3", "1", MB_OK);
	for (int i = 0; i < collect_struct->lines; i++)
	{
		getval = coff_header_getval(collect_struct, i, &offset);
		sprintf(print_string[i], "%08X        %08X        %s", collect_struct->file_offset, getval, description_coff_header[i]);
		collect_struct->file_offset += offset;
	}
}

void parser_to_string_dos_header(char(*print_string)[100], Collect_Struct* collect_struct,HWND hWnd)
{
	//MessageBox(hWnd, "5", "1", MB_OK);
	int getval = 0;
	int offset = 0;
	collect_struct->file_offset = 0;
	char* description_dos_header[31] = { "Signature", "Bytes on Last Page of File", "Pages in File", "Relocations",
		"Size of Header in Paragraphs", "Minimum Extra Paragraphs", "Maximum Extra Paragraphs", "Initial SS",
		"Initial SP", "Checksum", "Initial IP", "Initial CS", "Offset to Relocation Table", "Overlay Number",
		"Rserved", "Reserved", "Reserved", "Reserved", "OEM Identifier", "OEM Information", "Reserved", "Reserved",
		"Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Offset to New EXE Header" };
	//int file_offset = 0;
	//MessageBox(hWnd, "4", "1", MB_OK);
	for (int i = 0; i < 100; i++)
	{
		memset(print_string[i], 0, 100);
	}
	//MessageBox(hWnd, "1", "1", MB_OK);
	sprintf(print_string[0], "%08X        %X%X        %s", collect_struct->file_offset, collect_struct->dos_header.signature[0],collect_struct->dos_header.signature[1], description_dos_header[0]);
	collect_struct->file_offset += sizeof(collect_struct->dos_header.signature);
	//MessageBox(hWnd, "2", "1", MB_OK);
	for (int i = 1; i < collect_struct->lines; i++)
	{
		getval = dos_header_getval(collect_struct, i, &offset); 
		sprintf(print_string[i], "%08X        %04X        %s", collect_struct->file_offset, getval, description_dos_header[i]);
		collect_struct->file_offset += offset;
	}
	//MessageBox(hWnd, "3", "1", MB_OK);
}

int coff_header_getval(Collect_Struct* collect_struct, int n, int* offset)
{
	switch(n)
	{
		case 0: *offset = sizeof(collect_struct->coff_header.signature); return collect_struct->coff_header.signature;
		case 1: *offset = sizeof(collect_struct->coff_header.Machine); return collect_struct->coff_header.Machine;
		case 2: *offset = sizeof(collect_struct->coff_header.NumberOfSections); return collect_struct->coff_header.NumberOfSections;
		case 3: *offset = sizeof(collect_struct->coff_header.TimeDateStamp); return collect_struct->coff_header.TimeDateStamp;
		case 4: *offset = sizeof(collect_struct->coff_header.PointerToSymbolTable); return collect_struct->coff_header.PointerToSymbolTable;
		case 5: *offset = sizeof(collect_struct->coff_header.NumberOfSymbols); return collect_struct->coff_header.NumberOfSymbols;
		case 6: *offset = sizeof(collect_struct->coff_header.SizeOfOptionalHeader); return collect_struct->coff_header.SizeOfOptionalHeader;
		case 7: *offset = sizeof(collect_struct->coff_header.Characteristics); return collect_struct->coff_header.Characteristics;
	}
	return -1;
}

int pe64_header_getval(Collect_Struct* collect_struct, int n, int* offset)
{
	switch (n)
	{
	case 0: *offset = sizeof(collect_struct->pe_option_header64.signature); return collect_struct->pe_option_header.signature;
	case 1: *offset = sizeof(collect_struct->pe_option_header64.MajorLinkerVersion); return collect_struct->pe_option_header.MajorLinkerVersion;
	case 2: *offset = sizeof(collect_struct->pe_option_header64.MinorLinkerVersion); return collect_struct->pe_option_header.MinorLinkerVersion;
	case 3: *offset = sizeof(collect_struct->pe_option_header64.SizeOfCode); return collect_struct->pe_option_header.SizeOfCode;
	case 4: *offset = sizeof(collect_struct->pe_option_header64.SizeOfInitializedData); return collect_struct->pe_option_header.SizeOfInitializedData;
	case 5: *offset = sizeof(collect_struct->pe_option_header64.SizeOfUninitializedData); return collect_struct->pe_option_header.SizeOfUninitializedData;
	case 6: *offset = sizeof(collect_struct->pe_option_header64.AddressOfEntryPoint); return collect_struct->pe_option_header.AddressOfEntryPoint;
	case 7: *offset = sizeof(collect_struct->pe_option_header64.BaseOfCode); return collect_struct->pe_option_header.BaseOfCode;
	//case 8: *offset = sizeof(collect_struct->pe_option_header64.BaseOfData); return collect_struct->pe_option_header.BaseOfData;
	case 8: *offset = sizeof(collect_struct->pe_option_header64.ImageBase); return collect_struct->pe_option_header.ImageBase;
	case 9: *offset = sizeof(collect_struct->pe_option_header64.SectionAlignment); return collect_struct->pe_option_header.SectionAlignment;
	case 10: *offset = sizeof(collect_struct->pe_option_header64.FileAlignment); return collect_struct->pe_option_header.FileAlignment;
	case 11: *offset = sizeof(collect_struct->pe_option_header64.MajorOSVersion); return collect_struct->pe_option_header.MajorOSVersion;
	case 12: *offset = sizeof(collect_struct->pe_option_header64.MinorOSVersion); return collect_struct->pe_option_header.MinorOSVersion;
	case 13: *offset = sizeof(collect_struct->pe_option_header64.MajorImageVersion); return collect_struct->pe_option_header.MajorImageVersion;
	case 14: *offset = sizeof(collect_struct->pe_option_header64.MinorImageVersion); return collect_struct->pe_option_header.MinorImageVersion;
	case 15: *offset = sizeof(collect_struct->pe_option_header64.MajorSubsystemVersion); return collect_struct->pe_option_header.MajorSubsystemVersion;
	case 16: *offset = sizeof(collect_struct->pe_option_header64.MinorSubsystemVersion); return collect_struct->pe_option_header.MinorSubsystemVersion;
	case 17: *offset = sizeof(collect_struct->pe_option_header64.Win32VersionValue); return collect_struct->pe_option_header.Win32VersionValue;
	case 18: *offset = sizeof(collect_struct->pe_option_header64.SizeOfImage); return collect_struct->pe_option_header.SizeOfImage;
	case 19: *offset = sizeof(collect_struct->pe_option_header64.SizeOfHeaders); return collect_struct->pe_option_header.SizeOfHeaders;
	case 20: *offset = sizeof(collect_struct->pe_option_header64.Checksum); return collect_struct->pe_option_header.Checksum;
	case 21: *offset = sizeof(collect_struct->pe_option_header64.Subsystem); return collect_struct->pe_option_header.Subsystem;
	case 22: *offset = sizeof(collect_struct->pe_option_header64.DLLCharacteristics); return collect_struct->pe_option_header.DLLCharacteristics;
	case 23: *offset = sizeof(collect_struct->pe_option_header64.SizeOfStackReserve); return collect_struct->pe_option_header.SizeOfStackReserve;
	case 24: *offset = sizeof(collect_struct->pe_option_header64.SizeOfStackCommit); return collect_struct->pe_option_header.SizeOfStackCommit;
	case 25: *offset = sizeof(collect_struct->pe_option_header64.SizeOfHeapReserve); return collect_struct->pe_option_header.SizeOfHeapReserve;
	case 26: *offset = sizeof(collect_struct->pe_option_header64.SizeOfHeapCommit); return collect_struct->pe_option_header.SizeOfHeapCommit;
	case 27: *offset = sizeof(collect_struct->pe_option_header64.LoaderFlags); return collect_struct->pe_option_header.LoaderFlags;
	case 28: *offset = sizeof(collect_struct->pe_option_header64.NumberOfRvaAndSizes); return collect_struct->pe_option_header.NumberOfRvaAndSizes;
	case 29: *offset = sizeof(collect_struct->pe_option_header64.signature); return collect_struct->pe_option_header.signature;
	}
}

int pe32_header_getval(Collect_Struct* collect_struct, int n, int* offset)
{
	switch (n)
	{
		case 0: *offset = sizeof(collect_struct->pe_option_header.signature); return collect_struct->pe_option_header.signature;
		case 1: *offset = sizeof(collect_struct->pe_option_header.MajorLinkerVersion); return collect_struct->pe_option_header.MajorLinkerVersion;
		case 2: *offset = sizeof(collect_struct->pe_option_header.MinorLinkerVersion); return collect_struct->pe_option_header.MinorLinkerVersion;
		case 3: *offset = sizeof(collect_struct->pe_option_header.SizeOfCode); return collect_struct->pe_option_header.SizeOfCode;
		case 4: *offset = sizeof(collect_struct->pe_option_header.SizeOfInitializedData); return collect_struct->pe_option_header.SizeOfInitializedData;
		case 5: *offset = sizeof(collect_struct->pe_option_header.SizeOfUninitializedData); return collect_struct->pe_option_header.SizeOfUninitializedData;
		case 6: *offset = sizeof(collect_struct->pe_option_header.AddressOfEntryPoint); return collect_struct->pe_option_header.AddressOfEntryPoint;
		case 7: *offset = sizeof(collect_struct->pe_option_header.BaseOfCode); return collect_struct->pe_option_header.BaseOfCode;
		case 8: *offset = sizeof(collect_struct->pe_option_header.BaseOfData); return collect_struct->pe_option_header.BaseOfData;
		case 9: *offset = sizeof(collect_struct->pe_option_header.ImageBase); return collect_struct->pe_option_header.ImageBase;
		case 10: *offset = sizeof(collect_struct->pe_option_header.SectionAlignment); return collect_struct->pe_option_header.SectionAlignment;
		case 11: *offset = sizeof(collect_struct->pe_option_header.FileAlignment); return collect_struct->pe_option_header.FileAlignment;
		case 12: *offset = sizeof(collect_struct->pe_option_header.MajorOSVersion); return collect_struct->pe_option_header.MajorOSVersion;
		case 13: *offset = sizeof(collect_struct->pe_option_header.MinorOSVersion); return collect_struct->pe_option_header.MinorOSVersion;
		case 14: *offset = sizeof(collect_struct->pe_option_header.MajorImageVersion); return collect_struct->pe_option_header.MajorImageVersion;
		case 15: *offset = sizeof(collect_struct->pe_option_header.MinorImageVersion); return collect_struct->pe_option_header.MinorImageVersion;
		case 16: *offset = sizeof(collect_struct->pe_option_header.MajorSubsystemVersion); return collect_struct->pe_option_header.MajorSubsystemVersion;
		case 17: *offset = sizeof(collect_struct->pe_option_header.MinorSubsystemVersion); return collect_struct->pe_option_header.MinorSubsystemVersion;
		case 18: *offset = sizeof(collect_struct->pe_option_header.Win32VersionValue); return collect_struct->pe_option_header.Win32VersionValue;
		case 19: *offset = sizeof(collect_struct->pe_option_header.SizeOfImage); return collect_struct->pe_option_header.SizeOfImage;
		case 20: *offset = sizeof(collect_struct->pe_option_header.SizeOfHeaders); return collect_struct->pe_option_header.SizeOfHeaders;
		case 21: *offset = sizeof(collect_struct->pe_option_header.Checksum); return collect_struct->pe_option_header.Checksum;
		case 22: *offset = sizeof(collect_struct->pe_option_header.Subsystem); return collect_struct->pe_option_header.Subsystem;
		case 23: *offset = sizeof(collect_struct->pe_option_header.DLLCharacteristics); return collect_struct->pe_option_header.DLLCharacteristics;
		case 24: *offset = sizeof(collect_struct->pe_option_header.SizeOfStackReserve); return collect_struct->pe_option_header.SizeOfStackReserve;
		case 25: *offset = sizeof(collect_struct->pe_option_header.SizeOfStackCommit); return collect_struct->pe_option_header.SizeOfStackCommit;
		case 26: *offset = sizeof(collect_struct->pe_option_header.SizeOfHeapReserve); return collect_struct->pe_option_header.SizeOfHeapReserve;
		case 27: *offset = sizeof(collect_struct->pe_option_header.SizeOfHeapCommit); return collect_struct->pe_option_header.SizeOfHeapCommit;
		case 28: *offset = sizeof(collect_struct->pe_option_header.LoaderFlags); return collect_struct->pe_option_header.LoaderFlags;
		case 29: *offset = sizeof(collect_struct->pe_option_header.NumberOfRvaAndSizes); return collect_struct->pe_option_header.NumberOfRvaAndSizes;
		case 30: *offset = sizeof(collect_struct->pe_option_header.signature); return collect_struct->pe_option_header.signature;
	}
}

int dos_header_getval(Collect_Struct* collect_struct, int n, int* offset)
{
	switch(n)
	{
		case 1: *offset = sizeof(collect_struct->dos_header.lastsize);  return collect_struct->dos_header.lastsize;
		case 2: *offset = sizeof(collect_struct->dos_header.nblocks);  return collect_struct->dos_header.nblocks;
		case 3: *offset = sizeof(collect_struct->dos_header.nreloc);  return collect_struct->dos_header.nreloc;
		case 4: *offset = sizeof(collect_struct->dos_header.hdrsize);  return collect_struct->dos_header.hdrsize;
		case 5: *offset = sizeof(collect_struct->dos_header.minalloc);  return collect_struct->dos_header.minalloc;
		case 6: *offset = sizeof(collect_struct->dos_header.maxalloc);  return collect_struct->dos_header.maxalloc;
		case 7: *offset = sizeof(collect_struct->dos_header.ss);  return collect_struct->dos_header.ss;
		case 8: *offset = sizeof(collect_struct->dos_header.sp);  return collect_struct->dos_header.sp;
		case 9: *offset = sizeof(collect_struct->dos_header.checksum);  return collect_struct->dos_header.checksum;
		case 10: *offset = sizeof(collect_struct->dos_header.ip);  return collect_struct->dos_header.ip;
		case 11: *offset = sizeof(collect_struct->dos_header.cs);  return collect_struct->dos_header.cs;
		case 12: *offset = sizeof(collect_struct->dos_header.relocpos); return collect_struct->dos_header.relocpos;
		case 13: *offset = sizeof(collect_struct->dos_header.noverlay); return collect_struct->dos_header.noverlay;
		case 14: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved1[0];
		case 15: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved1[1];
		case 16: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved1[2];
		case 17: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved1[3];
		case 18: *offset = sizeof(collect_struct->dos_header.oem_id); return collect_struct->dos_header.oem_id;
		case 19: *offset = sizeof(collect_struct->dos_header.oem_info); return collect_struct->dos_header.oem_info;
		case 20: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved2[0];
		case 21: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved2[1];
		case 22: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved2[2];
		case 23: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved2[3];
		case 24: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved2[4];
		case 25: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved2[5];
		case 26: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved2[6];
		case 27: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved2[7];
		case 28: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved2[8];
		case 29: *offset = sizeof(collect_struct->dos_header.reserved1[0]); return collect_struct->dos_header.reserved2[9];
		case 30: *offset = sizeof(collect_struct->dos_header.e_lfanew); return collect_struct->dos_header.e_lfanew;
	}
	return -1;
}
