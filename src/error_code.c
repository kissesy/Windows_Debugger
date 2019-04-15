#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
/*
argu1(msg) : print msg error_msg
argu2(error_code) : save erorro number, if error_code is 0, ignore!

return : void 

description : print error msg 
*/

void error_msg_print(const char* msg, int error_code)
{
	if(error_code)
	{
		printf("error msg : %s and number : %d, the meaning is : %s\n", msg, error_code, strerror(error_code));
	}
	else
	{
		printf("error msg : %s", msg);
	}
	exit(1);
}

