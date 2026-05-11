# Particle Material Interaction

This is a simple demonstration of the physics of particle-material interactions.\
The objective is to estimate the impact of space radiation on a CubeSat or instrument.\
The content will cover stopping power, linear energy transfer and energy deposition.\
_Please note that all radiation effects on electronics, such as TID, TNID and SEE, originate from particle-material interactions and energy deposition in the material._

## Stopping Power 
Unit: $MeV cm^2/g$\
Physics Definition：\
The mean energy loss of an energetic particle per unit path length in a given material\
Stopping power is obtained from the NIST STAR database:：\
https://www.nist.gov/pml/stopping-power-range-tables-electrons-protons-and-helium-ions\
<img src="image/aluminum_stopping_power.png" width="60%">\
The y axis, stopping power can be explain as $Energy [MeV] /Density[(g/cm^3)] \times thickness(cm)$\
Reference：[Inokuti, M., & Argonne National Lab., IL (United States). (1995). Stopping power, its meaning, and its general characteristics.](<Reference document/Stopping power its meaning and its general characteristics.pdf>)

## Linear Energy Transfer
Unit: $MeV/cm$\
Definition：Average energy deposited per unit length of track\
Continuous Slowing Down Approximation (CSDA)\
[CSDA LET Python code](Stopping_power_to_LET.py)
1. Assume the material thickness $ L $. Set unit length $ \Delta t=10 nm$ 
2. At $ L=0 $, the incident particle with energy $ E =E_{0}$, the initial particle stopping power $ T(E_{0})=T_{0}$ (refer to stopping power profile) 
3. At $ L=\Delta t $, the incident particle energy $ E_{1} =E_{0}-(T_{0}\Delta t)$, the particle stopping power $ T(E_{1})=T_{1}$(refer to stopping power profile) 
4. At $ L=2\Delta t $, the incident particle energy $ E_{2} =E_{1}-(T_{1}\Delta t)$, the particle stopping power $ T(E_{2})=T_{2}$(refer to stopping power profile) 

<img src="image/LET_profile_aluminum.png" width="60%">\


## Energy Deposition
LET profile integration\
Particle energy loss=particle energy deposition\
$\int_0^L {T} \,{\rm d}t $ (Estimate the energy deposition in the material)\
Particle energy remaining\
$E=E_{0}-\int_0^L {T} \,{\rm d}t $ (Estimate the particle energy when it pass through shielding)\
[Python code of Stopping power to energy deposition](Stopping_power_to_energy_deposition.py)


## Geant4 Simulation
Toolkit for the simulation of the passage of particles through matter\
Geant4：https://geant4.web.cern.ch/\
Basic structure：https://prezi.com/i/gp3kiz0ubh3e/geant4-basic-structure/ \
Physic list document：[PhysicsListGuide](<Reference document/PhysicsListGuide.pdf>)\
Helpful tutorial：
https://www.youtube.com/playlist?list=PL1AYUn8GT4HkL09iElGRk8NRIfR3tUywQ

### Examples：DSRP
Deep Space Radiation Probe (DSRP) was developed by National Central University\
Reference :\
L. C. Chang et al., "The Deep Space Radiation Probe: Development of a first lunar science payload for space environment studies and capacity building," Advances in Space Research, 2024, doi: 10.1016/j.asr.2024.05.032.

### DSRP Model Geometry
+ Top and bottom panel：2.5 mm aluminum 
+ PCB：1.6 mm 
+ RADFET：400 nm SiO2
### Model
1. DSRP Simple Model (Based on Geant4 basic example B1) 
[Geant4 code](<DSRP reduced model>)\
 a. [DetectorConstruction](<DSRP reduced model/src/DetectorConstruction.cc>)\
 b. [PrimaryGeneratorAction](<DSRP reduced model/src/PrimaryGeneratorAction.cc>)\
 c. [Physicslist](<DSRP reduced model/exampleB1.cc>)
2. DSRP Model changing incident angle (Based on Geant4 basic example B5)
 [Geant4 code](<DSRP model adjust the incident angle>)
### Results
---
The figure shows the proton, electron and helium interaction with material\
From Geant4 simulation we can observe the secondary particles contribution and some fun phonomenon
+ The electron possess obviouse scattering effect, the gamma ray as secondary radiaiton can be observed. Since electron has lowest stopping power, it can transverse the material most easily
+ When proton and helium transverse the material, electron as secondary particle would be generated which can't be astimate as the simple calculation as we discuss previously
![Geant4 simulation results](<image/geant4 simulation.png>)
---
The figure shows the geant4 simulation results with proton as incident particle
<img src="image/Al stopping power_energy deposited_LET profile subplot version thesis v update.png" width="80%">\
Reference：
1. J. Allison et al., "Geant4 developments and applications," IEEE Transactions on Nuclear Science, vol. 53, no. 1, pp. 270-278, Feb. 2006, doi: 10.1109/TNS.2006.869826.
2. S. Agostinelli et al., "Geant4—a simulation toolkit," Nuclear Instruments and Methods in Physics Research Section A: Accelerators, Spectrometers, Detectors and Associated Equipment, vol. 506, no. 3, pp. 250-303, 2003/07/01/ 2003, doi: https://doi.org/10.1016/S0168-9002(03)01368-8.


