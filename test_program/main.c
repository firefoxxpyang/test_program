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
	char* puszCurrentChar;
	bool bFirstCharCheck;
	bool bSecondCharCheck;
	bool bThirdCharCheck;
	bool bFourthCharCheck;

	puszCurrentChar = puszFileBuffer;
	if( 'T' == *puszCurrentChar ){
		KdPrint("first char ok\n");
		bFirstCharCheck = true;
	}
	else{
		KdPrint("first char error\n");
		return -1;
	}

	puszCurrentChar++;
	if( 'E' == *puszCurrentChar ){
		bSecondCharCheck = true;
		KdPrint("second char ok\n");
	}
	else{
		KdPrint("second char error\n");
		return -1;
	}

	puszCurrentChar++;
	if( 'S' == *puszCurrentChar ){
		bThirdCharCheck = true;
		KdPrint("third char ok\n");
	}
	else{
		KdPrint("third char error\n");
		return -1;
	}

	puszCurrentChar++;
	if( 'T' == *puszCurrentChar ){
		bFourthCharCheck = true;
		KdPrint("fourth char ok\n");
	}
	else{
		KdPrint("fourth char error\n");
		return -1;
	}

	puszCurrentChar = puszFileBuffer + 15;

	if(*puszCurrentChar < 15){
		return -1;
	}

	if(*puszCurrentChar > 17){
		return -1;
	}

	return 0;
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
		KdPrint("have %d fields\n", pstFileHeaderBlock->uiFileds);
		return 0;
	}

	return -1;
}



int	ParseContent(unsigned char* puszFileBuffer)
{
	unsigned char*	puszCurrentChar;
	unsigned char*	puszBuffer;
	int				i = 0;

	KdPrint("[ParseContent] size:%d\n",g_uiContentLength);

	puszBuffer = malloc(g_uiContentLength);
	puszCurrentChar = puszFileBuffer + g_uiContentOffset;

	KdFPrint(stderr,"current puszFileBuffer:%lx\n", (uint64_t)puszFileBuffer);
	KdFPrint(stderr,"current puszCurrentChar:%lx\n",(uint64_t)puszCurrentChar);
	KdFPrint(stderr,"current char:%x\n",*puszCurrentChar);

	while( *puszCurrentChar != 0 )
	{
		*puszBuffer = *puszCurrentChar;
		puszCurrentChar++;
		puszBuffer++;
		i++;
		//KdPrint("current char:%x",*puszCurrentChar);
	}
	KdPrint("parse length:%d\n",i);

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

	KdPrint("%s\n",argv[1]);

	//generate_file(argv[1]);

	g_iFileLength = GetFileSize(argv[1]);
	KdPrint("file length:%d\n",g_iFileLength);

	fp = fopen(argv[1],"rb");
	if(NULL == fp){
		KdPrint("open file error\n");
		exit(-1);
	}

	g_puszBuffer = calloc(g_iFileLength,1);

	if( NULL == g_puszBuffer )
	{
		KdPrint("malloc memory error\n");
		goto error_exit;
	}

	iResult = fread(g_puszBuffer, g_iFileLength, 1, fp);
	if( 0 > iResult )
	{
		KdPrint("read file error\n");
	}else{
		KdPrint("%s\n",g_puszBuffer);
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
	KdPrint("program exit\n");
	exit(0);
}