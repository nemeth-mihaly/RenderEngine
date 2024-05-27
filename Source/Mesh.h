#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

#include "Common.h"
#include "VertexArray.h"
#include "Graphics3D/Geometry.h"

////////////////////////////////////////////////////
//  class Mesh_t
////////////////////////////////////////////////////

class Mesh_t
{
 public:
    Mesh_t();
    ~Mesh_t();

    void LoadFromFile(const std::string& Filename);

    const std::shared_ptr<VertexArray_t>& GetVertexArray() const { return VertexArray; }

 private:
    std::shared_ptr<VertexArray_t> VertexArray;
};