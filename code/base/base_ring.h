/* date = September 8th 2022 11:54 pm */

#ifndef BASE_RING_H
#define BASE_RING_H

////////////////////////////////
//~ rjf: Ring Buffer Functions

core_function U64 RingWrite(U8 *base, U64 size, U64 write_pos, String8 string);
core_function U64 RingRead(void *dst, U8 *base, U64 buffer_size, U64 read_pos, U64 read_size);

#endif // BASE_RING_H
