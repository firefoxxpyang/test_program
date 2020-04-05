#ifndef _GLOBAL_DEFINE_H_
#define _GLOBAL_DEFINE_H_

#define _TRACER_DEBUG_

#ifdef _TRACER_DEBUG_
#define     KdPrint(...) printf(__VA_ARGS__)
#define     KdFPrint(x, ...) fprintf(x, __VA_ARGS__)
#define     KdFFlush(x) fflush(x)
#else
#define     KdPrint(...)
#define     KdFPrint(x, ...)
#define     KdFFlush(x)
#endif

#define     INPUT_PARAMETER
#define     OUTPUT_PARAMETER


typedef struct _TEST_FILE_HEADER_{
    char            szMagicWord[4];
    unsigned int    uiContentLength;
    unsigned int    uiFileds;
    unsigned int    uiContentOffset;
}TEST_FILE_HEADER,*P_TEST_FILE_HEADER,**PP_TEST_FILE_HEADER;


#endif