#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <stdexcept>

#include "G4SystemOfUnits.hh"

#include "PhantomSetup.hh"

PhantomSetup::PhantomSetup(const char* hed_name,
                           int split_Bub, int split_Air,
                           float fill_rate):
    _nofv_x(-1),
    _nofv_y(-1),
    _nofv_z(-1),

    _voxel_x(-1.0f),
    _voxel_y(-1.0f),
    _voxel_z(-1.0f),

    _cube_x(-1.0f),
    _cube_y(-1.0f),
    _cube_z(-1.0f),

    _split_Bub(split_Bub),
    _split_Air(split_Air),
    _fill_rate(fill_rate)
{
    std::cout << "Reading file:" << hed_name << std::endl;
    std::ifstream hed_file(hed_name, std::ios::in);
    if (!hed_file)
    {
        std::string msg("Cannot open phantom header file: ");
        throw std::logic_error(msg + hed_name);
    }

    char keyword[17], thebar;
    bool read_cards = true;
 
    while (read_cards)
    {
        hed_file >> keyword;

        if (!strcmp(keyword,"END-INPUT"))
        {
            read_cards = false;
        }
        else
        {
            if (!strcmp(keyword,"VOXELSIZE"))
            {
                hed_file >> thebar >> _voxel_x >> _voxel_y >> _voxel_z;
            }
            if (!strcmp(keyword,"DIMENSION"))
            {
                hed_file >> thebar >> _nofv_x >> _nofv_y >> _nofv_z;
            }
        }
    }

    // close header file
    hed_file.close();

    if (_nofv_x > 0 && _nofv_y > 0 && _nofv_z > 0)
    {
        if (_voxel_x > 0.0f && _voxel_y > 0.0f && _voxel_z > 0.0f)
        {
            _voxel_x = _voxel_x*cm;
            _voxel_y = _voxel_y*cm;
            _voxel_z = _voxel_z*cm;

            _cube_x = _voxel_x * float(_nofv_x);
            _cube_y = _voxel_y * float(_nofv_y);
            _cube_z = _voxel_z * float(_nofv_z);
        }
        else
        {
            throw std::logic_error("Problem with setting up phantom");
        }
    }

    bool inv = (_split_Bub >= 0) && (_split_Air >= 0) && (_split_Bub <= _split_Air) &&
               (_split_Bub <= _nofv_z) && (_split_Air <= _nofv_z);

    if (!inv)
        throw std::logic_error("Problem with setting up phantom splitting");

    if (_fill_rate < 0.0f || _fill_rate > 1.0f)
        throw std::logic_error("Problem with setting up phantom fill rate");            
}
