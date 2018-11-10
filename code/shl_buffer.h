//
// Structure
// Capacity (int)
// Length (int)
// 
// buffer area
//

#ifndef SHL_BUFFER_H
	
	#define SHL_BUFFER_H

	#ifndef shl_buffer_realloc
		#include <malloc.h>
		#define shl_buffer_realloc realloc
	#endif



	#define shl__buff_cap(buff) (*((int*)(buff) - 2))
	#define shl__buff_len(buff) (*((int*)(buff) - 1))
	#define shl__buff_head(buff) (void*) ((int*)(buff) - 2)
	
	
	static void* shl__grow(void* buff, int element_size)
	{
		int new_cap = buff ? shl__buff_cap(buff) * 2 + 1 : 5;

		int* new_loc = (int*) realloc(buff ? shl__buff_head(buff) : 0, element_size * new_cap + 2 * sizeof(int));

		*new_loc = new_cap;

		if (!buff) *(new_loc + 1) = 0;

		return (void*) (new_loc + 2);


	}

	static void* shl__reserve(void* buff, int new_cap, int element_size)
	{
		int* new_loc = (int*) realloc(buff ? shl__buff_head(buff) : 0, new_cap * element_size + 2 * sizeof(int));

		*new_loc = new_cap;

		if (!buff) *(new_loc + 1) = 0;

		return (void*) (new_loc + 2);

	}

	#define shl__should_grow(buff) ( ( (buff) == 0 || shl__buff_len(buff) == shl__buff_cap(buff) ) ? shl__grow_m(buff) : 0 )
	#define shl__grow_m(buff) *((void**) &(buff)) = shl__grow(buff, sizeof(*buff))
	#define shl__reverse_m(buff, cap) *((void**) &(buff)) = shl__reserve(buff, cap, sizeof(*buff))

	#define shl_buffer(type) type##*

	#define shl_buffer_push(buff, val) (shl__should_grow(buff), (buff)[shl__buff_len(buff)++] = (val))
	#define shl_buffer_length(buff) ((buff) ? shl__buff_len(buff) : 0)
	#define shl_buffer_capacity(buff) ((buff) ? shl__buff_cap(buff) : 0)
	#define shl_buffer_free(buff) free(shl__buff_head(buff))
	#define shl_buffer_reserve(buff, cap) ((buff) == 0 || (shl__buff_cap(buff) < (cap)) ?  shl__reverse_m(buff, cap) : 0) 


#endif