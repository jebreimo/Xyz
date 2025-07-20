//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-06-03.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <vector>
#include "Face.hpp"
#include "Vector.hpp"

namespace Xyz
{
    template <typename T>
    class Mesh
    {
    public:
        using Vertex = Vector<T, 3>;

        Mesh() = default;

        Mesh(std::vector<Vertex> vertexes, std::vector<Face> faces)
            : vertexes_(std::move(vertexes)),
              faces_(std::move(faces))
        {
            assign_face_ids(faces_);
        }

        [[nodiscard]]
        const std::vector<Vertex>& vertexes() const
        {
            return vertexes_;
        }

        void add_vertex(const Vertex& v)
        {
            vertexes_.push_back(v);
        }

        void set_vertex(size_t n, const Vertex& v)
        {
            vertexes_[n] = v;
        }

        void set_vertexes(std::vector<Vertex> vertexes)
        {
            vertexes_ = std::move(vertexes);
        }

        [[nodiscard]]
        const std::vector<Face>& faces() const
        {
            return faces_;
        }

        void add_face(const Face& face)
        {
            faces_.push_back(face);
            faces_.back().set_id(FaceId(faces_.size() - 1));
        }

        void set_faces(std::vector<Face> faces)
        {
            faces_ = std::move(faces);
            assign_face_ids(faces_);
        }

        Vertex normal(const Face& face) const
        {
            return normalize(cross(vertexes_[face[1]] - vertexes_[face[0]],
                                  vertexes_[face[2]] - vertexes_[face[1]]));
        }
    private:
        void assign_face_ids(std::vector<Face>& faces)
        {
            FaceId n = 0;
            std::for_each(faces.begin(), faces.end(),
                          [&n](auto& f) {f.set_id(n++);});
        }

        std::vector<Vertex> vertexes_;
        std::vector<Face> faces_;
    };
}
