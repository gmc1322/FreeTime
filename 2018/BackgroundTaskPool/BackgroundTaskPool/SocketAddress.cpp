/*!------------------------------------------------------------------------------
\file   SocketAddress.cpp

\author Garrett Conti

\par    Project: Assignment 4
\par    Course:  CS260
   ------------------------------------------------------------------------------ */

#include "SocketAddress.h"

#ifndef WIN32_LEAN_AND_MEAN
#define LAM_DEF
#define WIN32_LEAN_AND_MEAN
#endif

#include <WS2tcpip.h>

#ifdef LAM_DEF
#undef WIN32_LEAN_AND_MEAN
#endif

#pragma comment( lib, "Ws2_32.lib" )

SocketAddress::SocketAddress() noexcept  : Data{ AF_INET, {} }
{
}

SocketAddress::SocketAddress( const char *const Address, const unsigned short Port ) noexcept : Data{ AF_INET, {} }
{
  auto &DataIn = reinterpret_cast< sockaddr_in & >( Data );

  DataIn.sin_port = htons( Port );

  inet_pton( AF_INET, Address, &( DataIn.sin_addr ) );
}

SocketAddress::SocketAddress( const unsigned short Port ) noexcept : Data{ AF_INET, {} }
{
  reinterpret_cast< sockaddr_in & >( Data ).sin_port = htons( Port );
}

SocketAddress::SocketAddress( const sockaddr *const Adder ) noexcept : Data{ *Adder }
{
}

SocketAddress::operator bool() const noexcept
{
  return reinterpret_cast< const sockaddr_in & >( Data ).sin_addr.s_addr;
}

const sockaddr * SocketAddress::GetSockAddr() const noexcept
{
  return &Data;
}
