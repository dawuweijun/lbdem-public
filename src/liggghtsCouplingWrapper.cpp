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

#include "liggghtsCouplingWrapper.h"
#include "mpi.h"

LiggghtsCouplingWrapper::LiggghtsCouplingWrapper(char **argv, MPI_Comm communicator)
  : lmp(0), x(0),v(0),f(0),t(0),r(0),omega(0),id(0)
{
  // todo: get LAMMPS to recognize command line options
  int argc_lmp = 1;
  char **argv_lmp = 0;
  argv_lmp = new char*[1];
  argv_lmp[0] = argv[0];

  lmp = new LAMMPS_NS::LAMMPS(argc_lmp,argv_lmp,communicator);

  //    delete[] argv_lmp[0];
  delete[] argv_lmp;
}
void LiggghtsCouplingWrapper::execFile(char* const fname)
{
  lmp->input->file(fname);
}
void LiggghtsCouplingWrapper::execCommand(std::stringstream const &cmd)
{
  lmp->input->one(cmd.str().c_str());
}
void LiggghtsCouplingWrapper::execCommand(char* const cmd)
{
  lmp->input->one(cmd);
}
void LiggghtsCouplingWrapper::allocateVariables()
{
  allocate_external_double(x,3,"nparticles",1.,lmp);
  allocate_external_double(v,3,"nparticles",0.,lmp);
  allocate_external_double(f,3,"nparticles",0.,lmp);
  allocate_external_double(t,3,"nparticles",0.,lmp);
  allocate_external_double(omega,3,"nparticles",0.,lmp);
  allocate_external_double(r,1,"nparticles",0.,lmp);
  allocate_external_int(id,1,"nparticles",0,lmp);
}
void LiggghtsCouplingWrapper::dataFromLiggghts()
{
  data_liggghts_to_of("x","vector-atom",lmp,(void*&)x,"double");
  data_liggghts_to_of("v","vector-atom",lmp,(void*&)v,"double");
  data_liggghts_to_of("omega","vector-atom",lmp,(void*&)omega,"double");
  data_liggghts_to_of("radius","scalar-atom",lmp,(void*&)r,"double");
  data_liggghts_to_of("id","scalar-atom",lmp,(void*&)id,"int");
}
void LiggghtsCouplingWrapper::dataToLiggghts()
{
  data_of_to_liggghts("dragforce","vector-atom",lmp,(void*)f,"double");
  data_of_to_liggghts("hdtorque","vector-atom",lmp,(void*)t,"double");
}
plb::plint LiggghtsCouplingWrapper::getNumParticles()
{
  return lammps_get_natoms(lmp);  
}
void LiggghtsCouplingWrapper::setVariable(char const *name, double value)
{
  std::stringstream cmd;
  cmd << "variable " << name << " equal " << value;
  plb::pcout << cmd.str() << std::endl;
  execCommand(cmd);
}
void LiggghtsCouplingWrapper::setVariable(char const *name, std::string &value)
{
  std::stringstream cmd;
  cmd << "variable " << name << " string " << value;
  plb::pcout << cmd.str() << std::endl;
  execCommand(cmd);
}
void LiggghtsCouplingWrapper::run(plb::plint nSteps)
{
  std::stringstream cmd;
  cmd << "run " << nSteps;
  execCommand(cmd);
}
void LiggghtsCouplingWrapper::runUpto(plb::plint nSteps)
{
  std::stringstream cmd;
  cmd << "run " << nSteps << " upto";
  execCommand(cmd);
}

                              
                           
                           
