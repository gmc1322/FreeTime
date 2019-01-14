/*!----------------------------------------------------------------------------
\file   Serializer.h

\author Garrett Conti

\par    Project: Serializer
\par    Course:  S17

\copyright 
   ---------------------------------------------------------------------------- */

#include <fstream>
#include <iostream>

#include "Serializer.h"

Serializer::DataInfo::DataInfo( const char *Tag_, const char *Name_, const SerializerData& Data_ ) noexcept :
  Tag( Tag_ ), Name( Name_ ), Data( Data_ )
{
}

Serializer::DataInfo::DataInfo( const std::string &Tag_, const std::string &Name_, const SerializerData &Data_ ) noexcept :
  Tag( Tag_ ), Name( Name_ ), Data( Data_ )
{
}

Serializer::BadFile::BadFile( const char *String ) noexcept
{
  std::cerr << "Bad file name:" << String << "!" << std::endl;
}

Serializer::BadFile::BadFile( const std::string &String ) noexcept
{
  std::cerr << "Bad file name:" << String << "!" << std::endl;
}

Serializer::BadTag::BadTag( const std::string &Tag ) noexcept
{
  std::cerr << "Bad tag: "<< Tag << "!" << std::endl;
}

Serializer::BadName::BadName( const std::string &Name ) noexcept
{
  std::cerr << "Bad name: " << Name << "!" << std::endl;
}

Serializer::Serializer( const char* FileName_ ) : FileName( FileName_ )
{
  FileName.append( ".bin" );

  std::ofstream FileCreator( FileName, std::ios_base::app ); // Create file if there is none

  FileCreator.close();

  std::ifstream DataFile( FileName );

  if( !DataFile.is_open() )
  {
    throw( BadFile( FileName_ ) );
  }

  LoadFile( DataFile );
}

Serializer & Serializer::operator<<( const DataInfo &SaveData ) noexcept
{
  auto Iter = FileData.find( SaveData.Tag );

  if( Iter == FileData.end() )
  {
    FileData.emplace( std::make_pair( SaveData.Tag, std::map< std::string, std::string >{} ) );

    FileData[ SaveData.Tag ].emplace( std::make_pair( SaveData.Name, SaveData.Data ) );
  }
  else
  {
    if( ( *Iter ).second.find( SaveData.Name ) == ( *Iter ).second.end() )
    {
      ( *Iter ).second.emplace( std::make_pair( SaveData.Name, SaveData.Data ) );
    }
    else
    {
      ( *Iter ).second[ SaveData.Name ] = SaveData.Data;
    }
  }

  return *this;
}

const Serializer & Serializer::operator>>( DataInfo &LoadData ) const
{
  auto Iter = FileData.find( LoadData.Tag );

  if( Iter == FileData.end() )
  {
    throw( BadTag( LoadData.Tag ) );
  }

  auto DataIter = ( *Iter ).second.find( LoadData.Name );

  if( DataIter == ( *Iter ).second.end() )
  {
    throw( BadName( LoadData.Name ) );
  }

  LoadData.Data = ( *DataIter ).second;

  return *this;
}

void Serializer::Save()
{
  std::ofstream DataFile( FileName, std::ios_base::out | std::ios_base::trunc );

  if( !DataFile.is_open() )
  {
    throw( BadFile( FileName ) );
  }

  for( auto Iter : FileData )
  {
    for( auto NameIter : Iter.second )
    {
      DataFile << Iter.first << " " << NameIter.first << " " << NameIter.second << "\n";
    }
  }
}

void Serializer::Load( const char *FileName_ )
{
  FileName = FileName_;

  FileName.append( ".bin" );

  std::ifstream DataFile( FileName );

  if( !DataFile.is_open() )
  {
    throw( BadFile( FileName_ ) );
  }

  LoadFile( DataFile );
}

void Serializer::LoadFile( std::ifstream &File )
{
  std::string Data;

  while( std::getline( File, Data ) )
  {
    const size_t NameOffset = Data.find( " " );

    const size_t DataOffset = Data.find( " ", NameOffset + 1 );

    const std::string Tag{ Data.substr( 0, NameOffset ) };
    const std::string Name{ Data.substr( NameOffset + 1, DataOffset - NameOffset - 1 ) };

    if( FileData.find( Tag ) == FileData.end() )
    {
      FileData.emplace( std::make_pair( Tag, std::map< std::string, std::string >{} ) );
    }

    FileData[ Tag ].emplace( std::make_pair( Name, Data.substr( DataOffset + 1 ) ) );
  }
}
