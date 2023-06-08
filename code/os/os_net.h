/* date = June 6th 2023 5:53 pm */

#ifndef OS_NET_H
#define OS_NET_H

////////////////////////////////
//~ rjf: Receipts

typedef struct OS_InitNetReceipt OS_InitNetReceipt;
struct OS_InitNetReceipt
{
 U64 u64[1];
};

////////////////////////////////
//~ rjf: @os_per_backend Initialization

core_function OS_InitNetReceipt OS_InitNet(OS_InitReceipt os_init);

////////////////////////////////
//~ rjf: @os_per_backend Sockets

core_function OS_Handle OS_SocketOpen(void);
core_function void OS_SocketClose(OS_Handle socket);

#endif // OS_NET_H
