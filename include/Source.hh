#pragma once

#include <cassert>

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include "troika.hh"

class G4ParticleGun;
class G4Event;
class SourceMessenger;
class Spectrum;
class Phantom;

class Source : public G4VUserPrimaryGeneratorAction
{
#pragma region Data
    private: G4ParticleGun*             _particleGun;
    private: SourceMessenger*           _srcMessenger;
    private: Spectrum*                  _spectrum;

    private: double _hcube_x;
    private: double _hcube_y;
    private: double _hcube_z;

    private: double _SID; // source-to-isocenter distance

    private: double _momentum;
    private: double _momentumSigma;

    private: double _radius;
    private: double _radiusSigma;
#pragma endregion

#pragma region Ctor/Dtor/ops
    public: Source(double cube_x, double cube_y, double cube_z,
                   double SID,
                   double momentum, double momentumSigma,
                   double radius, double radiusSigma,
                   Spectrum* spectrum = 0);
    public: ~Source();
#pragma endregion

#pragma region Mutators
    protected: troika sample_direction(double x0, double y0, double z0) const;

    public: void GeneratePrimaries(G4Event* anEvent);

    public: void set_momentum(double val)
    {
        _momentum = val;
    }

    public: void set_momentumSigma(double val)
    {
        _momentumSigma = val;
    }

    public: void set_radius(double val)
    {
        _radius = val;
    }

    public: void set_radiusSigma(double val)
    {
        _radiusSigma = val;
    }    
#pragma endregion

#pragma region Observers
    public: double SID() const
    {
        return _SID;
    }

    public: double momentum() const
    {
        return _momentum;
    }

    public: double momentumSigma() const
    {
        return _momentumSigma;
    }

    public: double radius() const
    {
        return _radius;
    }

    public: double radiusSigma() const
    {
        return _radiusSigma;
    }
#pragma endregion
};
