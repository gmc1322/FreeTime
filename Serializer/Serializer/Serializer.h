/*!------------------------------------------------------------------------------
\file   Serializer.h

\author Garrett Conti

\par    Project: Serializer
\par    Course:  S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include <map>
#include <string>

#ifdef SERIALIZER_EXPORTS
#define SERIALIZER_API __declspec(dllexport)
#else
#define SERIALIZER_API __declspec(dllimport)
#endif

class Serializer
{
  public:
    typedef std::string SerializerData; // Typedef in case a class already has a std::string constructor

  public:
    struct DataInfo
    {
      SERIALIZER_API DataInfo( const char *Tag_, const char *Name_, const SerializerData &Data_ = {} ) noexcept;
      SERIALIZER_API DataInfo( const std::string &Tag_, const std::string &Name_, const SerializerData &Data_ = {} ) noexcept;

      std::string Tag;  // Generalized Catagory
      std::string Name; // Specific Data Name
      SerializerData Data; // The converted Data
    };

  public:
    struct BadFile
    {
      SERIALIZER_API explicit BadFile( const char *String ) noexcept;
      SERIALIZER_API explicit BadFile( const std::string &String ) noexcept;
    };

    struct BadTag
    {
      SERIALIZER_API explicit BadTag( const std::string &Tag ) noexcept;
    };

    struct BadName
    {
      SERIALIZER_API explicit BadName( const std::string &Name ) noexcept;
    };

  public:
    /**
     * \brief Creates a Serializer.
     *
     * \param FileName - The file for the Serializer to save to and load from.
     * 
     * \exception BadFile - The input file could not be opened or created.
     */
    SERIALIZER_API explicit Serializer( const char *FileName );

  public:
    /**
     * \brief Adds new data to be saved, or overwrites existing data to be saved.
     *
     * \param[ in ] SaveData - The Data to save.
     *
     * \return Serializer &
     */
    SERIALIZER_API Serializer & operator<<( const DataInfo &SaveData ) noexcept;

    /**
     * \brief Finds data from the file and puts in the passed in DataInfo.
     *
     * \param[ in,out ] LoadData - The tag and name to get and the holder for the Data.
     *
     * \exception BadTag  - The tag in LoadData was bad.
     * \exception BadName - The name in LoadData was bad.
     *
     * \return const Serializer &
     */
    SERIALIZER_API const Serializer & operator>>( DataInfo &LoadData ) const;

  public:
    /**
     * \brief Saves all buffered data.
     *
     * \exception BadFile - The input file could not be opened or created.
     *
     * \return void
     */
    SERIALIZER_API void Save();


    /**
     * \brief Loads a new file.
     *
     * \param FileName_ - The file to replace the currently stored file.
     *
     * \exception BadFile - The input file could not be found.
     *
     * \return void
     */
    SERIALIZER_API void Load( const char *FileName_ );

  public:  
    /**
     * \brief Creates a Data Info for built-in types. Dont use with classes!
     *
     * \tparam DataType - The built-in type to convert.
     * 
     * \param Tag       - The Tag for the Data.
     * \param Name      - The Name for the Data.
     * \param Data      - The Data to convert.
     *
     * \return DataInfo
     */
    template< typename DataType >
    static DataInfo CreateSaveData( const char *Tag, const char *Name, const DataType &Data );

  private:
  SERIALIZER_API void LoadFile( std::ifstream &File );

  private:
    std::string FileName;

           // Tag                    Name         Data
    std::map< std::string, std::map< std::string, SerializerData > > FileData;
};

template< typename DataType >
Serializer::DataInfo Serializer::CreateSaveData( const char *Tag, const char *Name, const DataType &Data )
{
  DataInfo Return{ Tag, Name };

  Return.Data = std::to_string( Data );

  return Return;
}
