#include <sstream>

#include "G4SystemOfUnits.hh"

#include "Phantom.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4RotationMatrix.hh"

#include "G4NistManager.hh"

#include "G4PVParameterised.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"

#include "Parameterisation.hh"
#include "G4ios.hh"  

#include "G4VisAttributes.hh"
#include "G4UserLimits.hh"

#include "G4UniformMagField.hh"
#include "G4GeometryManager.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

#include "G4VPhysicalVolume.hh"

#include "MagneticField.hh"

#include "Randomize.hh"

Phantom::Phantom(const G4String&     name,
                 const PhantomSetup& phs):
    _name(name),
    _phs(phs),

    _world_box(0),
    _world_log(0),
    _world_phys(0),

    _water_box(0),
    _water_box_log(0),
    _water_box_phys(0),

    _voxel_box(0),
    _voxel_box_log(0),

    _mat_IDs(0),

    _magField(0)
{
#ifdef USE_MAG_FIELD
    _magField = new MagneticField();
#endif
}

Phantom::~Phantom()
{ 
    delete _world_log;
    delete _world_phys;
#ifdef USE_MAG_FIELD
    delete _magField;
#endif
}

void Phantom::construct_materials()
{
    _materials.clear();
    G4Material* acetone = G4NistManager::Instance()->FindOrBuildMaterial("G4_ACETONE");
    G4Material* matH2O = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
    G4Material* air    = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

    double at_number = 1.0;
    double mol_mass  = 1.008*g/mole;
    double vdensity  = 1.0e-25*g/cm3;
    double temperat  = 2.73*kelvin;
    double pressure  = 3.0e-18*pascal;
    G4Material* vacuum = new G4Material("Vacuum", at_number, mol_mass, vdensity,
                                        kStateGas, temperat, pressure);
    at_number = 1.0;
    mol_mass  = 2.01588*g/mole;
    vdensity  = 0.001225*g/cm3;
    temperat  = 293.0*kelvin;
    pressure  = 101325.0*pascal;
    G4Material* h2 = new G4Material("H2", at_number, mol_mass, vdensity,
                                    kStateGas, temperat, pressure);
    // Bubble
    double a, z, d;
    int natoms;
    a = 1.01*g/mole;
    G4Element* elH = new G4Element ("Hydrogen", "H", z=1., a);

    a = 16.00*g/mole;
    G4Element* elO = new G4Element("Oxygen","O", z=8., a);

    d = 0.010*g/cm3;
    G4Material* bubble = new G4Material("Bubble", d, 2);
    bubble->AddElement(elH, natoms=2);
    bubble->AddElement(elO, natoms=1);
    bubble->GetIonisation()->SetMeanExcitationEnergy(75.1*eV);

    bool isotopes = false;
    G4Material* plexiglass = G4NistManager::Instance()->FindOrBuildMaterial("G4_PLEXIGLASS", isotopes);    

    _materials.push_back(matH2O);
    _materials.push_back(air);
    _materials.push_back(vacuum);
    _materials.push_back(bubble);
    _materials.push_back(plexiglass);
    _materials.push_back(acetone);
    _materials.push_back(h2);
}

void Phantom::construct_mat_IDs()
{
    _mat_IDs = new size_t [_phs.nof_voxels()];

    // fill everything with water first
    for(int k = 0; k != _phs.nof_voxels(); ++k)
    {
        _mat_IDs[k] = MAT_WATER;        
    }

    // now fill wall with bubbles
    for(int iz = 0; iz != _phs.split_Bub(); ++iz)
    {
        for(int iy = 0; iy != _phs.nofv_y(); ++iy)
        {
            for(int ix = 0; ix != _phs.nofv_x(); ++ix)
            {
                int k = _phs.idx(ix, iy, iz);
                if (G4UniformRand() < _phs.fill_rate())
                    _mat_IDs[k] = MAT_AIR;
            }
        }
    }

    // now air gap
    for(int iz = _phs.split_Bub(); iz != _phs.split_Air(); ++iz)
    {
        for(int iy = 0; iy != _phs.nofv_y(); ++iy)
        {
            for(int ix = 0; ix != _phs.nofv_x(); ++ix)
            {
                int k = _phs.idx(ix, iy, iz);
                _mat_IDs[k] = MAT_VACUUM;           
            }
        }
    }

    // now patient phantom
    for(int iz = _phs.split_Air(); iz != _phs.nofv_z(); ++iz)
    {
        for(int iy = 0; iy != _phs.nofv_y(); ++iy)
        {
            for(int ix = 0; ix != _phs.nofv_x(); ++ix)
            {
                int k = _phs.idx(ix, iy, iz);
                _mat_IDs[k] = MAT_WATER;           
            }
        }
    }
}

void Phantom::construct_water_box()
{
    //  world
    _world_box  = new G4Box("WorldBox", 100.*cm, 100.*cm, 100.*cm);
    _world_log  = new G4LogicalVolume(_world_box, _materials[MAT_VACUUM], "WorldLog", 0, 0, 0);
    _world_phys = new G4PVPlacement(0, G4ThreeVector(), _world_log, "WorldPhys", 0, false, 0);
    
    // whole phantom
    _water_box      = new G4Box("WaterBox", _phs.cube_x()/2., _phs.cube_y()/2., _phs.cube_z()/2.);
    _water_box_log  = new G4LogicalVolume(_water_box, _materials[MAT_WATER], "WaterBoxLog", 0, 0, 0);
    _water_box_phys = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
                                        _water_box_log, "WaterBoxPhys",
                                        _world_log, false, 1);    

    // _water_box_log->SetVisAttributes(new G4VisAttributes(G4Colour(1.0, 1.0, 0.0)));
}

G4VPhysicalVolume* Phantom::Construct()
{
    construct_materials();
    construct_mat_IDs();   

    // Volumes
    construct_water_box();

    Parameterisation* param = new Parameterisation(_phs);

    param->SetVoxelDimensions( _phs.voxel_x()/2.0, _phs.voxel_y()/2.0, _phs.voxel_z()/2.0 );

    param->SetNoVoxel( _phs.nofv_x(), _phs.nofv_y(), _phs.nofv_z() );

    param->SetMaterials( _materials ); 

    param->SetMaterialIndices( _mat_IDs );

    _voxel_box      = new G4Box("VoxelBox", _phs.voxel_x()/2., _phs.voxel_y()/2., _phs.voxel_z()/2.);
    _voxel_box_log  = new G4LogicalVolume(_voxel_box, _materials[MAT_WATER], "VoxelBoxLog", 0, 0, 0);

    //--- Assign the fContainer volume of the parameterisation
    param->BuildContainerSolid(_water_box_phys);

    //--- Assure yourself that the voxels are completely filling the fContainer volume
    param->CheckVoxelsFillContainer( _water_box->GetXHalfLength(), 
                                     _water_box->GetYHalfLength(), 
                                     _water_box->GetZHalfLength() );

    G4PVParameterised* voxelboxPhys = new G4PVParameterised("Phantom",
                                                            _voxel_box_log,
                                                            _water_box_log,
                                                            kUndefined,
                                                            _phs.nof_voxels(),
                                                            param);

    // Set this physical volume as having a regular structure of type 1,
    // so that G4RegularNavigation is used
    // voxelboxPhys->SetRegularStructureId(0); // if not set, G4VoxelNavigation will be used instead     

    return _world_phys;
}

double Phantom::density(int vidx) const
{
    int mid = _mat_IDs[vidx];
    return _materials[mid]->GetDensity();
}

void Phantom::SetMagField(double field_x, double field_y, double field_z)
{
    if (field_x == 0.0 && field_y == 0.0 && field_z == 0.0)
    {
        return;
    }
#ifdef USE_MAG_FIELD
    _magField->SetMagField(G4ThreeVector(field_x, field_y, field_z));
#endif
}
