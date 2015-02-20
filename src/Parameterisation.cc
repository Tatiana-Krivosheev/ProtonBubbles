#include "globals.hh"
#include "Parameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"


Parameterisation::Parameterisation(const PhantomSetup& phs):
        _phs(phs)
{
}

Parameterisation::~Parameterisation()
{
}

/*
void Parameterisation::ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const
{
    int iz = int((copyNo/(_phs.nofv_x()*_phs.nofv_y())));
    int iy = int(((copyNo-(iz*_phs.nofv_x()*_phs.nofv_y()))/_phs.nofv_x()));
    int ix = int(copyNo - iy*_phs.nofv_x() - iz*_phs.nofv_x()*_phs.nofv_y());

    double zkoord = _phs.voxel_z()/2. + double(iz)*_phs.voxel_z() - double(_phs.nofv_z())*_phs.voxel_z()/2.;
    double ykoord = _phs.voxel_y()/2. + double(iy)*_phs.voxel_y() - double(_phs.nofv_y())*_phs.voxel_y()/2.;
    double xkoord = _phs.voxel_x()/2. + double(ix)*_phs.voxel_x() - double(_phs.nofv_x())*_phs.voxel_x()/2.;

    G4ThreeVector origin(xkoord, ykoord, zkoord);
    physVol->SetTranslation(origin);
    physVol->SetRotation(0);
}
*/

G4Material* Parameterisation::ComputeMaterial(const int copyNo,
                                              G4VPhysicalVolume* physVol,
                                              const G4VTouchable*)
{
    G4Material* mate = G4PhantomParameterisation::ComputeMaterial( copyNo, physVol, 0 );
    return mate;
}
