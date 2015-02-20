// -*- C++ -*-

#pragma once

#include <algorithm>
#include <vector>

class Spectrum
{
#pragma region typedefs
    public: typedef std::vector<double>            container_type;

    public: typedef container_type::iterator       iterator;    
    public: typedef container_type::const_iterator const_iterator;    
#pragma endregion

#pragma region Data
    private: int    _nof_bins;
    private: double _min_energy;
    private: double _norm;

    // bin position and bin probablility
    private: container_type _pos;
    private: container_type _prb;
#pragma endregion

#pragma region Ctor/Dtor/ops
    public: Spectrum(const char* fname);

    public: Spectrum(const Spectrum& spec):
        _pos(spec._pos),
        _prb(spec._prb)
    {
    }

    public: ~Spectrum()
    {
    }
#pragma endregion

#pragma region Observers
    public:  int nof_bins() const
    {
        return _nof_bins;
    }

    public: double min_energy() const
    {
        return _min_energy;
    }

    public: double norm() const
    {
        return _norm;
    }

    public: const std::vector<double>& pos() const
    {
        return _pos;
    }

    public: const std::vector<double>& prb() const
    {
        return _prb;
    }  
#pragma endregion

#pragma region Sampling
    public: const_iterator sample_bin(double rn) const
    {
        return _pos.cbegin() + (std::lower_bound(_prb.cbegin(), _prb.cend(), rn) - _prb.cbegin());
    }

    public: double         sample(double rn1, double rn2) const;
#pragma endregion

    protected: void normalize();
};
