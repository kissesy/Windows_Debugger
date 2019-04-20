/*
   PE parser : tuuna
   read to file, mapping Heap!
   */

#include "parser.h"

//초기화 작업 
int setting_parser(HWND hWnd, Collect_Struct* collect_struct)
{
	collect_struct->fin_parsing = 0;
	if (collect_struct->image_section_header != NULL) //만약 처음 오픈한 파일이 아니라면 그전에 할당했던 malloc을 free해준다. 
	{
		free(collect_struct->image_section_header);
		collect_struct->image_section_header = NULL;
	}
	if (collect_struct->pe_option_header.DataDirectory != NULL)
	{
		free(collect_struct->pe_option_header.DataDirectory);
		collect_struct->pe_option_header.DataDirectory = NULL;
	}
	else if (collect_struct->pe_option_header64.DataDirectory != NULL)
	{
		free(collect_struct->pe_option_header64.DataDirectory);
		collect_struct->pe_option_header64.DataDirectory = NULL;
	}
	//collect_struct.file_pointer = NULL;
	collect_struct->what_paint = -1;
	//memcpy(collect_struct.file_name, 0, 300); 
	collect_struct->file_offset = 0;
	collect_struct->binary_bit = 0;
	memset(&collect_struct->dos_header, 0, sizeof(collect_struct->dos_header));
	memset(&collect_struct->coff_header, 0, sizeof(collect_struct->coff_header));
	memset(&collect_struct->pe_option_header, 0, sizeof(collect_struct->pe_option_header));
	memset(&collect_struct->pe_option_header64, 0, sizeof(collect_struct->pe_option_header64));

	change_char(collect_struct);

	collect_struct->file_pointer = fopen(collect_struct->file_name, "rb");
	if (collect_struct->file_pointer == NULL)
	{
		char test_file[300] = "";
		sprintf(test_file, "%s", collect_struct->file_name);
		MessageBox(hWnd, test_file, "Error Message", MB_OK);
		return -1;
	}

	//header를 파싱한 다음 section을 파싱한다. 
	int check = PE_Header_Parser(collect_struct);
	if (check == -1)
	{
		MessageBox(hWnd, "Parsing Error!", "Error Message", MB_OK);
		return -1;
	}
	collect_struct->image_section_header = (IMAGE_Section_Header*)malloc(sizeof(IMAGE_Section_Header) * collect_struct->coff_header.NumberOfSections);
	
	
	check = PE_Section_Parser(collect_struct);
	////////////////////////////////////////////////////////////////////////////////////////
	if (check == -1)
	{
		MessageBox(hWnd, "Section Parsing Error!", "Error Message", MB_OK);
		return -1;
	}
	MessageBox(hWnd, "Parsing Success!", "Success Message", MB_OK);
	collect_struct->fin_parsing = 1;
	add_ComBoBox(hWnd, collect_struct);
	fclose(collect_struct->file_pointer);
	return 0;
}

int PE_Header_Parser(Collect_Struct* collect_struct)
{
	short bit_signature = 0;
	int backup_filepointer_position = 0;
	fseek(collect_struct->file_pointer, 0, SEEK_SET);
	fread(&(collect_struct->dos_header), sizeof(DOS_Header), 1, collect_struct->file_pointer);

	fseek(collect_struct->file_pointer, collect_struct->dos_header.e_lfanew, SEEK_SET);
	fread(&(collect_struct->coff_header), sizeof(COFF_Header), 1, collect_struct->file_pointer);

	backup_filepointer_position = ftell(collect_struct->file_pointer);
	fread(&bit_signature, sizeof(bit_signature), 1, collect_struct->file_pointer);

	if (bit_signature = 267) {
		collect_struct->binary_bit = 32;
	}
	else if (bit_signature == 523) {
		collect_struct->binary_bit = 64;
	}
	else {
		return -1;
	}
	fseek(collect_struct->file_pointer, backup_filepointer_position, SEEK_SET);
	if (collect_struct->binary_bit == 32)
	{
		fread(&(collect_struct->pe_option_header), sizeof(PE_OptHeader) - sizeof(struct _data_directory*), 1, collect_struct->file_pointer);
		//read optional header 
		//why sub size struct _data_directory* => 이는 numberofrvaandsize 멤버를 통해 결정되는 사이즈이기 때문에 이것만 빼고 미리 읽어야함. 
		collect_struct->pe_option_header.DataDirectory = (struct _data_directory*)malloc(sizeof(struct _data_directory) * collect_struct->pe_option_header.NumberOfRvaAndSizes);

		//read DataDirectory until NumberOfRvaAndSizes!
		for (int i = 0; i < collect_struct->pe_option_header.NumberOfRvaAndSizes; i++)
		{
			fread(&(collect_struct->pe_option_header.DataDirectory[i]), sizeof(struct _data_directory), 1, collect_struct->file_pointer);
		}
	}
	else if (collect_struct->binary_bit == 64)
	{
		fread(&(collect_struct->pe_option_header64), sizeof(PE_OptHeader64) - sizeof(struct _data_directory*), 1, collect_struct->file_pointer);
		collect_struct->pe_option_header64.DataDirectory = (struct _data_directory*)malloc(sizeof(struct _data_directory) * collect_struct->pe_option_header64.NumberOfRvaAndSizes);

		for (int i = 0; i < collect_struct->pe_option_header64.NumberOfRvaAndSizes; i++)
		{
			fread(&(collect_struct->pe_option_header64.DataDirectory[i]), sizeof(struct _data_directory), 1, collect_struct->file_pointer);
		}
	}
}

int PE_Section_Parser(Collect_Struct* collect_struct)
{
	for (int i = 0; i < collect_struct->coff_header.NumberOfSections; i++)
	{
		fread(&(collect_struct->image_section_header[i]), sizeof(IMAGE_Section_Header), 1, collect_struct->file_pointer);
	}
	 //섹션의 갯수만큼 콤보박스에 추가
}
