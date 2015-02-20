#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>
#include <string>
#include <limits>

#include "G4SystemOfUnits.hh"

#include "Spectrum.hh"

Spectrum::Spectrum(const char* fname):
    _nof_bins(-1),
    _min_energy(-1.0),
    _norm(-1.0)

{
    std::ifstream is(fname, std::ios::in);

    if (not is.good())
        throw std::logic_error("Cannot find spectrum file");

    std::string tag, val;

    // read either min.energy or # of bins tags
    is >> tag >> val;
    if (tag == "MINIMUM-ENERGY:")
        _min_energy = std::stod(val);
    if (tag == "NUM-BINS-USER-SPECTRUM:")
        _nof_bins = std::stoi(val);

    // read either min.energy or # of bins tags
    is >> tag >> val;
    if (tag == "MINIMUM-ENERGY:")
        _min_energy = std::stod(val);
    if (tag == "NUM-BINS-USER-SPECTRUM:")
        _nof_bins = std::stoi(val);

    if (_min_energy < 0.0)
        throw std::logic_error("Energy min is bad");

    if (_nof_bins < 1)
        throw std::logic_error("# of bins is bad");

    _pos.reserve(_nof_bins+1);
    _prb.reserve(_nof_bins+1);

    _pos.emplace_back(_min_energy*MeV);
    _prb.emplace_back(0.0);

    for(int k = 0; k != _nof_bins; ++k)
    {
        double e, p;
        is >> e >> p;

        _pos.emplace_back(e*MeV);
        _prb.emplace_back(p);
    }

    is.close();

    normalize();
}

void Spectrum::normalize()
{
    // compute cumulative probabilities
    for(int k = 1; k != _nof_bins+1; ++k)
    {
        _prb[k] += _prb[k-1];
    }

    // normalizing probabilities
    _norm = _prb[_nof_bins];
    double n = 1.0/_norm;

    for(int k = 0; k != _nof_bins+1; ++k)
    {
        _prb[k] *= n;
    }

    // just to ensure we always miss first bin even if rn=0
    _prb[0] = -std::numeric_limits<double>::epsilon();

    // just to ensure we always miss last bin even if rn=1
    _prb[_nof_bins] += std::numeric_limits<double>::epsilon();
}

double Spectrum::sample(double rn1, double rn2) const
{
    // right boundary
    auto ir = sample_bin(rn1);
    assert(ir != _pos.cend());

    // left boundary
    auto il = ir;
    --il;

    // sample energy uniformly in the bin
    return *il + (*ir - *il)*rn2;
}
