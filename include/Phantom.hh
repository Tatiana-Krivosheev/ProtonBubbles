#pragma once

#include <vector>

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class MagneticField;

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "PhantomSetup.hh"

class Phantom : public G4VUserDetectorConstruction
{
#pragma region Constants
    public: const int MAT_WATER  = 0;
    public: const int MAT_AIR    = 1;
    public: const int MAT_VACUUM = 2;
    public: const int MAT_BUBBLE = 3;    
    public: const int MAT_PLEX   = 4;    
    public: const int MAT_ACET   = 5;
    public: const int MAT_H2     = 6;
#pragma endregion

#pragma region Data
    private: G4String           _name;

    private: PhantomSetup       _phs;

    private: G4Box*             _world_box;
    private: G4LogicalVolume*   _world_log;
    private: G4VPhysicalVolume* _world_phys;

    private: G4Box*             _water_box;
    private: G4LogicalVolume*   _water_box_log;
    private: G4VPhysicalVolume* _water_box_phys; 

    private: G4Box*             _voxel_box;
    private: G4LogicalVolume*   _voxel_box_log;

    std::vector<G4Material*>    _materials;
    size_t*                     _mat_IDs;

    private: MagneticField*     _magField;
#pragma endregion

#pragma region Ctor/Dtor/ops
    public: Phantom(const G4String& name, const PhantomSetup& phs);
    public: virtual ~Phantom();
#pragma endregion

#pragma region Overrides
    public: virtual G4VPhysicalVolume* Construct();
#pragma endregion

#pragma region Helpers
    private: void construct_materials();
    private: void construct_mat_IDs();
    private: void construct_water_box();
#pragma endregion

#pragma region Observers
    public: double cube_x() const
    {
        return _phs.cube_x();
    }

    public: double cube_y() const
    {
        return _phs.cube_y();
    }

    public: double cube_z() const
    {
        return _phs.cube_z();
    }
 
    public: int nofv_x() const
    {
        return _phs.nofv_x();
    }

    public: int nofv_y() const
    {
        return _phs.nofv_y();
    }

    public: int nofv_z() const
    {
        return _phs.nofv_z();
    }

    public: int nof_voxels() const
    {
        return _phs.nof_voxels();
    }

    public: double voxel_x() const
    {
        return _phs.voxel_x();
    }

    public: double voxel_y() const
    {
        return _phs.voxel_y();
    }

    public: double voxel_z() const
    {
        return _phs.voxel_z();
    }

    public: double voxel_volume() const
    {
        return _phs.voxel_volume();
    }

    public: double density(int vidx) const;

    public: int idx(int ix, int iy, int iz) const
    {
        return _phs.idx(ix, iy, iz);
    }

    public: G4LogicalVolume* voxel_box_log() const
    {
        return _voxel_box_log;
    }

    public: const std::vector<G4Material*>& materials() const
    {
        return _materials;
    } 

    public: const size_t* mat_IDs() const
    {
        return _mat_IDs;
    } 
#pragma endregion

    public: void SetMagField(double field_x, double field_y, double field_z);
};
