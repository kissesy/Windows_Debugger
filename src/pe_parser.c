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


int main(int argc, char** argv)
{
	if(argc !=2)
	{
		error_msg_print("Please use argv[1]", 0); 
	}
	control_function(argv[1]); 
	return 0;
}


/*i
 this function is controll function like little main!
*/
int control_function(const char* file_name)
{
	DOS_Header dos_header; 
	COFF_Header coff_header; 
	PE_OptHeader pe_option_header; 
	
	int file_vol=0; 
	FILE* file_pointer = fopen(file_name, "rb"); 
	if(file_pointer == NULL)
	{
		error_msg_print("fopen() is error", errno); 
	}
	pe_header_parser(file_pointer, &dos_header, &coff_header, &pe_option_header); 

	IMAGE_SECTION_HEADER image_section_header[coff_header.NumberOfSections]; //section table 

	pe_section_parser(file_pointer, image_section_header, coff_header.NumberOfSections);
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

void pe_header_parser(FILE* file_pointer, DOS_Header* dos_header, COFF_Header* coff_header, PE_OptHeader* pe_option_header)
{
	fseek(file_pointer, 0, SEEK_SET); 
	fread(dos_header, sizeof(DOS_Header), 1, file_pointer); //read dos header 


	fseek(file_pointer, dos_header->e_lfanew, SEEK_SET);
	fread(coff_header, sizeof(COFF_Header), 1, file_pointer); //read file header 
	
	fseek(file_pointer, 0, SEEK_CUR);
	fread(pe_option_header, sizeof(PE_OptHeader)-sizeof(struct _data_directory*), 1, file_pointer); 
	//read optional header 
	//why sub size struct _data_directory* => 이는 numberofrvaandsize 멤버를 통해 결정되는 사이즈이기 때문에 이것만 빼고 미리 읽어야함. 
	pe_option_header->DataDirectory = (struct _data_directory*)malloc(sizeof(struct _data_directory) * pe_option_header->NumberOfRvaAndSizes); 
	printf("test : %x\n", pe_option_header->NumberOfRvaAndSizes);
	
	//read DataDirectory until NumberOfRvaAndSizes!
	for(int i=0;i<pe_option_header->NumberOfRvaAndSizes;i++)
	{
		fread(&(pe_option_header->DataDirectory[i]), sizeof(struct _data_directory), 1, file_pointer);
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





















