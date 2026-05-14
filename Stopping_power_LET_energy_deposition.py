import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

# =========================================================
# USER SETTINGS
# =========================================================

# ----- Material selection -----
material_name = "Aluminum"
#material_name = "SiO2"
# Excel file
excel_file = "stopping power and energy deposited aluminum.xlsx"
#excel_file = "SiO2 electron and proton stopping power.xlsx"

# Density [g/cm^3]
rho = 2.7 #Aluminum density
#rho = 1.85  #SiO2 density
# ----- Initial energies [MeV] -----
# Single energy:
#E0_list = [220]

# Multiple energies:
E0_list = [22, 30, 50]

# ----- Step size -----
dx_nm = 10         # nm(1mm)
#dx_nm = 1*10^6          # nm(1mm)

# ----- Maximum depth -----
max_depth_mm = 120  # mm 
#max_depth_mm = 1.6  # mm one PCB
# ----- Plot mode -----
# "LET" or "Energy"
plot_mode = "LET"
#plot_mode="Energy Remaining"
#plot_mode="Energy Deposition"
# =========================================================
# READ STOPPING POWER DATA
# =========================================================

data = pd.read_excel(excel_file)

# Energy column
E_SP = data.iloc[:, 0].values

# Stopping power column
SP_data = data.iloc[:, 1].values

# Interpolation function
SP = interp1d(
    E_SP,
    SP_data,
    kind='linear',
    fill_value='extrapolate'
)

# =========================================================
# UNIT CONVERSION
# =========================================================

# Step size:
# nm → cm
dx_cm = dx_nm * 1e-7

# nm → mm
dx_mm = dx_nm * 1e-6

# =========================================================
# STORAGE
# =========================================================

all_depth = []
all_LET = []
all_energy = []
all_energy_deposition=[]
# =========================================================
# MAIN LOOP
# =========================================================

for E0 in E0_list:

    E = E0
    depth_mm = 0

    depth_track = []
    LET_track = []
    energy_track = []
    energy_deposition=[]

    while depth_mm < max_depth_mm and E > 0:

        # Stopping power
        LET = SP(E)   # [MeV cm^2 / g]

        # Energy loss
        dE = LET * rho * dx_cm

        # Prevent overshoot
        if dE >= E:
            dE = E
            E = 0
        else:
            E -= dE
            DE=E0-E
        # Store
        depth_track.append(depth_mm)
        LET_track.append(LET)
        energy_track.append(E)
        energy_deposition.append(DE)
        #energy_deposition=E0-E
    

        # Update depth
        depth_mm += dx_mm

    # Save results
    all_depth.append(np.array(depth_track))
    all_LET.append(np.array(LET_track))
    all_energy.append(np.array(energy_track))
    all_energy_deposition.append(np.array(energy_deposition))
# =========================================================
# PLOT
# =========================================================

plt.figure(figsize=(8,6))

for i, E0 in enumerate(E0_list):

    depth = all_depth[i]

    if plot_mode == "LET":

        plt.plot(
            depth,
            all_LET[i],
            linewidth=2,
            label=f'{E0} MeV'
        )

        plt.ylabel('Stopping Power (MeV cm$^2$/g)')
        plt.title(f'LET Profile in {material_name}')

    elif plot_mode == "Energy Remaining":

        plt.plot(
            depth,
            all_energy[i],
            linewidth=2,
            label=f'{E0} MeV'
        )

        plt.ylabel('Particle Energy (MeV)')
        plt.title(f'Incident Particle Remaining Energy')
    elif plot_mode == "Energy Deposition":

        plt.plot(
            depth,
            #all_energy[i],
            all_energy_deposition[i],
            linewidth=2,
            label=f'{E0} MeV'
        )

        plt.ylabel('Energy Deposition (MeV)')
        plt.title(f'Deposition Energy in {material_name}')
# Common settings
plt.xlabel('Depth (mm)')
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()