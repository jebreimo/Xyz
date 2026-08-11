//
// Created by jeb on 04.08.2026.
//
#include <iostream>
#include <Xyz/Xyz.hpp>

void check_z_axis()
{
    int n = 0;
    for (int yaw = 0; yaw < 360; yaw += 30)
    {
        for (int pitch = 0; pitch < 360; pitch += 30)
        {
            for (int roll = 0; roll < 360; roll += 30)
            {
                const Xyz::Orientation3D o(Xyz::to_radians(yaw), Xyz::to_radians(pitch), Xyz::to_radians(roll));
                auto [x, y, z] = get_vectors(o);
                auto zz = normalize(cross(x, y));
                if (!are_equal(z, zz))
                {
                    ++n;
                    std::cout << n << ": " << yaw << ", " << pitch << ", " << roll << ": " << z << " vs " << zz << "\n";
                }
            }
        }
    }
}

int main()
{
    check_z_axis();
    return 0;
}
