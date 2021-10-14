#ifndef CRT_HEADER_H
#define CRT_HEADER_H

#define CRTDBG_ON 1
#define MYDEBUG_ON 0

#if CRTDBG_ON
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW
#endif	// _DEBUG
#endif	// CRTDBG_ON


#if MYDEBUG_ON
#ifndef _DEBUG
#define _DELETECOUNT
static int newCount = 0;

void* operator new(size_t size)
{
	newCount++;
	return malloc(size);
}

void operator delete(void* memory)
{
	newCount--;
	free(memory);
}
#endif
#endif

#endif	// CRT_HEADER_H
