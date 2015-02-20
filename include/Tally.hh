// -*- C++ -*-

#pragma once

#include <iosfwd>

#include "globals.hh"
#include "G4UserRunAction.hh"

class Phantom;
class G4LogicalVolume;
class G4Run;

class Tally : public G4UserRunAction
{
#pragma region Data
    private: Phantom*  _phantom;

    private: double*   _edep;
    private: double*   _eerr;
    private: int       _maxTally;

    // total energy deposition and 
    private: mutable double _tot_edep;
    private: mutable double _tot_eerr;     
#pragma endregion

#pragma region Ctor/Dtor/ops
    public:  Tally( Phantom* phantom );
    public: ~Tally();
#pragma endregion

#pragma region Overrides
    public: virtual void BeginOfRunAction(const G4Run*);
    public: virtual void EndOfRunAction(const G4Run*);
#pragma endregion
    
#pragma region Observers
    public: double* edep() const
    {
        return _edep;
    }

    public: double* eerr() const
    {
        return _eerr;
    }

    public: double& tot_edep() const
    {
        return _tot_edep;
    }

    public: double& tot_eerr() const
    {
        return _tot_eerr;
    }    

    public: G4LogicalVolume* voxel_box_log() const;

    private: void postprocess(int nof_events, double mult = 1.0) const;
#pragma endregion

#pragma region I/O
    private: void write_axis(std::ostream& os, int nv, float start, float voxel) const;

    public:  void write_out_file(const char* fname) const;
    public:  void write_hed_file(const char* fname, int& mx, int& my, int& mz, double& dmax) const;

    public:  void write_d3dose_file(const char* fname) const;
    public:  void write_d3errr_file(const char* fname) const;

    public:  void write_PDD_along_dmax(std::ostream& os, int mx, int my, double dmax) const;
    public:  void write_profiles_across_dmax(std::ostream& os, int mz, double dmax) const;
#pragma endregion
};
