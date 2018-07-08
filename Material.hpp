//
// Created by Peter Zdankin on 26.03.18.
//

#ifndef PATHTRACER_MATERIAL_HPP
#define PATHTRACER_MATERIAL_HPP

#include "Point.hpp"

enum MaterialType {
    DIFFUSE, REFLECTING, REFRACTING
};

class Material {
private:
    const Point ambient;
    const Point diffuse;
    const Point specular;
    const Point emission;
    bool emits;
    const double exponent;
    const double local;
    const double IOR;
    const bool shadowCaster;
    MaterialType type;
public:
    Material(const Point &_ambient, const Point &_diffuse, const Point &_specular, const Point &_emission,
             double _exponent) :
            ambient(_ambient), diffuse(_diffuse), specular(_specular), emission(_emission), exponent(_exponent),
            local(1), IOR(0), shadowCaster(true) {
        emits = (_emission - Point()).length() > 0;
        type = DIFFUSE;
    }

    Material(const Point &_ambient, const Point &_diffuse, const Point &_specular, const Point &_emission,
             double _exponent, double _local) :
            ambient(_ambient), diffuse(_diffuse), specular(_specular), emission(_emission), exponent(_exponent),
            local(_local), IOR(0), shadowCaster(true) {
        emits = (_emission - Point()).length() > 0;
        type = local < 1 ? REFLECTING : DIFFUSE;
    }

    Material(const Point &_ambient, const Point &_diffuse, const Point &_specular, const Point &_emission,
             double _exponent, double _local, double _IOR) :
            ambient(_ambient), diffuse(_diffuse), specular(_specular), emission(_emission), exponent(_exponent),
            local(_local), IOR(_IOR), shadowCaster(true) { emits = (_emission - Point()).length() > 0;
        if(local < 1 && IOR != 1){
            type = REFRACTING;
        }else if(local == 1 && IOR == 0){
            type = DIFFUSE;
        }
        else{
            type = REFLECTING;
        }
    }

    Material(const Point &_ambient, const Point &_diffuse, const Point &_specular, const Point &_emission,
             double _exponent, double _local, bool _isShadowCaster) :
            ambient(_ambient), diffuse(_diffuse), specular(_specular), emission(_emission), exponent(_exponent),
            local(_local), IOR(0), shadowCaster(_isShadowCaster) { emits = (_emission - Point()).length() > 0;
        if(local < 1 && IOR != 1){
            type = REFRACTING;
        }else if(local == 1 && IOR == 0){
            type = DIFFUSE;
        }
        else{
            type = REFLECTING;
        }}

    double getExponent() const;

    bool isShadowCaster() const;

    bool reflects() const;

    bool refracts() const;

    double getLocalReflectivity() const;

    double getIndexOfRefraction() const;

    const Point &getAmbient() const;

    const Point &getDiffuse() const;

    const Point &getSpecular() const;

    const Point &getEmission() const;

    bool isEmitting() const;

    double getReflectivity(double cosI) const;

    MaterialType getMaterialType() const;

};

#endif //PATHTRACER_MATERIAL_HPP
