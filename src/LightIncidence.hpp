#include "Math.hpp"
#include "Ray.hpp"

#include <optional>

class LightIncidenceFresnel {
public:
    LightIncidenceFresnel(const Ray &ray, const Intersection &intersection,
                          const float indexOfRefractionFrom,
                          const float indexOfRefractionTo);

    Vec3 reflectedDirection() const;
    Vec3 refractedDirection() const;
    float reflectedProportion() const;
    float transmittedProportion() const;

private:
    void computeHelperVars();

    const Ray& ray_;
    const Intersection& intersection_;
    const float n1_;
    const float n2_;

    bool  total_internal_reflection_;
    float n1_over_n2_;
    float cos_theta_i_;
    float cos_theta_i_squared_;
    float sin_theta_i_squared_;
    float sin_theta_t_squared_;
    float cos_theta_t_squared_;
    float cos_theta_t_;

    float R_normal_;
    float R_tangent_;
    float reflectedProportion_;
    float transmittedProportion_;
};

class LightIncidenceReflect {
public:
    LightIncidenceReflect(const Ray &ray, const Intersection &intersection);

    Vec3 reflectedDirection() const;

private:
    const Ray &ray_;
    const Intersection &intersection_;
};

