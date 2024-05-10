#include "MeshGen.h"

#include <cassert>

constexpr float Pi = 3.14159265358979323846f;

std::vector<Vertex_Position> GenerateTriangle_Position()
{
    /*
    //         A 
    //        / \ 
    //       /   \ 
    //      C-- - B
    */

    std::vector<Vertex_Position> vertices =
    {
        // ACB
        Vertex_Position{ glm::vec3(0.0f, 0.5f, 0.0f) },
        Vertex_Position{ glm::vec3(-0.5f, -0.5f, 0.0f) },   
        Vertex_Position{ glm::vec3(0.5f, -0.5f, 0.0f) }
    };

    return vertices;
}

std::vector<Vertex_Textured> GenerateTriangle_Textured()
{
    std::vector<Vertex_Textured> vertices =
    {
        // ACB
        Vertex_Textured{ glm::vec3(0.0f, 0.5f, 0.0f), glm::vec2(0.5f, 1.0f) },
        Vertex_Textured{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f) },   
        Vertex_Textured{ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f) }
    };

    return vertices;
}

std::vector<Vertex_Position> GenerateRectangle_Position()
{
    /*
    //      A --  B
    //      |   / |
    //      | /   |
    //      C  -- D
    */

    std::vector<Vertex_Position> vertices =
    {
        // ACB
        Vertex_Position{ glm::vec3(-0.5f, 0.5f, 0.0f) },
        Vertex_Position{ glm::vec3(-0.5f, -0.5f, 0.0f) },
        Vertex_Position{ glm::vec3(0.5f, 0.5f, 0.0f) },

        // BCD
        Vertex_Position{ glm::vec3(0.5f, 0.5f, 0.0f) },
        Vertex_Position{ glm::vec3(-0.5f, -0.5f, 0.0f) },
        Vertex_Position{ glm::vec3(0.5f, -0.5f, 0.0f) }
    };

    return vertices;
}

std::vector<Vertex_Textured> GenerateRectangle_Textured()
{
    std::vector<Vertex_Textured> vertices =
    {
        // ACB
        Vertex_Textured{ glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f) },
        Vertex_Textured{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f) },
        Vertex_Textured{ glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f, 1.0f) },

        // BCD
        Vertex_Textured{ glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f, 1.0f) },
        Vertex_Textured{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f) },
        Vertex_Textured{ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f) }
    };

    return vertices;
}

std::vector<Vertex_Position> GenerateCube_Position()
{
    /*
    //      A --  B
    //      |   / |
    //      | /   |
    //      C  -- D
    */

    std::vector<Vertex_Position> vertices =
    {
        /*
        // North (-z)
        */

        // ACB
        Vertex_Position { glm::vec3(-0.5f, 0.5f, -0.5f) },
        Vertex_Position { glm::vec3(-0.5f, -0.5f, -0.5f) },
        Vertex_Position { glm::vec3(0.5f, 0.5f, -0.5f) },

        // BCD
        Vertex_Position { glm::vec3(0.5f, 0.5f, -0.5f) },
        Vertex_Position { glm::vec3(-0.5f, -0.5f, -0.5f) },
        Vertex_Position { glm::vec3(0.5f, -0.5f, -0.5f) },

        /*
        // South (+z)
        */

        // ACB
        Vertex_Position { glm::vec3(-0.5f, 0.5f, 0.5f) },
        Vertex_Position { glm::vec3(-0.5f, -0.5f, 0.5f) },
        Vertex_Position { glm::vec3(0.5f, 0.5f, 0.5f) },

        // BCD
        Vertex_Position { glm::vec3(0.5f, 0.5f, 0.5f) },
        Vertex_Position { glm::vec3(-0.5f, -0.5f, 0.5f) },
        Vertex_Position { glm::vec3(0.5f, -0.5f, 0.5f) },

        /*
        // East (+x)
        */

        // ACB
        Vertex_Position { glm::vec3(0.5f, 0.5f, -0.5f) },
        Vertex_Position { glm::vec3(0.5f, -0.5f, -0.5f) },
        Vertex_Position { glm::vec3(0.5f, 0.5f, 0.5f) },

        // BCD
        Vertex_Position { glm::vec3(0.5f, 0.5f, 0.5f) },
        Vertex_Position { glm::vec3(0.5f, -0.5f, -0.5f) },
        Vertex_Position { glm::vec3(0.5f, -0.5f, 0.5f) },

        /*
        // West (-x)
        */

        // ACB
        Vertex_Position { glm::vec3(-0.5f, 0.5f, -0.5f) },
        Vertex_Position { glm::vec3(-0.5f, -0.5f, -0.5f) },
        Vertex_Position { glm::vec3(-0.5f, 0.5f,  0.5f) },

        // BCD
        Vertex_Position { glm::vec3(-0.5f, 0.5f, 0.5f) },
        Vertex_Position { glm::vec3(-0.5f, -0.5f, -0.5f) },
        Vertex_Position { glm::vec3(-0.5f, -0.5f, 0.5f) },

        /*
        // Up (+y)
        */

        // ACB
        Vertex_Position { glm::vec3(0.5f, 0.5f, -0.5f) },
        Vertex_Position { glm::vec3(-0.5f, 0.5f, -0.5f) },
        Vertex_Position { glm::vec3(0.5f, 0.5f, 0.5f) },

        // BCD
        Vertex_Position { glm::vec3(0.5f, 0.5f, 0.5f) },
        Vertex_Position { glm::vec3(-0.5f, 0.5f, -0.5f) },
        Vertex_Position { glm::vec3(-0.5f, 0.5f, 0.5f) },

        /*
        // Down (-y)
        */

        // ACB
        Vertex_Position { glm::vec3(0.5f, -0.5f, -0.5f) },
        Vertex_Position { glm::vec3(-0.5f, -0.5f, -0.5f) },
        Vertex_Position { glm::vec3(0.5f, -0.5f, 0.5f) },

        // BCD
        Vertex_Position { glm::vec3(0.5f, -0.5f, 0.5f) },
        Vertex_Position { glm::vec3(-0.5f, -0.5f, -0.5f) },
        Vertex_Position { glm::vec3(-0.5f, -0.5f, 0.5f) },
    };

    return vertices;
}

std::vector<Vertex_Position> GenerateUVSphere_Position(uint32_t segmentCount, uint32_t ringCount)
{
    std::vector<Vertex_Position> temp_vertices;

    const uint32_t temp_vertexCount = (1 + 1 + (segmentCount * (ringCount - 1)));
    temp_vertices.reserve(temp_vertexCount);

    // Add the bottom vertex.
    Vertex_Position v0;
    v0.Pos = glm::vec3(0.0f,- 1.0f, 0.0f);
    temp_vertices.push_back(v0);

    // Add the vertices for the middle part.
    for (uint32_t ringIndex = 0; ringIndex < (ringCount - 1); ringIndex++)
    {
        float_t phi = (Pi / float_t(ringCount)) * (ringIndex + 1);

        // Adjust phi by -90 degrees to align with the coordinate system where the Y-axis is up.
        phi -= 0.5f * Pi;

        for (uint32_t segmentIndex = 0; segmentIndex < segmentCount; segmentIndex++)
        {
            const float theta = ((2.0f * Pi) / static_cast<float>(segmentCount)) * static_cast<float>(segmentIndex);

            /*
            //  sin(yaw) z     +            sin(pitch) y     +  
            //           |   / |                       |   / |
            //           | /   |                       | /   |
            //           ++ -  + x                     ++ -  + x/z
            //              cos(yaw)                        cos(pitch)
            */  

            Vertex_Position vertex;
            vertex.Pos.x = cosf(phi) * cosf(theta);
            vertex.Pos.y = sinf(phi);
            vertex.Pos.z = cosf(phi) * sinf(theta);

            temp_vertices.push_back(vertex);
        }
    }

    // Add the bottom vertex.
    Vertex_Position v1;
    v1.Pos = glm::vec3(0.0f, 1.0f, 0.0f);
    temp_vertices.push_back(v1);

    std::vector<Vertex_Position> vertices; // Triangulated vertices.

    const uint32_t vertexCount = (1 + 4 + 1) * (segmentCount * (ringCount - 1));
    vertices.reserve(vertexCount);

    // Add the bottom and the top triangles.
    /*
    //    i1-- - i2             v1 
    //      \   /               / \ 
    //       \ /               /   \ 
    //        v0             i1-- - i2
    */

    for (uint32_t segmentIndex = 0; segmentIndex < segmentCount; segmentIndex++)
    { 
        // Bottom triangle.
        uint32_t ringIndex = 0;

        uint32_t i1 = (1 + (segmentIndex       + ((ringIndex    ) * segmentCount)));
        uint32_t i2 = (1 + (i1 % segmentCount) + ((ringIndex    ) * segmentCount));
 
        vertices.push_back(v0);
        vertices.push_back(temp_vertices[i1]);
        vertices.push_back(temp_vertices[i2]);

        // Top triangle.
        ringIndex = (ringCount - 2);
        i1 = (1 + (segmentIndex       + ((ringIndex    ) * segmentCount)));
        i2 = (1 + (i1 % segmentCount) + ((ringIndex    ) * segmentCount));

        vertices.push_back(v1);
        vertices.push_back(temp_vertices[i1]);
        vertices.push_back(temp_vertices[i2]);
    }

    // Add the triangles for each segment of each ring.
    /*
    //     j1 --  j2
    //      |   / |
    //      | /   |
    //     i1  -- i2
    */

    for (uint32_t ringIndex = 0; ringIndex < (ringCount - 2); ringIndex++) 
    {
        for (uint32_t segmentIndex = 0; segmentIndex < segmentCount; segmentIndex++)
        {
            uint32_t i1 = (1 + (segmentIndex       + ((ringIndex    ) * segmentCount)));
            uint32_t i2 = (1 + (i1 % segmentCount) + ((ringIndex    ) * segmentCount));

            uint32_t j1 = (1 + (segmentIndex       + ((ringIndex + 1) * segmentCount)));
            uint32_t j2 = (1 + (i1 % segmentCount) + ((ringIndex + 1) * segmentCount));

            // 1st triangle.
            vertices.push_back(temp_vertices[i1]);
            vertices.push_back(temp_vertices[j1]);
            vertices.push_back(temp_vertices[j2]);

            // 2nd triangle.
            vertices.push_back(temp_vertices[j2]);
            vertices.push_back(temp_vertices[i1]);
            vertices.push_back(temp_vertices[i2]);
        }
    }

    return vertices;
}