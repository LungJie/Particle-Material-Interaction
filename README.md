# Particle Material Interaction
This is a simple demostration about particle material interaction physics\
The objective is to estimate the space radiation impact on CubeSat or instrument\
The content will cover the stopping power, linear energy transfer and the energy deposition
## Space Energetic Particles
1. Proton
2. Electron
3. Helium
## Stopping Power 
Unit: $MeV cm^2/g$\
Physics Definition：\
The mean energy loss of an energetic particle per unit path length in a given material\
Stopping power dataset：https://www.nist.gov/pml/stopping-power-range-tables-electrons-protons-and-helium-ions\
<img src="image/aluminum_stopping_power.png" width="60%">\
The y axis, stopping power can be explain as $Energy [MeV] /Density[(g/cm^3)]*thickness(cm)$\
Reference：STOPPING POWER, ITS MEANING, AND ITS GENERAL CHARACTERISTICS\

## Linear Energy Transfer
<img src="image/LET_profile_aluminum.png" width="60%">\
When particle transverse the material

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





## Reference
1. Particle Interactions with Matter：https://cds.cern.ch/record/2674116/files/660.pdf

