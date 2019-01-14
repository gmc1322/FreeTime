/*!------------------------------------------------------------------------------
\file   SocketAddress.h

\author Garrett Conti

\par    Project: Assignment 4
\par    Course:  CS260
   ------------------------------------------------------------------------------ */

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define LAM_DEF
#define WIN32_LEAN_AND_MEAN
#endif

#include <WinSock2.h>

#ifdef LAM_DEF
#undef WIN32_LEAN_AND_MEAN
#endif

class SocketAddress
{
  public:
    static constexpr int Size() noexcept;

  public:
    SocketAddress() noexcept;
    SocketAddress( const char *Address, unsigned short Port ) noexcept;

    explicit SocketAddress( unsigned short Port ) noexcept;
    explicit SocketAddress( const sockaddr *Adder ) noexcept;

  public:
    operator bool() const noexcept;

  public:
    const sockaddr * GetSockAddr() const noexcept;

  private:
    sockaddr Data;
};

constexpr int SocketAddress::Size() noexcept
{
  return static_cast< int >( sizeof( decltype( Data ) ) );
}
