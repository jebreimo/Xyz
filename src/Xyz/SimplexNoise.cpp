//****************************************************************************
// Copyright © 2022 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2022-05-07.
//
// This file is distributed under the Zero-Clause BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Xyz/SimplexNoise.hpp"

#include <algorithm>

namespace Xyz
{
    // Copied (and slightly adapted)
    // from https://gist.github.com/Flafla2/f0260a861be0ebdeef76

    namespace
    {
        // Hash lookup table as defined by Ken SimplexNoise. This is a randomly
        // arranged array of all numbers from 0-255 inclusive.
        uint8_t PERMUTATION[256] = {
            151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233,
            7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10,
            23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252,
            219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87,
            174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48,
            27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230,
            220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
            63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169,
            200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186,
            3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126,
            255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189,
            28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70,
            221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98,
            108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
            251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51,
            145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157,
            184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236,
            205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66,
            215, 61, 156, 180
        };

        double gradient(int hash, double x, double y, double z)
        {
            switch (hash & 0xF)
            {
            case 0x0: return x + y;
            case 0x1: return -x + y;
            case 0x2: return x - y;
            case 0x3: return -x - y;
            case 0x4: return x + z;
            case 0x5: return -x + z;
            case 0x6: return x - z;
            case 0x7: return -x - z;
            case 0x8: return y + z;
            case 0x9: return -y + z;
            case 0xA: return y - z;
            case 0xB: return -y - z;
            case 0xC: return y + x;
            case 0xD: return -y + z;
            case 0xE: return y - x;
            case 0xF: return -y - z;
            default: return 0;
            }
        }

        double fade(double t)
        {
            // Fade function as defined by Ken SimplexNoise. This eases coordinate
            // values so that they will "ease" towards integral values.
            // This ends up smoothing the final output.
            return t * t * t * (t * (t * 6 - 15) + 10); // 6t^5 - 15t^4 + 10t^3
        }
    }

    SimplexNoise::SimplexNoise()
    {
        std::copy(std::begin(PERMUTATION), std::end(PERMUTATION),
                  permutation_);
        std::copy(std::begin(PERMUTATION), std::end(PERMUTATION),
                  permutation_ + 256);
    }

    double SimplexNoise::simplex(double x, double y, double z)
    {
        // Calculate the "unit cube" that the point asked will be located in.
        // The left bound is ( |_x_|,|_y_|,|_z_| ) and the right bound is that
        // plus 1. Next we calculate the location (from 0.0 to 1.0) in that cube.
        // We also fade the location to smooth the result.
        int xi = int(x) & 255;
        int yi = int(y) & 255;
        int zi = int(z) & 255;
        double xf = x - int(x);
        double yf = y - int(y);
        double zf = z - int(z);
        double u = fade(xf);
        double v = fade(yf);
        double w = fade(zf);

        int aaa, aba, aab, abb, baa, bba, bab, bbb;
        aaa = permutation_[permutation_[permutation_[xi] + yi] + zi];
        aba = permutation_[permutation_[permutation_[xi] + ++yi] + zi];
        aab = permutation_[permutation_[permutation_[xi] + yi] + ++zi];
        abb = permutation_[permutation_[permutation_[xi] + ++yi] + ++zi];
        baa = permutation_[permutation_[permutation_[++xi] + yi] + zi];
        bba = permutation_[permutation_[permutation_[++xi] + ++yi] + zi];
        bab = permutation_[permutation_[permutation_[++xi] + yi] + ++zi];
        bbb = permutation_[permutation_[permutation_[++xi] + ++yi] + ++zi];

        double x1, x2, y1, y2;
        // The gradient function calculates the dot product between a pseudorandom
        // gradient vector and the vector from the input coordinate to the 8
        // surrounding points in its unit cube.
        x1 = std::lerp(gradient(aaa, xf, yf, zf),
                       gradient(baa, xf - 1, yf, zf),
                       u);
        // This is all then lerped together as a sort of weighted average based on
        // the faded (u,v,w) values we made earlier.
        x2 = std::lerp(gradient(aba, xf, yf - 1, zf),
                       gradient(bba, xf - 1, yf - 1, zf),
                       u);
        y1 = std::lerp(x1, x2, v);

        x1 = std::lerp(gradient(aab, xf, yf, zf - 1),
                       gradient(bab, xf - 1, yf, zf - 1),
                       u);
        x2 = std::lerp(gradient(abb, xf, yf - 1, zf - 1),
                       gradient(bbb, xf - 1, yf - 1, zf - 1),
                       u);
        y2 = std::lerp(x1, x2, v);
        // For convenience, we reduce it to 0 - 1 (theoretical min/max before
        // is -1 - 1)
        return (std::lerp(y1, y2, w) + 1) / 2;
    }

    double SimplexNoise::simplex(double x, double y, double z,
                                 int octaves, double persistence)
    {
        double total = 0;
        double frequency = 1;
        double amplitude = 1;
        double max_value = 0;
        for (int i = 0; i < octaves; i++)
        {
            total += simplex(x * frequency, y * frequency, z * frequency)
                     * amplitude;
            max_value += amplitude;
            amplitude *= persistence;
            frequency *= 2;
        }
        return total / max_value;
    }
}
