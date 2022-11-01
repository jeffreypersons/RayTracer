#include "LightIncidence.hpp"

#include "gtest/gtest.h"


TEST(Reflect, OppositeDirectionNormal)
{
    Ray ray{{1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}};
    Intersection intersection{{0.0, 0.0, 0.0}, Math::normalize(Vec3(1.0, 1.0, 0.0)), 0.0, NULL};

    LightIncidenceFresnel lightIncidence{ray, intersection, 1.0f, 1.0f};
    Vec3 reflected_direction = lightIncidence.reflectedDirection();

    EXPECT_NEAR(reflected_direction.x, 0.0f, 0.1f);
    EXPECT_NEAR(reflected_direction.y, 1.0f, 0.1f);
    EXPECT_NEAR(reflected_direction.z, 0.0f, 0.1f);
}

TEST(Reflect, SameDirectionNormal)
{
    Ray ray{{1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}};
    Intersection intersection{{0.0, 0.0, 0.0}, Math::normalize(Vec3(-1.0, -1.0, 0.0)), 0.0, NULL};

    LightIncidenceFresnel lightIncidence{ray, intersection, 1.0f, 1.0f};
    Vec3 reflected_direction = lightIncidence.reflectedDirection();

    EXPECT_NEAR(reflected_direction.x, 0.0f, 0.1f);
    EXPECT_NEAR(reflected_direction.y, 1.0f, 0.1f);
    EXPECT_NEAR(reflected_direction.z, 0.0f, 0.1f);
}

