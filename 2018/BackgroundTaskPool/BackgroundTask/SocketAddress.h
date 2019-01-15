/*!------------------------------------------------------------------------------
\file   SocketAddress.h

\author Garrett Conti

\par    Project: Assignment 4
\par    Course:  CS260
   ------------------------------------------------------------------------------ */

#pragma once

#include <WinSock2.h>

class SocketAddress
{
  public:
    static constexpr size_t Size() noexcept;

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

constexpr size_t SocketAddress::Size() noexcept
{
  return sizeof( decltype( Data ) );
}
