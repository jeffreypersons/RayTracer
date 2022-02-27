#include "Material.hpp"
#include "Color.hpp"
#include "Objects.hpp"
#include "Ray.hpp"

#include "gtest/gtest.h"

#include <iostream>

TEST(Intersection, Sphere)
{
    Material reflectiveRed{};
    reflectiveRed.setWeights(0.10f, 0.90f);
    reflectiveRed.setColors(Palette::darkRed, Palette::red, Palette::orangeRed);
    reflectiveRed.setShininess(15);

    Sphere obj{Vec3(0, 0, 0), 10.00f, reflectiveRed};
    Ray ray{{-20.0, 0.0, 0.0}, {1.0, 0.0, 0.0}};

    Intersection intersection;
    bool intersectionOccured = obj.intersect(ray, intersection);

    EXPECT_NEAR(intersection.point.x, -10.0f, 0.1f);
    EXPECT_NEAR(intersection.point.y, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.point.z, 0.0f, 0.1f);
}

TEST(Intersection, SphereInner)
{
    Material reflectiveRed{};
    reflectiveRed.setWeights(0.10f, 0.90f);
    reflectiveRed.setColors(Palette::darkRed, Palette::red, Palette::orangeRed);
    reflectiveRed.setShininess(15);

    Sphere obj{Vec3(0, 0, 0), 10.00f, reflectiveRed};
    Ray ray{{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}};

    Intersection intersection;
    bool intersectionOccured = obj.intersect(ray, intersection);

    EXPECT_NEAR(intersection.point.x, 10.0f, 0.1f);
    EXPECT_NEAR(intersection.point.y, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.point.z, 0.0f, 0.1f);
}
