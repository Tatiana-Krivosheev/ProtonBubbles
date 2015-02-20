#pragma once

#define USE_MAG_FIELD

#ifdef USE_MAG_FIELD 

#include "G4UniformMagField.hh"

class G4FieldManager;

///  A class for control of the Magnetic Field of the detector.
///  The field is assumed to be uniform. 
class MagneticField : public G4UniformMagField
{
#pragma region Ctor/Dtor/ops
    public: MagneticField();                     //  no field
    public: MagneticField(const G4ThreeVector&); //  the value of the field
    public: virtual ~MagneticField();
#pragma endregion  

    public: void SetMagField(const G4ThreeVector& fieldVector);
      
    public: static G4FieldManager* GetGlobalFieldManager();
};

#endif // USE_MAG_FIELD 
