#ifndef __P2DEFS_H__
#define __P2DEFS_H__

#include <stdio.h>
#include <string>
#include <assert.h>

//  NULL just in case ----------------------

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != NULL )        \
	    {                      \
      delete x;            \
	  x = NULL;              \
	    }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
    {                              \
    if( x != NULL )              \
	    {                            \
      delete[] x;                \
	  x = NULL;                    \
	    }                            \
                              \
    }

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

// Standard string size
#define SHORT_STR	32
#define MID_STR		255
#define HUGE_STR	8192

// Joins a path and file
inline const char* const PATH(const char* folder, const char* file)
{
	static char path[MID_STR];
	sprintf_s(path, MID_STR, "%s/%s", folder, file);
	return path;
}

// Performance macros
#define PERF_START(timer) timer.Start()
#define PERF_PEEK(timer) LOG("%s took %f ms", __FUNCTION__, timer.ReadMs())


inline void cutString2(std::string& string, uint start, uint end)
{
	start = MIN(start, string.size());
	end = (end == 0) ? string.size() : MIN(end, string.size());
	assert(start <= end);
	int s = end - start;

	if (start > 0)
	{
		memmove((char*)string.data(), &string[start], s);
	}

	string[s] = '\0';
}

inline void cutString(std::string& string, uint pos)
{
	if (pos < string.size())
	{
		memmove(&string[pos], &string[pos + 1], string.size() - pos);
	}
}

// Returns true if a point is inside a rectangle
template <class VALUE>
bool isInsideRectangle(VALUE x, VALUE y, VALUE recX, VALUE recY, VALUE recW, VALUE recH)
{
	return((x >= recX && x <= (recX + recW)) && (y >= recY && (y <= (recY + recH))));
}

#endif