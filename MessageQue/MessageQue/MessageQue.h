#pragma once

#include <string>

#include <Windows.h>

#include "Tracer.h"

class MessageQue
{
  public:
    struct BadQueCreation
    {
      explicit BadQueCreation( const std::string &PipeName ) noexcept;

      std::string What;
    };

    struct BadProcessCreation
    {
      explicit BadProcessCreation( const std::string &ExecProgram ) noexcept;

      std::string What;
    };   

    struct QueNotConnected
    {
      explicit QueNotConnected() noexcept;

      std::string What;
    };    

  public:
  /**
     * \brief Creates a sudo message que to another process.
     *
     * \param ExecProgram - The program to run and connect to.
     * \param QueName     - Name for the Que.
     * \param MessageSize - Max size of a message. (Not including null char)
     *
     * \except BadQueCreation     - Could not create the que.
     * \except BadProcessCreation - Could not create the other process.
     * \except QueNotConnected    - Could not connect to the que.
     */
    explicit MessageQue( const std::string &ExecProgram, const std::string &QueName, size_t MessageSize = 1 /* In characters */,
                         float RoutineCheckTime = 1.0f /* This is in seconds */ );

    /**
     * \brief Clears everything up, must wait for all messages to be passed before returning.
     */
    ~MessageQue() noexcept;

  public:
    /**
     * \brief Send a msg to the other process.
     *
     * \param Message - The msg to send.
     *
     * \return void
     * 
     * It is advised to send a special message to the other program to speed up msg reads when you know you are going to close ahead of time.
     * \par
     * Do not send duplicate messages consecutively! This will cause the other program to close!
     */
    void SendMsg( const std::string &Message ) const ExceptDebug;

  private:
    struct MsgTooLarge
    {
      explicit MsgTooLarge( const std::string &Message, int Size ) noexcept;
    };

  private:
    static const std::string CloseProgram; // This is "-1", it will be sent to signal that the other program needs to close.

  private: 
    const size_t MessageSize;

    STARTUPINFO StartInfo;
    PROCESS_INFORMATION ProcInfo;

    HANDLE Pipe;
};
