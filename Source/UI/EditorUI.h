#pragma once

#include <stddef.h>
#include <stdint.h>

//-----------------------------------------------------------------------------
// class EditorUI
//-----------------------------------------------------------------------------

class EditorUI
{
 public:
    EditorUI();
    ~EditorUI();

    void Init();
    void Draw();

 private:
    bool                    m_bShowSceneObjectList;
};