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

unsigned int		g_iFileLength;
unsigned char*		g_puszBuffer;
unsigned int		g_uiContentOffset;
unsigned int		g_uiContentLength;


int PrintUsage()
{
	printf("test_program [input_file_path]\n");
	return 0;
}

int CheckID(unsigned char* puszFileBuffer)
{
	if( ( *puszFileBuffer == 'T' ) &&
		( *( puszFileBuffer + 1 ) == 'E' ) &&
		( *( puszFileBuffer + 2 ) == 'S' ) &&
		( *( puszFileBuffer + 3 ) == 'T' ) )
	{	
		printf("File Header ID error\n");
		return 0;
	}else{
		printf("File Header ID ok\n");
		return -1;
	}
}

int	CheckHeadBlock(unsigned char* puszFileBuffer)
{
	P_TEST_FILE_HEADER pstFileHeaderBlock;
	pstFileHeaderBlock = (P_TEST_FILE_HEADER)puszFileBuffer;

	if( pstFileHeaderBlock->uiFileds < 1 )
	{
		return -1;
	}
	else{
		g_uiContentOffset = pstFileHeaderBlock->uiContentOffset;
		g_uiContentLength = pstFileHeaderBlock->uiContentLength;
		printf("have %d fields\n", pstFileHeaderBlock->uiFileds);
		return 0;
	}

	return -1;
}



int	ParseContent(unsigned char* puszFileBuffer)
{
	unsigned char*	puszCurrentChar;
	unsigned char*	puszBuffer;
	int				i = 0;

	printf("[ParseContent] size:%d\n",g_uiContentLength);

	puszBuffer = malloc(g_uiContentLength);
	puszCurrentChar = puszFileBuffer + g_uiContentOffset;

	fprintf(stderr,"current puszFileBuffer:%lx\n", (uint64_t)puszFileBuffer);
	fprintf(stderr,"current puszCurrentChar:%lx\n",(uint64_t)puszCurrentChar);
	fprintf(stderr,"current char:%x\n",*puszCurrentChar);

	while( *puszCurrentChar != 0 )
	{
		*puszBuffer = *puszCurrentChar;
		puszCurrentChar++;
		puszBuffer++;
		i++;
		//printf("current char:%x",*puszCurrentChar);
	}
	printf("parse length:%d\n",i);

	return -1;
}


unsigned long GetFileSize(const char *path)
{
	unsigned long filesize = -1;	
	struct stat statbuff;
	if(stat(path, &statbuff) < 0){
		return filesize;
	}else{
		filesize = statbuff.st_size;
	}
	return filesize;
}


int main(int argc, char* argv[])
{
	FILE*			fp;
	int				iResult;

	g_iFileLength		= 0;
	g_puszBuffer		= NULL;
	g_uiContentOffset	= 0;
	g_uiContentLength	= 0;
	
	if( argc < 2 ){
		PrintUsage();
		exit(0);
	}

	printf("%s\n",argv[1]);

	//generate_file(argv[1]);

	g_iFileLength = GetFileSize(argv[1]);
	printf("file length:%d\n",g_iFileLength);

	fp = fopen(argv[1],"rb");
	if(NULL == fp){
		printf("open file error\n");
		exit(-1);
	}

	g_puszBuffer = calloc(g_iFileLength,1);

	if( NULL == g_puszBuffer )
	{
		printf("malloc memory error\n");
		goto error_exit;
	}

	iResult = fread(g_puszBuffer,1048576,1,fp);
	if( 0 > iResult )
	{
		printf("read file error\n");
	}

	iResult = CheckID(g_puszBuffer);
	if( -1 == iResult )
	{
		goto error_exit;
	}

	iResult = CheckHeadBlock(g_puszBuffer);
	if( -1 == iResult )
	{
		goto error_exit;
	}

	iResult = ParseContent(g_puszBuffer);

	free(g_puszBuffer);
	fclose(fp);

error_exit:
	printf("program exit\n");
	exit(0);
}