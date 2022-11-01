#include "LightIncidence.hpp"
#include "Ray.hpp"
#include "Math.hpp"

#include <optional>

LightIncidenceFresnel::LightIncidenceFresnel(const Ray& ray, const Intersection& intersection, float indexOfRefractionFrom, float indexOfRefractionTo) :
    ray_(ray),
    intersection_(intersection),
    n1_(indexOfRefractionFrom),
    n2_(indexOfRefractionTo)
{
    computeHelperVars();
}

void LightIncidenceFresnel::computeHelperVars()
{
    n1_over_n2_ = n1_ / n2_;

    cos_theta_i_         = -Math::dot(ray_.direction, intersection_.normal);
    cos_theta_i_squared_ = Math::square(cos_theta_i_);
    sin_theta_i_squared_ = 1.0f - cos_theta_i_squared_;

    if (Math::square(n1_over_n2_) * sin_theta_i_squared_ > 1.0f) {
        reflectedProportion_ = 1.0f;
        transmittedProportion_ = 0.0f;
        return;
    }

    sin_theta_t_squared_ = Math::square(n1_over_n2_)*sin_theta_i_squared_;
    cos_theta_t_squared_ = 1.0f - sin_theta_t_squared_;
    cos_theta_t_         = Math::squareRoot(cos_theta_t_squared_);

    R_normal_  = Math::square((n1_ * cos_theta_i_ - n2_ * cos_theta_t_) / (n1_ * cos_theta_i_ + n2_ * cos_theta_t_));
    R_tangent_ = Math::square((n2_ * cos_theta_i_ - n1_ * cos_theta_t_) / (n2_ * cos_theta_i_ + n1_ * cos_theta_t_));

    reflectedProportion_ = (R_normal_ + R_tangent_) / 2.0f;
    transmittedProportion_ = 1.0f - reflectedProportion_;
}

Vec3 LightIncidenceFresnel::reflectedDirection() const
{
    return ray_.direction + 2.0f * cos_theta_i_ * intersection_.normal;
}

Vec3 LightIncidenceFresnel::refractedDirection() const
{
    return n1_over_n2_ * ray_.direction + (n1_over_n2_ * cos_theta_i_ - Math::squareRoot(1.0f - sin_theta_t_squared_)) * intersection_.normal;
}

float LightIncidenceFresnel::reflectedProportion() const
{
    return reflectedProportion_;
}

float LightIncidenceFresnel::transmittedProportion() const
{
    return transmittedProportion_;
}


LightIncidenceReflect::LightIncidenceReflect(const Ray &ray, const Intersection &intersection) :
    ray_(ray),
    intersection_(intersection)
{
}

Vec3 LightIncidenceReflect::reflectedDirection() const {
    return ray_.direction + 2.0f * -Math::dot(ray_.direction, intersection_.normal) * intersection_.normal;
}
