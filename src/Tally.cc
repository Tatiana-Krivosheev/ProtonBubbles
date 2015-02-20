#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>

#include "G4SystemOfUnits.hh"

#include "Tally.hh"
#include "Phantom.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"

#include "Randomize.hh"

static inline double squared(double x)
{
    return x*x;
}

Tally::Tally( Phantom* phantom ):
    _phantom(phantom),
    _edep(0),
    _eerr(0),
    _maxTally(-1),
    _tot_edep(0.0),
    _tot_eerr(0.0)
{
    _maxTally = phantom->nof_voxels();

    _edep = new double [_maxTally];
    _eerr = new double [_maxTally];
}

Tally::~Tally()
{
    delete [] _edep;
    delete [] _eerr;
}

void Tally::BeginOfRunAction(const G4Run* /* aRun */)
{  
    for(int k = 0; k != _maxTally; ++k)
    {
        _edep[k] = 0.0;
    }

    for(int k = 0; k != _maxTally; ++k)
    {
        _eerr[k] = 0.0;
    }

    _tot_edep = 0.0;
    _tot_eerr = 0.0;    
#ifdef G4VIS_USE
    if (G4VVisManager::GetConcreteInstance())
    {
        G4UImanager::GetUIpointer()->ApplyCommand("/vis/scene/notifyHandlers");
    }
#endif
}

G4LogicalVolume* Tally::voxel_box_log() const
{
    return _phantom->voxel_box_log();
}

void Tally::postprocess(int nof_events, double mult) const
{
    if (nof_events <= 0)
        return;

    double norm = mult / double(nof_events);

    double vv = _phantom->voxel_volume(); // should be in cc or cm3

    for(int k = 0; k != _maxTally; ++k)
    {
        _eerr[k] = sqrt( std::max( _eerr[k] - squared(_edep[k]), 0.0 ) );

        double dens = _phantom->density(k);
        double mass = dens*vv;

        _edep[k] *= ( norm / mass );
        _eerr[k] *= ( norm / mass );
    }

    // energy deposited per source particle
    _tot_edep /= double(nof_events);
    _tot_eerr /= double(nof_events);
}

void Tally::write_axis(std::ostream& os, int nv, float start, float voxel) const
{
    for(int k = 0; k != nv; ++k)
    {
        float pos = start + float(k)*voxel;
        os << pos << " ";
    }
    os << std::endl;
}

void Tally::write_out_file(const char* fname) const
{
    std::string filename(fname);
    filename += ".out";

    std::ofstream dose_out(filename.c_str(), std::ios_base::out);

    if (!dose_out)
        return; // exception?

    dose_out << _phantom->nofv_x() << " " << _phantom->nofv_y() << " " << _phantom->nofv_z() << std::endl;

    // printing one line of X
    write_axis(dose_out, _phantom->nofv_x(), -0.5f * _phantom->cube_x() + 0.5f * _phantom->voxel_x(), _phantom->voxel_x());   

    // printing one line of Y
    write_axis(dose_out, _phantom->nofv_y(), -0.5f * _phantom->cube_y() + 0.5f * _phantom->voxel_y(), _phantom->voxel_y());   

    // printing one line of Y
    write_axis(dose_out, _phantom->nofv_z(), -0.5f * _phantom->cube_z() + 0.5f * _phantom->voxel_z(), _phantom->voxel_z());

    // writing single dose column

    dose_out << std::scientific;
    for (int iz = 0; iz != _phantom->nofv_z(); ++iz)
    {
        for (int iy = 0; iy != _phantom->nofv_y(); ++iy)
        {
            for (int ix = 0; ix != _phantom->nofv_x(); ++ix)
            {
                int    idx  = _phantom->idx(ix, iy, iz);    
                double dose = _edep[idx];

                dose_out << std::setw(15) << std::setprecision(4) << dose << std::endl;
            }
        }
    }

    // writing single dose error column
    for (int iz = 0; iz != _phantom->nofv_z(); ++iz)
    {
        for (int iy = 0; iy != _phantom->nofv_y(); ++iy)
        {
            for (int ix = 0; ix != _phantom->nofv_x(); ++ix)
            {
                int idx      = _phantom->idx(ix, iy, iz);
                double error = _eerr[idx];

                dose_out << std::setw(15) << std::setprecision(4) << error << std::endl;
            }
        }
    }

    dose_out << "C:/some ugly file path\n";

    dose_out.close();    
}

void Tally::write_hed_file(const char* fname, int& mx, int& my, int& mz, double& dmax) const
{
    std::string filename(fname);
    filename += ".hed";

    std::ofstream dose_head( filename.c_str(), std::ios_base::out );
    if (!dose_head)
        return;

    int koordx = 0;
    int koordy = 0;
    int koordz = 0;

    double dosemaximum = -0.0;

    for (int iz = 0; iz != _phantom->nofv_z(); ++iz)
    {
        for (int iy = 0; iy != _phantom->nofv_y(); ++iy)
        {
            for (int ix = 0; ix != _phantom->nofv_x(); ++ix)
            {
                int idx = _phantom->idx(ix, iy, iz);    
                double dose = _edep[idx];
    
                if (dose > dosemaximum) 
                {
                    dosemaximum = dose;
                    koordx = ix;
                    koordy = iy;
                    koordz = iz;
                }
            }
        }
    } 

    dose_head << "VOXELSIZE      |"<<'\t'<<float(_phantom->voxel_x()/cm)<<'\t'<<float(_phantom->voxel_y()/cm)<<'\t'<<float(_phantom->voxel_z()/cm)<<G4endl;
    dose_head << "DIMENSION      |"<<'\t'<<int(_phantom->nofv_x())<<'\t'<<int(_phantom->nofv_y())<<'\t'<<int(_phantom->nofv_z())<<std::endl;
    dose_head << "DOSE-MAXIMUM   |"<<'\t'<<dosemaximum<< std::endl;
    dose_head << "CPU-TIME       |       0.000000"<< std::endl;
    dose_head << "POS-MAXIMUM    |"<<'\t'<<koordx<<'\t'<<koordy<<'\t'<<koordz<< std::endl;
    dose_head << "END-INPUT      |"<< std::endl;

    dose_head.close();

    mx = koordx;
    my = koordy;
    mz = koordz;

    dmax = dosemaximum;
}

void Tally::write_d3dose_file(const char* fname) const
{
    std::string filename(fname);
    filename += ".d3d";

    FILE* dmx_file = fopen(filename.c_str(), "wb");
    if (!dmx_file)
        return;

    for (int iz = 0; iz != _phantom->nofv_z(); ++iz)
    {
        for (int iy = 0; iy != _phantom->nofv_y(); ++iy)
        {
            for (int ix = 0; ix != _phantom->nofv_x(); ++ix)
            {
                int    idx  = _phantom->idx(ix, iy, iz);
                float dosef = _edep[idx];
                fwrite( &dosef, sizeof(dosef), 1, dmx_file );      
            }
        }
    }
    fclose( dmx_file );
}

void Tally::write_d3errr_file(const char* fname) const
{
    std::string filename(fname);
    filename += ".d3e";

    FILE* dme_file = fopen(filename.c_str(), "wb");
    if (!dme_file)
        return;

    for (int iz = 0; iz != _phantom->nofv_z(); ++iz)
    {
        for (int iy = 0; iy != _phantom->nofv_y(); ++iy)
        {
            for (int ix = 0; ix != _phantom->nofv_x(); ++ix)
            {
                int    idx  = _phantom->idx(ix, iy, iz);
                float errrf = _eerr[idx];
                fwrite( &errrf, sizeof(errrf), 1, dme_file );      
            }
        }
    }
    fclose( dme_file );
}

void Tally::write_PDD_along_dmax(std::ostream& os, int mx, int my, double dmax) const
{
    for (int iz = 0; iz != _phantom->nofv_z(); ++iz)
    {
        int idx = _phantom->idx(mx, my, iz);
           
        float dosef = _edep[idx];
        os << " " << ( float(iz) + 0.5f ) * (_phantom->voxel_z()/cm) << "          "
                  << 100.0*(dosef/dmax) << std::endl;
    }
}

void Tally::write_profiles_across_dmax(std::ostream& os, int mz, double dmax) const
{
    // now plot 2d plane through zmax
    for (int iy = 0; iy != _phantom->nofv_y(); ++iy)
    {
        for (int ix = 0; ix != _phantom->nofv_x(); ++ix)
        {
            int idx = _phantom->idx(ix, iy, mz);
           
            float dosef = _edep[idx];

            float x = ( float(ix) + 0.5f ) * (_phantom->voxel_x()/cm);
            float y = ( float(iy) + 0.5f ) * (_phantom->voxel_y()/cm);

            os << std::scientific;
            os << std::setw(15) << std::setprecision(4)
               << x
               << std::setw(15) << std::setprecision(4)
               << y
               << std::setw(15) << std::setprecision(4)
               << 100.0*(dosef/dmax) << std::endl;
        }
        std::cout << "\n";
    }
}

void Tally::EndOfRunAction(const G4Run* aRun)
{
    int nof_events = aRun->GetNumberOfEvent();

    // first, convert and process energy deposition into dose, with averages
    postprocess(nof_events, 1.0); // 1.6e-13 from MeV to J, 1000 from g to kg

    // write out .out file
    write_out_file( "dose" );

    // write out .hed file
    int mx, my, mz;
    double dmax;
    write_hed_file( "dose", mx, my, mz, dmax );

    // writing binary files
    write_d3dose_file( "dose" );
    write_d3errr_file( "dose" );

    // PDD
    write_PDD_along_dmax(std::cout, _phantom->nofv_x()/2, _phantom->nofv_y()/2, 1.0);

    // profiles
    write_profiles_across_dmax(std::cout, 35, 1.0);

    std::cout << std::endl;

    std::cout << "Deposited energy per particle: " << _tot_edep << std::endl;    
    std::cout << "Deposited error per particle: " << _tot_eerr << std::endl;    

#ifdef G4VIS_USE
    if (G4VVisManager::GetConcreteInstance())
    {
        G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
    }
#endif
}
