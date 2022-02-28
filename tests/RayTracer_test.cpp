#include "Material.hpp"
#include "Color.hpp"
#include "Objects.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "RayTracer.hpp"

#include "gtest/gtest.h"

#include <iostream>

TEST(Reflect, SameDirectionNormal)
{
    RayTracer ray_tracer;
    Ray ray{{1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}};
    Intersection intersection{{0.0, 0.0, 0.0}, Math::normalize(Vec3(-1.0, -1.0, 0.0)), 0.0, NULL};

    Ray reflected_ray = ray_tracer.reflectRay(ray, intersection);

    EXPECT_NEAR(reflected_ray.direction.x, 0.0f, 0.1f);
    EXPECT_NEAR(reflected_ray.direction.y, 1.0f, 0.1f);
    EXPECT_NEAR(reflected_ray.direction.z, 0.0f, 0.1f);
}

TEST(Reflect, OppositeDirectionNormal)
{
    RayTracer ray_tracer;
    Ray ray{{1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}};
    Intersection intersection{{0.0, 0.0, 0.0}, Math::normalize(Vec3(1.0, 1.0, 0.0)), 0.0, NULL};

    Ray reflected_ray = ray_tracer.reflectRay(ray, intersection);

    EXPECT_NEAR(reflected_ray.direction.x, 0.0f, 0.1f);
    EXPECT_NEAR(reflected_ray.direction.y, 1.0f, 0.1f);
    EXPECT_NEAR(reflected_ray.direction.z, 0.0f, 0.1f);
}

TEST(NearestIntersection, Simple)
{
    Scene scene{};
    Camera camera{};

    scene.addSceneObject(Sphere(Vec3(0.0f, 0.0f, 10.0f), 5.00f, Material()));
    scene.addSceneObject(Sphere(Vec3(0.0f, 0.0f, 30.0f), 5.00f, Material()));

    Ray ray{{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}};

    Intersection intersection;
    RayTracer ray_tracer;

    ray_tracer.findNearestIntersection(camera, scene, ray, intersection);

    EXPECT_NEAR(intersection.point.x, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.point.y, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.point.z, 5.0f, 0.1f);
}

TEST(NearestIntersection, Inside)
{
    Scene scene{};
    Camera camera{};
    
    scene.addSceneObject(Sphere(Vec3(0.0f, 0.0f, 0.0f), 5.00f, Material()));

    Ray ray{{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}};

    Intersection intersection;
    RayTracer ray_tracer;

    ray_tracer.findNearestIntersection(camera, scene, ray, intersection);

    EXPECT_NEAR(intersection.point.x, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.point.y, 0.0f, 0.1f);
    EXPECT_NEAR(intersection.point.z, 5.0f, 0.1f);
}
