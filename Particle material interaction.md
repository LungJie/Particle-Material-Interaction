# Particle Material Interaction
This is a simple demostration about particle material interaction physics\
The objective is to estimate the space radiation impact on CubeSat or instrument\
The content will cover the stopping power, linear energy transfer and the energy deposition

## Stopping Power 
Unit: $MeV cm^2/g$\
Physics Definition：\
The mean energy loss of an energetic particle per unit path length in a given material\
Stopping power is obtained from the NIST STAR database:：\
https://www.nist.gov/pml/stopping-power-range-tables-electrons-protons-and-helium-ions\
<img src="image/aluminum_stopping_power.png" width="60%">\
The y axis, stopping power can be explain as $Energy [MeV] /Density[(g/cm^3)]*thickness(cm)$\
Reference：[Inokuti, M., & Argonne National Lab., IL (United States). (1995). Stopping power, its meaning, and its general characteristics.](<Reference document/Stopping power its meaning and its general characteristics.pdf>)

## Linear Energy Transfer
Unit: $MeV/cm$\
Definition：Average energy deposited per unit length of track\
<img src="image/LET_profile_aluminum.png" width="60%">\


## Energy Deposition
[Stopping power to energy deposition](Stopping_power_to_energy_deposition.py)\
LET profile integration

## Geant4 Simulation
Toolkit for the simulation of the passage of particles through matter\
Geant4：https://geant4.web.cern.ch/\
Basic structure：https://prezi.com/i/gp3kiz0ubh3e/geant4-basic-structure/\
Physic list document：[PhysicsListGuide](<Reference document/PhysicsListGuide.pdf>)\
Helpful tutorial：
https://www.youtube.com/playlist?list=PL1AYUn8GT4HkL09iElGRk8NRIfR3tUywQ
Examples\
1. DSRP Simple Model (Based on Geant4 basic example B1) 
[Geant4 code](<DSRP reduced model>)
2. DSRP Model changing incident angle (Based on Geant4 basic example B5)
 [Geant4 code](<DSRP model adjust the incident angle>)
![Geant4 simulation results](<image/geant4 simulation.png>)

Reference：
1. J. Allison et al., "Geant4 developments and applications," IEEE Transactions on Nuclear Science, vol. 53, no. 1, pp. 270-278, Feb. 2006, doi: 10.1109/TNS.2006.869826.
2. S. Agostinelli et al., "Geant4—a simulation toolkit," Nuclear Instruments and Methods in Physics Research Section A: Accelerators, Spectrometers, Detectors and Associated Equipment, vol. 506, no. 3, pp. 250-303, 2003/07/01/ 2003, doi: https://doi.org/10.1016/S0168-9002(03)01368-8.


