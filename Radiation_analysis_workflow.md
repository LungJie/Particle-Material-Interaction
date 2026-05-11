# Radiation Analysis Workflow
1. Define and simulate the environment condiiton
    + What kinds of particle
    + How much the incident energy
2. Define and simulate the instrument fluence to deposited energy
    + Obtain by stopping power to energy deposition transfer 
    + Obtain by Geant4 simulation
3. Integrating the differential flux with fluence to absorbed dose 
    $$
    D = \int_{0}^{\infty} d(E)\,\frac{d\Phi}{dE}\,dE
    $$
4. TID, TNID and SEE estimation
    + TNID about 0.1% of total energy loss

# Example
## DSRP Analysis
### Objective: Analysis the expected energy deposition when DSRP transverse the radiation belt
1. Environment > Radiation Belt
    Incident particle fluence by IRENE simulation
2. Instrument fluence to deposited energy
    Obtain by Geant4 simulation
## ATP Test