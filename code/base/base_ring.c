////////////////////////////////
//~ rjf: Ring Functions

root_function U64
RingWrite(U8 *base, U64 size, U64 write_pos, String8 string)
{
 U64 first_part_write_off = write_pos % size;
 U64 second_part_write_off = 0;
 String8 first_part = string;
 String8 second_part = Str8Lit("");
 if(first_part_write_off + string.size > size)
 {
  first_part.size = size - first_part_write_off;
  second_part = Str8Skip(string, first_part.size);
 }
 if(first_part.size != 0)
 {
  MemoryCopy(base + first_part_write_off, first_part.str, first_part.size);
 }
 if(second_part.size != 0)
 {
  MemoryCopy(base + second_part_write_off, second_part.str, second_part.size);
 }
 return string.size;
}

root_function U64
RingRead(void *dst, U8 *base, U64 buffer_size, U64 read_pos, U64 read_size)
{
 U64 first_part_read_off = read_pos % buffer_size;
 U64 second_part_read_off = 0;
 U64 first_part_read_size = read_size;
 U64 second_part_read_size = 0;
 if(first_part_read_off + read_size > buffer_size)
 {
  first_part_read_size = buffer_size - first_part_read_off;
  second_part_read_size = read_size - first_part_read_size;
 }
 if(first_part_read_size != 0)
 {
  MemoryCopy(dst, base + first_part_read_off, first_part_read_size);
 }
 if(second_part_read_size != 0)
 {
  MemoryCopy((U8 *)dst + first_part_read_size, base + second_part_read_off, second_part_read_size);
 }
 return read_size;
}
