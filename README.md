lbdem-dev
=========

About
-----

LBDEMcoupling is a coupling between the Lattice Boltzmann (LB) library
Palabos (http://www.palabos.org) and the Discrete Element Method code
LIGGGHTS (http://www.ligggghts.com). 

Installation
------------

Just clone the git repo, and create the following three shell
variables in your .bashrc:

          export PALABOS_ROOT=path/to/palabos/
          export LIGGGHTS_ROOT=path/to/liggghts/
          export LBDEM_ROOT=path/to/lbdem/

Setting Up a Simulation
-----------------------

It is assumed that you have basic knowledge in both Palabos and
LIGGGHTS. If you are not familiar with one or both of the codes, it is
strongly recommended that you work through a few tutorial and example
cases.

To compile a case, it is necessary to build LIGGGHTS as a
library. This is explained in the LIGGGHTS documentation. Compiling a
case works just like in Palabos. In the Makefile, you will find the
line

          linkFlags    = -Wl,--whole-archive -llmp_fedora -Wl,--no-whole-archive

In this line, you need to replace the -llmp_fedora with the name of
your library (usually called lmp_whatever).



License and Copyright
---------------------

(c) Johannes Kepler University Linz, Austria

released under the GPLv3

main author: Philippe Seil (philippe.seil@jku.at)
