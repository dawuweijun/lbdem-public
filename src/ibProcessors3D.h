/*
 * This file is part of the LBDEMcoupling software.
 *
 * LBDEMcoupling is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2014 Johannes Kepler University Linz
 *
 * Author: Philippe Seil (philippe.seil@jku.at)
 */
#ifndef IB_PROCESSORS_3D_H
#define IB_PROCESSORS_3D_H

#include "liggghtsCouplingWrapper.h"
#include "physunits.h"

namespace plb{
  template<typename T>
  struct ParticleData {
    typedef typename std::vector<Array<T,3> > ParticleDataArrayVector;
    typedef typename std::vector<T> ParticleDataScalarVector;
  };

  template<typename T, template<typename U> class Descriptor>
  struct SetSingleSphere3D : public BoxProcessingFunctional3D_L<T,Descriptor> {
  public:
    SetSingleSphere3D(T *x_, T *v_, T *omega_, T r_, int id_, bool initVelFlag_)
      : x(x_),v(v_),omega(omega_),r(r_),id(id_), initVelFlag(initVelFlag_) {}
    SetSingleSphere3D(T *x_, T *v_, T *omega_, T r_, int id_)
      : x(x_),v(v_),omega(omega_),r(r_),id(id_), initVelFlag(false) {}
    SetSingleSphere3D(T *x_, T *v_, T r_, int id_)
      : x(x_),v(v_),omega(0),r(r_),id(id_), initVelFlag(false) {}
    SetSingleSphere3D(const SetSingleSphere3D &orig)
      : x(orig.x),v(orig.v),omega(orig.omega),r(orig.r),
      id(orig.id),initVelFlag(orig.initVelFlag) {}

    Box3D getBoundingBox() const { return Box3D(x[0]-r-2,x[0]+r+2,
                                                x[1]-r-2,x[1]+r+2,
                                                x[2]-r-2,x[2]+r+2);}

    virtual void process(Box3D domain, BlockLattice3D<T,Descriptor> &lattice);
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const;
    SetSingleSphere3D<T,Descriptor>* clone() const;
  private:
    T *x,*v,*omega,r;
    int id;
    bool initVelFlag;
    T calcSolidFraction(T dx_, T dy_, T dz_, T r_);
    void setValues(Cell<T,Descriptor>& c, T sf, T dx, T dy, T dz);
    void setToZero(Cell<T,Descriptor>& c);
  };
  
  /*
   * this functional does nothing, it is just used to get 
   * all the static variables right after looping over 
   * the spheres
   */
  template<typename T, template<typename U> class Descriptor>
  struct AttributeFunctional : public BoxProcessingFunctional3D_L<T,Descriptor> {
  public:
    virtual void process(Box3D domain, BlockLattice3D<T,Descriptor> &lattice) {}
    virtual void getTypeOfModification(std::vector<modif::ModifT>& modified) const
    {modified[0] = modif::staticVariables;}

    AttributeFunctional<T,Descriptor>* clone() const
    { return new AttributeFunctional<T,Descriptor>(*this);}
  };
  
  // template<typename T, template<typename U> class Descriptor>
  // void setSpheresOnLattice(MultiBlockLattice3D<T,Descriptor> &lattice,
  //                          T **x, T **v, T **omega, T *r, int **id, plint nSpheres, bool initVelFlag);
  template<typename T, template<typename U> class Descriptor>
  void setSpheresOnLattice(MultiBlockLattice3D<T,Descriptor> &lattice,
                           LiggghtsCouplingWrapper &wrapper,
                           PhysUnits3D<T> const &units,
                           bool initVelFlag);


  template<typename T, template<typename U> class Descriptor>
  void getForcesFromLattice(MultiBlockLattice3D<T,Descriptor> &lattice,
                            LiggghtsCouplingWrapper &wrapper,
                            PhysUnits3D<T> const &units);

  template<typename T, template<typename U> class Descriptor>
  void setSpheresOnLatticeNew(MultiBlockLattice3D<T,Descriptor> &lattice,
                              LiggghtsCouplingWrapper &wrapper,
                              PhysUnits3D<T> const &units,
                              bool initVelFlag);

  template<typename T, template<typename U> class Descriptor>
  void getForcesFromLatticeNew(MultiBlockLattice3D<T,Descriptor> &lattice,
                               LiggghtsCouplingWrapper &wrapper,
                               PhysUnits3D<T> const &units);

  template<typename T, template<typename U> class Descriptor>
  struct SumForceTorque3D : public BoxProcessingFunctional3D_L<T,Descriptor> {
  public:
    SumForceTorque3D(typename ParticleData<T>::ParticleDataArrayVector &x,
                     T *force_, T *torque_, LiggghtsCouplingWrapper &wrapper_);
    SumForceTorque3D(SumForceTorque3D<T,Descriptor> const &orig);

    virtual void process(Box3D domain, BlockLattice3D<T,Descriptor>& lattice);

    SumForceTorque3D<T,Descriptor>* clone() const;
    void getTypeOfModification(std::vector<modif::ModifT>& modified) const;

  private:
    typename ParticleData<T>::ParticleDataArrayVector &x;
    T *force,*torque;
    LiggghtsCouplingWrapper &wrapper;
    void addForce(plint partId, plint coord, T value);
    void addTorque(plint partId, plint coord, T value);
  };

}; // plb

#include "ibProcessors3D.hh"

#endif // IB_PROCESSORS_3D_H
