#include <iostream>

#include "MagneticField.hh"

#ifdef USE_MAG_FIELD

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

MagneticField::MagneticField():
    G4UniformMagField(G4ThreeVector())
{
    GetGlobalFieldManager()->SetDetectorField(NULL);
}

MagneticField::MagneticField(const G4ThreeVector& fieldVector):
    G4UniformMagField(fieldVector)
{
    GetGlobalFieldManager()->SetDetectorField(this);    
    GetGlobalFieldManager()->CreateChordFinder(this);
}

MagneticField::~MagneticField()
{
}

void MagneticField::SetMagField(const G4ThreeVector& fieldVector)
{
    std::cout << "Setting magnetic field ...\n"; 
    if (fieldVector != G4ThreeVector(0.,0.,0.))
    {
        SetFieldValue(fieldVector);
        GetGlobalFieldManager()->SetDetectorField(this);
        GetGlobalFieldManager()->CreateChordFinder(this);
    }
    else
    {
        GetGlobalFieldManager()->SetDetectorField(NULL);
    }
}

G4FieldManager* MagneticField::GetGlobalFieldManager()
{
    return G4TransportationManager::GetTransportationManager()->GetFieldManager();
}

#endif // USE_MAG_FIELD