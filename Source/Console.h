#pragma once

#include <string>
#include <vector>

//-----------------------------------------------------------------------------
// class Console
//-----------------------------------------------------------------------------

class Console
{
public:
    Console();

    void Init();
    void Draw();

private:
    bool                        m_bScrollDown;
    std::vector<std::string>    m_LastTexts;
};