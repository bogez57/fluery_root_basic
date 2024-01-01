#include "base/base_inc.h"
#include "os/os_inc.h"
#include "base/base_inc.c"
#include "os/os_inc.c"
#include "os/core/os_entry_point.c"

typedef struct Foo Foo;
struct Foo
{
 String8 name;
 Vec4F32 color;
};

typedef struct FooNode FooNode;
struct FooNode
{
 FooNode *next;
 FooNode *prev;
 FooNode *first;
 FooNode *last;
 FooNode *parent;
 FooNode *hash_next;
 FooNode *hash_prev;
 Foo v;
};

typedef struct FooSlot FooSlot;
struct FooSlot
{
 FooNode *first;
 FooNode *last;
};

typedef struct FooChunkNode FooChunkNode;
struct FooChunkNode
{
 FooChunkNode *next;
 Foo *v;
 U64 cap;
 U64 count;
};

typedef struct FooChunkList FooChunkList;
struct FooChunkList
{
 FooChunkNode *first;
 FooChunkNode *last;
 U64 chunk_count;
 U64 total_count;
};

typedef struct FooArray FooArray;
struct FooArray
{
 Foo *v;
 U64 count;
};

function U64
HashFromString(String8 string)
{
 U64 result = 5381;
 for(U64 i = 0; i < string.size; i += 1)
 {
  result = ((result << 5) + result) + string.str[i];
 }
 return result;
}

function void
EntryPoint(CmdLine *cmdln)
{
 Arena *arena = ArenaAllocDefault();
 
 ///////////////////////////////
 //- rjf: single struct example
 //
 {
  Foo *foo = PushArray(arena, Foo, 1);
  foo->name = Str8Lit("single struct foo");
  foo->color = V4F32(1, 0, 0, 1);
  int x = 0;
 }
 
 ///////////////////////////////
 //- rjf: array of structs example
 //
 {
  U64 foos_count = 32;
  Foo *foos = PushArray(arena, Foo, foos_count);
  for(U64 idx = 0; idx < foos_count; idx += 1)
  {
   Foo *foo = &foos[idx];
   foo->name = PushStr8F(arena, "foo in array #%I64u", idx);
   foo->color = V4F32(1, 0, 0, 1);
  }
  int x = 0;
 }
 
 ///////////////////////////////
 //- rjf: singly-linked stack example
 //
 {
  FooNode *top_foo = 0;
  {
   FooNode *n = PushArray(arena, FooNode, 1);
   StackPush(top_foo, n);
   n->v.name = Str8Lit("linked list foo 1");
   n->v.color = V4F32(0, 1, 0, 1);
  }
  {
   FooNode *n = PushArray(arena, FooNode, 1);
   StackPush(top_foo, n);
   n->v.name = Str8Lit("linked list foo 2");
   n->v.color = V4F32(0, 0, 1, 1);
  }
  {
   FooNode *n = PushArray(arena, FooNode, 1);
   StackPush(top_foo, n);
   n->v.name = Str8Lit("linked list foo 3");
   n->v.color = V4F32(1, 1, 0, 1);
  }
  int x = 0;
 }
 
 ///////////////////////////////
 //- rjf: singly-linked queue example
 //
 {
  FooNode *first_foo = 0;
  FooNode *last_foo = 0;
  {
   FooNode *n = PushArray(arena, FooNode, 1);
   QueuePush(first_foo, last_foo, n);
   n->v.name = Str8Lit("linked list foo 1");
   n->v.color = V4F32(0, 1, 0, 1);
  }
  {
   FooNode *n = PushArray(arena, FooNode, 1);
   QueuePush(first_foo, last_foo, n);
   n->v.name = Str8Lit("linked list foo 2");
   n->v.color = V4F32(0, 0, 1, 1);
  }
  {
   FooNode *n = PushArray(arena, FooNode, 1);
   QueuePush(first_foo, last_foo, n);
   n->v.name = Str8Lit("linked list foo 3");
   n->v.color = V4F32(1, 1, 0, 1);
  }
  int x = 0;
 }
 
 ///////////////////////////////
 //- rjf: chunked linked list example
 //
 {
  FooChunkList foos_list = {0};
  for(U64 idx = 0; idx < 1024; idx += 1)
  {
   FooChunkNode *n = foos_list.last;
   if(n == 0 || n->count >= n->cap)
   {
    n = PushArray(arena, FooChunkNode, 1);
    QueuePush(foos_list.first, foos_list.last, n);
    foos_list.chunk_count += 1;
    n->cap = 256;
    n->v = PushArrayNoZero(arena, Foo, n->cap);
   }
   Foo *foo = &n->v[n->count];
   foo->name = PushStr8F(arena, "chunked linked list foo #%I64u", idx);
   foo->color = V4F32(0, 0, 1, 1);
   n->count += 1;
   foos_list.total_count += 1;
  }
  FooArray foos_array = {0};
  {
   foos_array.count = foos_list.total_count;
   foos_array.v = PushArrayNoZero(arena, Foo, foos_array.count);
   U64 idx = 0;
   for(FooChunkNode *n = foos_list.first; n != 0; n = n->next)
   {
    MemoryCopy(foos_array.v+idx, n->v, sizeof(Foo)*n->count);
    idx += n->count;
   }
  }
  int x = 0;
 }
 
 ///////////////////////////////
 //- rjf: doubly-linked queue example
 //
 {
  FooNode *first_foo = 0;
  FooNode *last_foo = 0;
  FooNode *foo_1 = 0;
  {
   FooNode *n = foo_1 = PushArray(arena, FooNode, 1);
   DLLPushBack(first_foo, last_foo, n);
   n->v.name = Str8Lit("linked list foo 1");
   n->v.color = V4F32(0, 1, 0, 1);
  }
  FooNode *foo_2 = 0;
  {
   FooNode *n = foo_2 = PushArray(arena, FooNode, 1);
   DLLPushBack(first_foo, last_foo, n);
   n->v.name = Str8Lit("linked list foo 2");
   n->v.color = V4F32(0, 0, 1, 1);
  }
  FooNode *foo_3 = 0;
  {
   FooNode *n = foo_3 = PushArray(arena, FooNode, 1);
   DLLPushBack(first_foo, last_foo, n);
   n->v.name = Str8Lit("linked list foo 3");
   n->v.color = V4F32(1, 1, 0, 1);
  }
  FooNode *foo_4 = 0;
  {
   FooNode *n = foo_4 = PushArray(arena, FooNode, 1);
   DLLInsert(first_foo, last_foo, foo_1, n);
   n->v.name = Str8Lit("linked list foo 1.5");
   n->v.color = V4F32(1, 1, 0, 1);
   int x = 0;
   DLLRemove(first_foo, last_foo, n);
  }
  int x = 0;
 }
 
 ///////////////////////////////
 //- rjf: tree example
 //
 {
  FooNode *root = PushArray(arena, FooNode, 1);
  root->v.name = Str8Lit("root");
  root->v.color = V4F32(1, 0, 0, 1);
  for(U64 idx = 0; idx < 3; idx += 1)
  {
   FooNode *child = PushArray(arena, FooNode, 1);
   child->v.name = PushStr8F(arena, "child #%I64u", idx);
   child->v.color = V4F32(0, 1, 0, 1);
   DLLPushBack(root->first, root->last, child);
   child->parent = root;
  }
  int x = 0;
 }
 
 ///////////////////////////////
 //- rjf: linear search example
 //
 {
  String8 names[] =
  {
   Str8Lit("foo"),
   Str8Lit("bar"),
   Str8Lit("baz"),
   Str8Lit("a"),
   Str8Lit("b"),
   Str8Lit("c"),
   Str8Lit("d"),
   Str8Lit("e"),
   Str8Lit("123"),
   Str8Lit("456"),
   Str8Lit("789abc"),
  };
  U64 foos_count = ArrayCount(names);
  Foo *foos = PushArray(arena, Foo, foos_count);
  for(U64 idx = 0; idx < foos_count; idx += 1)
  {
   Foo *foo = &foos[idx];
   foo->name = names[idx];
   foo->color = V4F32(1, 0, 0, 1);
  }
  int x = 0;
  // ... later in some other code which didn't build the table
  String8 key = Str8Lit("123");
  Foo *foo = 0;
  for(U64 idx = 0; idx < foos_count; idx += 1)
  {
   if(Str8Match(foos[idx].name, key, 0))
   {
    foo = &foos[idx];
    break;
   }
  }
  int y = 0;
 }
 
 ///////////////////////////////
 //- rjf: hash table example
 //
 {
  String8 names[] =
  {
   Str8Lit("foo"),
   Str8Lit("bar"),
   Str8Lit("baz"),
   Str8Lit("a"),
   Str8Lit("b"),
   Str8Lit("c"),
   Str8Lit("d"),
   Str8Lit("e"),
   Str8Lit("123"),
   Str8Lit("456"),
   Str8Lit("789abc"),
  };
  U64 slots_count = 256;
  FooSlot *slots = PushArray(arena, FooSlot, slots_count);
  for(U64 idx = 0; idx < ArrayCount(names); idx += 1)
  {
   FooNode *node = PushArray(arena, FooNode, 1);
   node->v.name = names[idx];
   node->v.color = V4F32(1, 0, 0, 1);
   U64 hash = HashFromString(node->v.name);
   U64 slot_idx = hash%slots_count;
   FooSlot *slot = &slots[slot_idx];
   QueuePush(slot->first, slot->last, node);
  }
  int x = 0;
  // ... later in some other code which didn't build the table
  {
   String8 key = Str8Lit("123");
   Foo *foo = 0;
   U64 hash = HashFromString(key);
   U64 slot_idx = hash%slots_count;
   FooSlot *slot = &slots[slot_idx];
   for(FooNode *n = slot->first; n != 0; n = n->next)
   {
    if(Str8Match(n->v.name, key, 0))
    {
     foo = &n->v;
     break;
    }
   }
   int y = 0;
  }
 }
 
 ///////////////////////////////
 //- rjf: hash table tree example
 //
 {
  String8 names[] =
  {
   Str8Lit("foo"),
   Str8Lit("bar"),
   Str8Lit("baz"),
   Str8Lit("a"),
   Str8Lit("b"),
   Str8Lit("c"),
   Str8Lit("d"),
   Str8Lit("e"),
   Str8Lit("123"),
   Str8Lit("456"),
   Str8Lit("789abc"),
  };
  FooNode *root = PushArray(arena, FooNode, 1);
  root->v.name = Str8Lit("root");
  root->v.color = V4F32(1, 0, 0, 1);
  U64 slots_count = 256;
  FooSlot *slots = PushArray(arena, FooSlot, slots_count);
  for(U64 idx = 0; idx < ArrayCount(names); idx += 1)
  {
   FooNode *node = PushArray(arena, FooNode, 1);
   node->v.name = names[idx];
   node->v.color = V4F32(1, 0, 0, 1);
   U64 hash = HashFromString(node->v.name);
   U64 slot_idx = hash%slots_count;
   FooSlot *slot = &slots[slot_idx];
   DLLPushBack_NPZ(slot->first, slot->last, node, hash_next, hash_prev, CheckNull, SetNull);
   DLLPushBack_NPZ(root->first, root->last, node, next, prev, CheckNull, SetNull);
  }
  int x = 0;
  // ... later in some other code which didn't build the table
  {
   String8 key = Str8Lit("123");
   Foo *foo = 0;
   U64 hash = HashFromString(key);
   U64 slot_idx = hash%slots_count;
   FooSlot *slot = &slots[slot_idx];
   for(FooNode *n = slot->first; n != 0; n = n->hash_next)
   {
    if(Str8Match(n->v.name, key, 0))
    {
     foo = &n->v;
     break;
    }
   }
   int y = 0;
  }
 }
}
