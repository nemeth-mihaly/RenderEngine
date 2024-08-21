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

    void Initialize();
    void Draw();

private:
    bool                        m_bScroll = false;
    std::vector<std::string>    m_LastTexts;
};