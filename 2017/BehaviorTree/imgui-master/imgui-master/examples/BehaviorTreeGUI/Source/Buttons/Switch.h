/*!------------------------------------------------------------------------------
\file   Switch.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include <string>

class Switch
{
  public:
    Switch( const char *Name, std::initializer_list< const char * >Strings ) noexcept;

    ~Switch();

  public:
    void RunSwitch() noexcept;

  public:
    int GetSelected() const noexcept;

    const char * GetSelectedString() const noexcept;

  public:
    void SetSwitch( const std::string &String ) noexcept;

  private:
    std::string Name_;

    const char * *Items_;

    const size_t ItemsCount_;

    int SelectedItem = 0;
};
