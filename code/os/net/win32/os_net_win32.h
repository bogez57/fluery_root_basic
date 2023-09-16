/* date = June 6th 2023 5:53 pm */

#ifndef OS_NET_WIN32_H
#define OS_NET_WIN32_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

////////////////////////////////
//~ rjf: Helpers

function int OS_W32_AFFromNetAddrFamily(OS_NetAddrFamily fam);
function OS_NetAddrFamily OS_W32_NetAddrFamilyFromAF(int af);
function struct sockaddr_in OS_W32_SockAddrInFromNetAddr(OS_NetAddr *net_addr);
function OS_NetAddr OS_W32_NetAddrFromSockAddrIn(struct sockaddr_in *sock_addr);

#endif // OS_NET_WIN32_H
