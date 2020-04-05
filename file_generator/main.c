#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <memory.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "global_define.h"

int	generate_file(char*		pszFilePath)
{
	FILE*				fp;
	unsigned char		uszBuffer[1048576];
	TEST_FILE_HEADER	stHeader;

	fp = fopen(pszFilePath,"wb");
	if( NULL == fp )
	{
		return -1;
	}

	memset(uszBuffer, 0xFF, 1048576);
	
	stHeader.szMagicWord[0]		= 'T';
	stHeader.szMagicWord[1]		= 'E';
	stHeader.szMagicWord[2]		= 'S';
	stHeader.szMagicWord[3]		= 'T';
	stHeader.uiContentLength	= 500;
	stHeader.uiContentOffset	= 100;
	stHeader.uiFileds			= 1;

	memcpy(uszBuffer, &stHeader, sizeof(TEST_FILE_HEADER));

	fwrite(uszBuffer, 1, 1048576, fp);
	fseek(fp, 0, SEEK_SET);
	fwrite(&stHeader, 1, sizeof(TEST_FILE_HEADER),fp);


	fclose(fp);

	return 0;
}

int main(int argc, char* argv[])
{
	FILE*			fp;
	int				iResult;

	if( argc < 2 ){
		exit(0);
	}

	printf("%s\n",argv[1]);

	generate_file(argv[1]);

	exit(0);
}