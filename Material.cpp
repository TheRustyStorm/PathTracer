//
// Created by Peter Zdankin on 26.03.18.
//

#include <cmath>
#include "Material.hpp"

double Material::getExponent() const{
    return exponent;
}
bool Material::isShadowCaster() const{
    return shadowCaster;
}
bool Material::reflects() const{
    return local < 1.0;
}
bool Material::refracts() const{
    return local < 1.0 && IOR > 0.0;
}
double Material::getLocalReflectivity() const{
    return local;
}
double Material::getIndexOfRefraction() const{
    return IOR;
}
const Point& Material::getAmbient() const{
    return ambient;
}
const Point& Material::getDiffuse() const{
    return diffuse;
}
const Point& Material::getSpecular() const{
    return specular;
}

double Material::getReflectivity(double cosI) const {

        double R0 = 1 - local;

        int sign = (cosI < 0) ? -1 : 1;

        if (IOR != 0.0) {
            double n = (sign == 1) ? IOR : 1.0 / IOR;
            double R0sqrt = (n - 1) / (n + 1);
            R0 = R0sqrt * R0sqrt;
        }

        return R0 + (1 - R0) * pow(1 - sign * cosI, 5);
}

const Point& Material::getEmission() const {
    return emission;
}

bool Material::isEmitting() const {
    return emits;
}

MaterialType Material::getMaterialType() const {
    return type;
}
