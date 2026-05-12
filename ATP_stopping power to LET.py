import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

# Read Excel file
data = pd.read_excel("SiO2 electron and proton stopping power.xlsx")

# Initial energy [MeV]
E0 = 220

rho = 1.85              # PCB Density [g/cm^3]
# Step size
dx_nm = 10              # 10 nm
dx = dx_nm * 1e-7       # cm
#dx_mm = 0.001           # Step size [mm]
#dx = dx_mm * 1e-1       # Step size [cm]

# Maximum depth
max_depth_mm = 1.6

# Energy and stopping power
E_SP = data.iloc[:, 0].values
SP_data = data.iloc[:, 1].values

# Interpolation function
SP = interp1d(E_SP, SP_data, kind='linear', fill_value='extrapolate')

# Initialize
E = E0
depth = 0

E_track = []
depth_track = []
LET_track = []

# Main loop
##while E > 0:
#
#    # Get stopping power
#    LET = SP(E)   # [MeV cm^2 / g]
#
#    # Energy loss
#    dE = LET * rho * dx
#
#    # Stop condition
#    if dE >= E:
#        dE = E
#        E = 0
#    else:
#        E -= dE
#
#    # Store data
#    E_track.append(E)
#    depth_track.append(depth)
#    LET_track.append(LET)

    # Update depth
#    depth += dx_mm   # mm

while depth < max_depth_mm and E > 0:

    # Stopping power
    LET = SP(E)   # [MeV cm^2 / g]

    # Energy loss
    dE = LET * rho * dx

    # Avoid overshoot
    if dE >= E:
        dE = E
        E = 0
    else:
        E -= dE

    # Store data
    E_track.append(E)
    depth_track.append(depth*0.1)
    LET_track.append(LET)

    # Update depth
    depth += dx_nm * 1e-6   # convert nm → mm

# Convert to numpy arrays
depth_track = np.array(depth_track)
LET_track = np.array(LET_track)
E_track = np.array(E_track)

# Plot LET profile
plt.figure()

#plt.plot(depth_track, LET_track, linewidth=2,
#         label=f'E0 = {E0} MeV')
#plt.xlabel('Depth (cm)')
#plt.ylabel('Stopping power (MeV cm^2/g)')
#plt.title('LET profile in PCB')
# Plot remaining particle energy
plt.plot(depth_track, E_track, linewidth=2)
plt.xlabel('Depth (cm)')
plt.ylabel('Particle Energy (MeV)')
plt.title('Incident Particle Energy')
plt.grid()
plt.legend()

plt.show()