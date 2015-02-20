#include "Source.hh"
#include "Spectrum.hh"
#include "SourceMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"

#include "Randomize.hh"

// here you set global source parameters, called once per run
Source::Source(double cube_x, double cube_y, double cube_z,
               double theSID,
               double the_momentum, double the_momentumSigma,
               double the_radius, double the_radiusSigma,
               Spectrum* spectrum):
    _particleGun(0),
    _srcMessenger(0),
    _spectrum(spectrum),
    _hcube_x(cube_x/2.),
    _hcube_y(cube_y/2.),
    _hcube_z(cube_z/2.),
    _SID(theSID),
    _momentum(the_momentum),
    _momentumSigma(the_momentumSigma),
    _radius(the_radius),
    _radiusSigma(the_radiusSigma)
{
    int nof_particles = 1;
    _particleGun = new G4ParticleGun(nof_particles);

    //create a messenger for this class
    _srcMessenger = new SourceMessenger(this);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "proton"; // "e-"
    _particleGun->SetParticleDefinition(particleTable->FindParticle(particleName));

    _particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    _particleGun->SetParticleEnergy(_momentum);
}

Source::~Source()
{
    delete _particleGun;
    delete _srcMessenger;
}

static inline double squared(double x)
{
    return x*x;
}

troika Source::sample_direction(double x0, double y0, double z0) const
{
    double x = 0.0; // -_half_field_x + 2.0*_half_field_x*G4UniformRand();
    double y = 0.0; // -_half_field_y + 2.0*_half_field_y*G4UniformRand();
    double z = 0.0;

    double len = sqrt( squared(x - x0) + squared(y - y0) + squared(z - z0) );

    troika dir;

    dir._wx = (x - x0) / len;
    dir._wy = (y - y0) / len;
    dir._wz = (z - z0) / len;

    return dir;
}

// source particle parameters, called per each source event
void Source::GeneratePrimaries(G4Event* anEvent)
{
    double Esigma = -1.0;
    double E0     = _momentum;
    do
    {
        Esigma = G4RandGauss::shoot(E0, _momentumSigma);
    }
    while ((Esigma<=0.1)||(Esigma>1500.));

    double z0 = -_SID;
    double x0 = -1.0, y0 = -1.0;
    do
    {
        x0 = G4RandGauss::shoot(0.0, _radiusSigma);
        y0 = G4RandGauss::shoot(0.0, _radiusSigma);
    }
    while ((x0*x0+y0*y0) > (_radius*_radius));

    _particleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

    _particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

    _particleGun->SetParticleEnergy(Esigma);
    _particleGun->GeneratePrimaryVertex(anEvent);
}
