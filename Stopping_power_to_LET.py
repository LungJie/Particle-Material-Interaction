import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

# Read Excel file
## aluminum stopping power
#data = pd.read_excel("stopping power and energy deposited aluminum.xlsx")
data = pd.read_excel("SiO2 electron and proton stopping power.xlsx")
# Initial energies [MeV]
#E0 = [22, 30, 50]
E0=[200,220]
#rho = 2.7              # Aluminum Density [g/cm^3]
rho=1.85               # PCB Density [g/cm^3]
dx_nm = 10             # Step size [nm]
dx = dx_nm * 1e-7      # Step size [cm]

# Energy and Stopping Power
E_SP = data.iloc[:, 0].values
SP_data = data.iloc[:, 1].values

# Interpolation function
SP = interp1d(E_SP, SP_data, kind='linear', fill_value='extrapolate')

# Initialize lists
E_list = []
depth_list = []
LET_list = []

for h in range(len(E0)):
    E = E0[h]
    depth = 0
    E_track = []
    depth_track = []
    LET_track = []

    while E > 0:
        # Get stopping power
        LET = SP(E)  # [MeV cm^2 / g]

        # Energy deposition
        dE = LET * rho * dx

        # Stop condition
        if dE >= E:
            dE = E
            E = 0
        else:
            E -= dE

        # Store
        E_track.append(E)
        depth_track.append(depth)
        LET_track.append(LET)

        # Update depth
        depth += dx_nm  # nm

    E_list.append(E_track)
    depth_list.append(depth_track)
    LET_list.append(LET_track)

# Convert to numpy arrays (ragged → object)
depth_array = np.array(depth_list, dtype=object)
LET_array = np.array(LET_list, dtype=object)

# Plot
plt.figure()
for i in range(3):
    depth_cm = np.array(depth_array[i]) * 1e-7
    LET_vals = np.array(LET_array[i])
    plt.plot(depth_cm, LET_vals, linewidth=2, label=f'E0 = {E0[i]} MeV')

plt.xlabel('Depth (cm)')
plt.ylabel('LET (MeV /cm)')
plt.title('LET profile in Al (CSDA)')
plt.grid()
plt.legend(loc='upper right')
plt.show()