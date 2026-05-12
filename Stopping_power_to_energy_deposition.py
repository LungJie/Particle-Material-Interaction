import numpy as np
import pandas as pd
from scipy.interpolate import interp1d

# Read Excel
data = pd.read_excel("stopping power and energy deposited aluminum.xlsx")

# Initial energies [MeV]
E0 = np.array([22, 30, 50])

rho = 2.7              # Density [g/cm^3]
dx_nm = 10             # Step size [nm]
dx = dx_nm * 1e-7      # Step size [cm]

# Energy and stopping power
E_SP = data.iloc[:, 0].values   # column 3 in MATLAB
SP_data = data.iloc[:, 1].values  # column 4 in MATLAB

# Interpolation
SP = interp1d(E_SP, SP_data, kind='linear', fill_value='extrapolate')

# Aluminum thickness
Al_thickness_nm = 2.5e6   # 2.5 mm

# Output arrays
E_residual = np.zeros(len(E0))
E_deposited = np.zeros(len(E0))

# Main loop
for h in range(len(E0)):
    E = E0[h]
    depth = 0

    while E > 0 and depth < Al_thickness_nm:

        # LET
        LET = SP(E)   # [MeV cm^2 / g]

        # Energy loss
        dE = LET * rho * dx

        # Avoid overshoot
        if dE >= E:
            E = 0
            break
        else:
            E -= dE

        # Update depth
        depth += dx_nm

    # Store results
    E_residual[h] = E
    E_deposited[h] = E0[h] - E

# Print results (optional)
for i in range(len(E0)):
    print(f"E0 = {E0[i]} MeV → Residual = {E_residual[i]:.4f} MeV, Deposited = {E_deposited[i]:.4f} MeV")