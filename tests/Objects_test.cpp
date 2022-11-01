#include "Material.hpp"
#include "Color.hpp"
#include "Objects.hpp"
#include "Ray.hpp"

#include "gtest/gtest.h"

#include <iostream>

TEST(TriangleContains, Yes2D)
{
    float L = 10.0f;
    Vec3 v1{0.0f, 0.0f, 0.0f};
    Vec3 v2{L,    0.0f, 0.0f};
    Vec3 v3{L,    L,    0.0f};
    Triangle triangle{v1, v2, v3, Material()};

    EXPECT_TRUE(triangle.contains(Vec3(1.0f, 0.5f, 0.0f)));
}

TEST(TriangleContains, No2D)
{
    float L = 10.0f;
    Vec3 v1{0.0f, 0.0f, 0.0f};
    Vec3 v2{L,    0.0f, 0.0f};
    Vec3 v3{L,    L,    0.0f};
    Triangle triangle{v1, v2, v3, Material()};

    EXPECT_FALSE(triangle.contains(Vec3(-1.0f, -1.0f, 0.0f)));
}

TEST(TriangleContains, Yes)
{
    float L = 10.0f;
    Vec3 v1{0.0f, 0.0f, 0.0f};
    Vec3 v2{L,    L,    0.0f};
    Vec3 v3{L,    L,    L};
    Triangle triangle{v1, v2, v3, Material()};

    EXPECT_TRUE(triangle.contains(Vec3(0.5f, 0.25f, 0.25f)));
}

TEST(TriangleContains, No)
{
    float L = 10.0f;
    Vec3 v1{0.0f, 0.0f, 0.0f};
    Vec3 v2{L,    L,    0.0f};
    Vec3 v3{L,    L,    L};
    Triangle triangle{v1, v2, v3, Material()};

    EXPECT_FALSE(triangle.contains(Vec3(-1.0f, -1.0f, -1.0f)));
}

TEST(TriangleContains, NoOneDimensionOutside)
{
    float L = 10.0f;
    Vec3 v1{0.0f, 0.0f, 0.0f};
    Vec3 v2{L,    L,    0.0f};
    Vec3 v3{L,    L,    L};
    Triangle triangle{v1, v2, v3, Material()};

    EXPECT_FALSE(triangle.contains(Vec3(0.5f, 0.25f, -1.0f)));
}

TEST(Intersection, TriangleCCW)
{
    float L = 10.0f;
    Vec3 v1{0.0f, 0.0f, 0.0f};
    Vec3 v2{L,    0.0f, 0.0f};
    Vec3 v3{L,    L,    0.0f};
    Triangle obj{v1, v2, v3, Material()};

    Ray ray{{L/2.0f, L/4.0f, -10.0f}, {0.0f, 0.0f, 1.0f}};

    Intersection intersection;
    bool intersectionOccured = obj.intersect(ray, intersection);

    EXPECT_TRUE(intersectionOccured);

    EXPECT_NEAR(intersection.point.x, L/2.0f, 0.1f);
    EXPECT_NEAR(intersection.point.y, L/4.0f, 0.1f);
    EXPECT_NEAR(intersection.point.z, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.normal.x, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.normal.y, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.normal.z, -1.0f, 0.1f);
}

TEST(Intersection, TriangleCW)
{
    float L = 10.0f;
    Vec3 v1{0.0f, 0.0f, 0.0f};
    Vec3 v2{L,    L,    0.0f};
    Vec3 v3{L,    0.0f, 0.0f};
    Triangle obj{v1, v2, v3, Material()};

    Ray ray{{L/2.0f, L/4.0f, -10.0f}, {0.0f, 0.0f, 1.0f}};

    Intersection intersection;
    bool intersectionOccured = obj.intersect(ray, intersection);

    EXPECT_TRUE(intersectionOccured);

    EXPECT_NEAR(intersection.point.x, L/2.0f, 0.1f);
    EXPECT_NEAR(intersection.point.y, L/4.0f, 0.1f);
    EXPECT_NEAR(intersection.point.z, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.normal.x, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.normal.y, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.normal.z, -1.0f, 0.1f);
}

TEST(Intersection, TriangleRayPointingAway)
{
    float L = 10.0f;
    Vec3 v1{0.0f, 0.0f, 0.0f};
    Vec3 v2{L,    0.0f, 0.0f};
    Vec3 v3{L,    L,    0.0f};
    Triangle obj{v1, v2, v3, Material()};

    Ray ray{{0.0, 0.0, -10.0}, {0.0, 0.0, -1.0}};

    Intersection intersection;
    bool intersectionOccured = obj.intersect(ray, intersection);

    EXPECT_FALSE(intersectionOccured);
}

TEST(Intersection, Sphere)
{
    Sphere obj{Vec3(0, 0, 0), 10.00f, Material()};
    Ray ray{{-20.0, 0.0, 0.0}, {1.0, 0.0, 0.0}};

    Intersection intersection;
    bool intersectionOccured = obj.intersect(ray, intersection);

    EXPECT_TRUE(intersectionOccured);

    EXPECT_NEAR(intersection.point.x, -10.0f, 0.1f);
    EXPECT_NEAR(intersection.point.y, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.point.z, 0.0f, 0.1f);
}

TEST(Intersection, SphereInner)
{
    Sphere obj{Vec3(0, 0, 0), 10.00f, Material()};
    Ray ray{{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}};

    Intersection intersection;
    bool intersectionOccured = obj.intersect(ray, intersection);

    EXPECT_TRUE(intersectionOccured);

    EXPECT_NEAR(intersection.point.x, 10.0f, 0.1f);
    EXPECT_NEAR(intersection.point.y, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.point.z, 0.0f, 0.1f);
}

TEST(Intersection, SphereRayPointingAway)
{
    Sphere obj{Vec3(0, 0, 0), 10.00f, Material()};
    Ray ray{{20.0, 0.0, 0.0}, {1.0, 0.0, 0.0}};

    Intersection intersection;
    bool intersectionOccured = obj.intersect(ray, intersection);

    EXPECT_FALSE(intersectionOccured);
}

