#include "parser.h"

void parser_to_string_dos_header(char(*print_string)[100], Collect_Struct* collect_struct,HWND hWnd)
{
	//MessageBox(hWnd, "5", "1", MB_OK);
	int getval = 0;
	int offset = 0;
	char* description_dos_header[31] = { "Signature", "Bytes on Last Page of File", "Pages in File", "Relocations",
		"Size of Header in Paragraphs", "Minimum Extra Paragraphs", "Maximum Extra Paragraphs", "Initial SS",
		"Initial SP", "Checksum", "Initial IP", "Initial CS", "Offset to Relocation Table", "Overlay Number",
		"Rserved", "Reserved", "Reserved", "Reserved", "OEM Identifier", "OEM Information", "Reserved", "Reserved",
		"Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Offset to New EXE Header" };
	int file_offset = 0;
	//MessageBox(hWnd, "4", "1", MB_OK);
	for (int i = 0; i < 100; i++)
	{
		memset(print_string[i], 0, 100);
	}
	//MessageBox(hWnd, "1", "1", MB_OK);
		sprintf(print_string[0], "%X        %X%X        %s", file_offset, collect_struct->dos_header.signature[0],collect_struct->dos_header.signature[1], description_dos_header[0]);
	file_offset += sizeof(collect_struct->dos_header.signature); 
	//MessageBox(hWnd, "2", "1", MB_OK);
	for (int i = 1; i < 31; i++)
	{
		getval = dos_header_getval(collect_struct, i, &offset); 
		sprintf(print_string[i], "%X        %X        %s", file_offset, getval, description_dos_header[i]);
		file_offset += offset; 
	}
	//MessageBox(hWnd, "3", "1", MB_OK);
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
