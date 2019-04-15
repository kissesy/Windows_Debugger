#include "parser.h"
#include "dumpcode.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


/*
argu1 : file pointer 
argu2 : DOS_Header
argu3 : COFF_Header
argu4 : PE_OptHeader
argu5 : IMAGE_SECTION_HEADER
argu6 : section table number 

return : void 

description: print pe imformation
*/

void print_pe_format_imformation(FILE* file_pointer, DOS_Header* dos_header, COFF_Header* coff_header, PE_OptHeader* pe_option_header, IMAGE_SECTION_HEADER* image_section_header, int section_number)
{
	char* description_dos_header[31] = {"Signature", "Bytes on Last Page of File", "Pages in File", "Relocations", "Size of Header in Paragraphs", "Minimum Extra Paragraphs", "Maximum Extra Paragraphs", "Initial SS", "Initial SP", "Checksum", "Initial IP", "Initial CS", "Offset to Relocation Table", "Overlay Number", "Rserved", "Reserved", "Reserved", "Reserved", "OEM Identifier", "OEM Information", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Offset to New EXE Header"};

	char* description_coff_header[8] = {"Signature", "Machine", "Number of Sections", "Time Data Stamp", "Pointer to Symbol Table", "Number of Symbols", "Size of Optional Header", "Characteristics"};

	char* description_pe_optional_header[31] = {"Magic", "Major Linker Version", "Minor Linker Version", "Size of Code", "Size of Inialtalized Data", "Size of Uninitalized Data", "Address of Entry Point", "Base of Code", "Base of Data", "Image Base", "Section Alignment", "File Alignment", "Major O/S Version", "Minor O/S Version", "Major Image Version", "Minor Image Version", "Major Subsystem Version", "Minor Subsystem Version", "Win32 Version Value", "Size of Image", "Size of Headers", "Checksum", "Subsystemem", "DLL Characteristics", "Size of Stack Reverse", "Size of Stack Commit", "SIze of Heap Reverse", "Size of Heap Commit", "Loader Flags", "Number of Data Directories"};
	//+ data_directory 추가 해야함. 

	Print_DOS_Header print_dos_header; 
	memcpy(&print_dos_header, dos_header, sizeof(DOS_Header)); 
	char command[256] = {0, }; 
	int file_offset = 0; //+ sizeof(data type); 	DOS_HEADER
	printf("\n=====================PE_VIWER=================\n");

	printf("====================DOS_HEADER================\n"); 
	printf("pFile \t\t     Data \t    Description\n");

	for(int i=0;i<30;i++)
	{
		printf("%08x \t %04X \t\t %s\n",file_offset,print_dos_header.imformation[i] ,description_dos_header[i]);
		file_offset+=sizeof(print_dos_header.imformation[i]); 
	}
	printf("%08x \t %08X\t%s\n",file_offset,dos_header->e_lfanew ,description_dos_header[30]);
	file_offset+=sizeof(dos_header->e_lfanew); 

	//1바이트 단위로 정렬했으니 offset으로 접근이 가능할 듯 중간 중간 short와 long이 교차하네 
	file_offset = dos_header->e_lfanew; 
	printf("\n=================COFF_HEADER===============\n");
	printf("pFile \t\t     Data \t    Description\n");
	printf("%08X \t %08X\t%s\n", file_offset, coff_header->signature, description_coff_header[0]);
	file_offset+=sizeof(coff_header->signature); 

	printf("%08X \t %08X\t%s\n", file_offset, coff_header->Machine, description_coff_header[1]);
	file_offset+=sizeof(coff_header->Machine); 

	printf("%08X \t %08X\t%s\n", file_offset, coff_header->NumberOfSections, description_coff_header[2]);
	file_offset+=sizeof(coff_header->NumberOfSections); 

	printf("%08X \t %08X\t%s\n", file_offset, coff_header->TimeDateStamp, description_coff_header[3]);
	file_offset+=sizeof(coff_header->TimeDateStamp); 

	printf("%08X \t %08X\t%s\n", file_offset, coff_header->PointerToSymbolTable, description_coff_header[4]);
	file_offset+=sizeof(coff_header->PointerToSymbolTable); 

	printf("%08x \t %08x\t%s\n", file_offset, coff_header->NumberOfSymbols, description_coff_header[5]);
	file_offset+=sizeof(coff_header->NumberOfSymbols); 


	printf("%08X \t %08X\t%s\n", file_offset, coff_header->SizeOfOptionalHeader, description_coff_header[6]);
	file_offset+=sizeof(coff_header->SizeOfOptionalHeader); 

	printf("%08X \t %08X\t%s\n", file_offset, coff_header->Characteristics, description_coff_header[7]);
	file_offset+=sizeof(coff_header->Characteristics); 


	printf("\n=================OPTIONAL_HEADER===============\n");
	printf("pFile \t\t     Data \t    Description\n");
	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->signature, description_pe_optional_header[0]);
	file_offset+=sizeof(pe_option_header->signature); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->MajorLinkerVersion,description_pe_optional_header[1] );
	file_offset+=sizeof(pe_option_header->MajorLinkerVersion); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->MinorLinkerVersion, description_pe_optional_header[2]);
	file_offset+=sizeof(pe_option_header->MinorLinkerVersion); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->SizeOfCode,description_pe_optional_header[3]);
	file_offset+=sizeof(pe_option_header->SizeOfCode); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->SizeOfInitializedData, description_pe_optional_header[4]);
	file_offset+=sizeof(pe_option_header->SizeOfInitializedData); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->SizeOfUninitializedData, description_pe_optional_header[5]);
	file_offset+=sizeof(pe_option_header->SizeOfUninitializedData); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->AddressOfEntryPoint, description_pe_optional_header[6]);
	file_offset+=sizeof(pe_option_header->AddressOfEntryPoint); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->BaseOfCode, description_pe_optional_header[7]);
	file_offset+=sizeof(pe_option_header->BaseOfCode); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->BaseOfData, description_pe_optional_header[8]);
	file_offset+=sizeof(pe_option_header->BaseOfData); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->ImageBase, description_pe_optional_header[9]);
	file_offset+=sizeof(pe_option_header->ImageBase); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->SectionAlignment,description_pe_optional_header[10]);
	file_offset+=sizeof(pe_option_header->SectionAlignment); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->FileAlignment, description_pe_optional_header[11]);
	file_offset+=sizeof(pe_option_header->FileAlignment); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->MajorOSVersion, description_pe_optional_header[12]);
	file_offset+=sizeof(pe_option_header->MajorOSVersion); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->MinorOSVersion, description_pe_optional_header[13]);
	file_offset+=sizeof(pe_option_header->MinorOSVersion); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->MajorImageVersion, description_pe_optional_header[14]);
	file_offset+=sizeof(pe_option_header->MajorImageVersion); 
	
	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->MinorImageVersion, description_pe_optional_header[15]);
	file_offset+=sizeof(pe_option_header->MinorImageVersion); 


	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->MajorSubsystemVersion, description_pe_optional_header[16]);
	file_offset+=sizeof(pe_option_header->MajorSubsystemVersion); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->MinorSubsystemVersion, description_pe_optional_header[17]);
	file_offset+=sizeof(pe_option_header->MinorSubsystemVersion); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->Win32VersionValue, description_pe_optional_header[18]);
	file_offset+=sizeof(pe_option_header->Win32VersionValue); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->SizeOfImage, description_pe_optional_header[19]);
	file_offset+=sizeof(pe_option_header->SizeOfImage); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->SizeOfHeaders, description_pe_optional_header[20]);
	file_offset+=sizeof(pe_option_header->SizeOfHeaders); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->Checksum, description_pe_optional_header[21]);
	file_offset+=sizeof(pe_option_header->Checksum); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->Subsystem, description_pe_optional_header[22]);
	file_offset+=sizeof(pe_option_header->Subsystem); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->DLLCharacteristics, description_pe_optional_header[23]);
	file_offset+=sizeof(pe_option_header->DLLCharacteristics); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->SizeOfStackReserve, description_pe_optional_header[24]);
	file_offset+=sizeof(pe_option_header->SizeOfStackReserve); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->SizeOfStackCommit, description_pe_optional_header[25]);
	file_offset+=sizeof(pe_option_header->SizeOfStackCommit); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->SizeOfHeapReserve, description_pe_optional_header[26]);
	file_offset+=sizeof(pe_option_header->SizeOfHeapReserve); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->SizeOfHeapCommit, description_pe_optional_header[27]);
	file_offset+=sizeof(pe_option_header->SizeOfHeapCommit); 

	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->LoaderFlags, description_pe_optional_header[28]);
	file_offset+=sizeof(pe_option_header->LoaderFlags); 


	printf("%08X \t %08X\t%s\n", file_offset, pe_option_header->NumberOfRvaAndSizes, description_pe_optional_header[29]);
	file_offset+=sizeof(pe_option_header->NumberOfRvaAndSizes); 

}
























