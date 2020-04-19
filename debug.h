#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H
#include <stdlib.h>
#include <stdio.h>

//#define CHE_MEMLOG //memory allocations
#define CHE_ASSERT
//#define CHE_SCOPEMEMLOG //memory allocations that get freed at the end of its scope

#ifdef CHE_MEMLOG
	#define che_malloc(size)\
	({\
		void* retVal = malloc((size));\
		printf("\033[0;34m");\
		printf("Che-Allocation:     bytes: %.4u | line: %.3i | file: %s\n", (uint16_t)size, __LINE__, __FILE__ );\
		printf("\033[0m");\
		retVal;\
	})


	#define che_calloc(num, size)\
	({\
		void* retVal = calloc((num), (size));\
		printf("\033[0;34m");\
		printf("Che-Allocation:     bytes: %.4u | line: %.3i | file: %s\n", (uint16_t)size, __LINE__, __FILE__ );\
		printf("\033[0m");\
		retVal;\
	})

	//changes font color to green, print message, reset font color
	#define che_free(p)\
	({\
		free((p));\
		printf("\033[0;32m");\
		printf("Che-Deallocation:                 line: %.3i | file: %s\n", __LINE__, __FILE__);\
		printf("\033[0m");\
	})
#else
	#define che_free(p) free(p)
	#define che_malloc(size) malloc(size)
	#define che_calloc(num, size) calloc(num, size)
#endif

#ifdef CHE_SCOPEMEMLOG
	#define che_scope_malloc(size)\
	({\
		void* retVal = malloc((size));\
		printf("\033[0;34m");\
		printf("Che-Scope-Allocation:bytes: %.4u | line: %.3i | file: %s\n", (uint16_t)size, __LINE__, __FILE__ );\
		printf("\033[0m");\
		retVal;\
	})


	#define che_scope_calloc(num, size)\
	({\
		void* retVal = calloc((num), (size));\
		printf("\033[0;34m");\
		printf("Che-Scope-Allocation:bytes: %.4u | line: %.3i | file: %s\n", (uint16_t)size, __LINE__, __FILE__ );\
		printf("\033[0m");\
		retVal;\
	})

	//changes font color to green, print message, reset font color
	#define che_scope_free(p)\
	({\
		free((p));\
		printf("\033[0;32m");\
		printf("Che-Scope-Deallocation:          line: %.3i | file: %s\n", __LINE__, __FILE__);\
		printf("\033[0m");\
	})
#else
	#define che_scope_free(p) free(p)
	#define che_scope_malloc(size) malloc(size)
	#define che_scope_calloc(num, size) calloc(num, size)
#endif

#ifdef CHE_ASSERT
	#define che_assert(expr)\
		if (!(expr))\
		{\
			printf("Che-Assertion:   %s failed. Line: %u, File: %s\n", #expr, __LINE__, __FILE__);\
			exit(-1);\
		}
#else
	#define che_assert(expr) (void)0
#endif


#endif