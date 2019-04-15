/*
argu1(name) :  file name   
argu2(file_vol) : file size pointer

return : return heap address mapped file contents

description : read file, mapping heap area, and input file size
*/
unsigned char* file_to_heap(const char* name, int* file_vol)
{
	FILE* file_pointer = NULL;
	unsigned char* file_heap_address = NULL; 

	file_pointer = fopen(name, "r");
	if(file_pointer == NULL)
	{
		error_msg_print("FILE OPEN ERROR!", errno); 
	}	
	fseek(file_pointer, 0, SEEK_END);  //get file total size 
	*file_vol = ftell(file_pointer); 


	fseek(file_pointer, 0, SEEK_SET);  //get back file position

	file_heap_address = (unsigned char*)malloc(sizeof(unsigned char)*(*file_vol)); 
	//allocate heap and copy file contets.
   	if(file_heap_address == NULL)
	{
		error_msg_print("malloc() function error!", errno);
	}	
	fread(file_heap_address, sizeof(unsigned char), *file_vol, file_pointer); 	
	fclose(file_pointer); 
	return file_heap_address; 	
}

