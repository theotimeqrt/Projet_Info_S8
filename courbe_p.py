import numpy as np
import matplotlib.pyplot as plt

# Charger les données
data = np.loadtxt("simulation_data_p.txt")
temps = data[:, 0]
position_z = data[:, 1]

# Tracer la courbe
plt.plot(temps, position_z, marker='o', linestyle='-')
plt.xlabel("Temps (ms)")
plt.ylabel("Position z (m)")
plt.title("Évolution de la position z en fonction du temps")
plt.grid()

# Save the plot to a file instead of showing it
plt.savefig('position_vs_time.png')  # Save as PNG (can also use .pdf, .svg, etc.)
print("Graph saved as 'position_vs_time.png'")
