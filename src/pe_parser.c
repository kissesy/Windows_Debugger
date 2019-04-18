/*
   PE parser : tuuna
   read to file, mapping Heap! 
   */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
//#include <windows.h>
//#include <winnt.h>

#include "parser.h"


/*
int main(int argc, char** argv)
{
	if(argc !=2)
	{
		error_msg_print("Please use argv[1]", 0); 
	}
	control_function(argv[1]); 
	return 0;
}
*/

/*
   this function is controll function like little main!
   */

//winproc함수에서 인자로 넘길 예정 
//구조체도 인자로 넘겨야 할듯 
//DOS_Header* dos_header, COFF_Header* coff_header, PE_OptHeader64* pe_option_header, PE_OptHeader pe_option_header; 
int control_function(const char* file_name)
{
	int binary_bit_number = 0; // 32bit is 32, 64bit is 64
	DOS_Header dos_header; 
	COFF_Header coff_header; 
	PE_OptHeader pe_option_header; //32bit optional header 
	PE_OptHeader64 pe_option_header64; //64bit optional header 	

	int file_vol=0; 
	FILE* file_pointer = fopen(file_name, "rb"); 
	if(file_pointer == NULL)
	{
		error_msg_print("fopen() is error", errno); 
	}
	pe_header_parser(file_pointer, &dos_header, &coff_header, &pe_option_header, &pe_option_header64, &binary_bit_number); 

	IMAGE_SECTION_HEADER image_section_header[coff_header.NumberOfSections]; //section table 

	pe_section_parser(file_pointer, image_section_header, coff_header.NumberOfSections);

	//print_pe_format_imformation(file_pointer, &dos_header, &coff_header, &pe_option_header, &pe_option_header64, image_section_header, coff_header.NumberOfSections);

	free(pe_option_header.DataDirectory); //memory free
	return 0;
}

/*
   argu1(file_pointer) : file_pointer to analysis file 
   argu2(*dos_header) : input dos_header contents
   argu3(*coff_header) :  input coff_header contents
   argu4(*pe_option_header) : input pe_option_header contents

return : void 
description : parsing pe format for DOS_Header and COFF_Header and PE_OptHeader 
*/

void pe_header_parser(FILE* file_pointer, DOS_Header* dos_header, COFF_Header* coff_header, PE_OptHeader* pe_option_header, PE_OptHeader64* pe_option_header64, int* binary_bit_number)
{
	short bit_signature=0; //check bit number 
	int backup_filepointer_position=0;
	fseek(file_pointer, 0, SEEK_SET); 
	fread(dos_header, sizeof(DOS_Header), 1, file_pointer); //read dos header 


	fseek(file_pointer, dos_header->e_lfanew, SEEK_SET);
	fread(coff_header, sizeof(COFF_Header), 1, file_pointer); //read file header 

	fseek(file_pointer, 0, SEEK_CUR);
	backup_filepointer_position = ftell(file_pointer); 
	fread(&bit_signature, sizeof(bit_signature), 1, file_pointer); 
	
	if(bit_signature = 267){
		*binary_bit_number = 32;
	}
	else if(bit_signature == 523){
		*binary_bit_number = 64;
	}
	else {
		error_msg_print("this is not 32bit binary and 64bit binary!", 0);
	}
	
	fseek(file_pointer, backup_filepointer_position, SEEK_SET); //restore file pointer position
	if(*binary_bit_number == 32)
	{
		fread(pe_option_header, sizeof(PE_OptHeader)-sizeof(struct _data_directory*), 1, file_pointer); 
		//read optional header 
		//why sub size struct _data_directory* => 이는 numberofrvaandsize 멤버를 통해 결정되는 사이즈이기 때문에 이것만 빼고 미리 읽어야함. 
		pe_option_header->DataDirectory = (struct _data_directory*)malloc(sizeof(struct _data_directory) * pe_option_header->NumberOfRvaAndSizes); 

		//read DataDirectory until NumberOfRvaAndSizes!
		for(int i=0;i<pe_option_header->NumberOfRvaAndSizes;i++)
		{
			fread(&(pe_option_header->DataDirectory[i]), sizeof(struct _data_directory), 1, file_pointer);
		}
	}
	else if(*binary_bit_number == 64)
	{
		fread(pe_option_header64, sizeof(PE_OptHeader64)-sizeof(struct _data_directory*), 1, file_pointer); 
		pe_option_header64->DataDirectory = (struct _data_directory*)malloc(sizeof(struct _data_directory) * pe_option_header64->NumberOfRvaAndSizes); 

		for(int i=0;i<pe_option_header64->NumberOfRvaAndSizes;i++)
		{
			fread(&(pe_option_header64->DataDirectory[i]), sizeof(struct _data_directory), 1, file_pointer);
		}
	}
}

/*
   argu1(file_pointer) : file pointer 
   argu2(image_section_header) : struct section table 
   argu3(section number) : section table number!

description : fill the image_section_header struct each section 

return : void
*/
void pe_section_parser(FILE* file_pointer, IMAGE_SECTION_HEADER* image_section_header, int section_number)
{
	for(int i=0;i<section_number;i++)
	{
		fread(&image_section_header[i],sizeof(IMAGE_SECTION_HEADER), 1,file_pointer); 
	}	
}





















