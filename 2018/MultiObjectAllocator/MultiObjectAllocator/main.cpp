// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>

#include "MultiObjectAllocator.h"

int main()
{
  MultiObjectAllocator< int, bool > Test1{ { 10, 12 } };
  MultiObjectAllocator< int, bool > Test2{ { 10 } };
  MultiObjectAllocator< int, double > Test3{};
  //MultiObjectAllocator< int, bool > Test( 10 ); // ERROR
  // MultiObjectAllocator< int, bool > Test{ 10 }; // ERROR

  // Fill up the buffer, should allocate 15 doubles,
  do
  {
    Test3.Allocate< double >();
  }
  while( Test3.IsFull< double >() == false );

  // Resize
  Test3.Allocate< double >();

  std::cout << "Hello World!\n";
}