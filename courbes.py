import numpy as np
import matplotlib.pyplot as plt

def plot_acceleration():
    # Charger les données
    data_x = np.loadtxt("courbes/simulation_data_a_x.txt")
    data_y = np.loadtxt("courbes/simulation_data_a_y.txt")
    data_z = np.loadtxt("courbes/simulation_data_a_z.txt")

    temps_x = data_x[:, 0]
    acceleration_x = data_x[:, 1]

    temps_y = data_y[:, 0]
    acceleration_y = data_y[:, 1]

    temps_z = data_z[:, 0]
    acceleration_z = data_z[:, 1]

    # Tracer la courbe
    plt.plot(temps_x, acceleration_x, marker='o', linestyle='-', label="Accélération X")
    plt.plot(temps_y, acceleration_y, marker='x', linestyle='--', label="Accélération Y")
    plt.plot(temps_z, acceleration_z, marker='s', linestyle='-', label="Accélération Z")

    plt.xlabel("Temps (ms)")
    plt.ylabel("Accélération (m/s²)")
    plt.title("Évolution des accélérations en fonction du temps")
    plt.grid()
    plt.legend()

    # Sauvegarder le graphique
    plt.savefig('courbes/acc_vs_time.png')
    print("Graphique sauvegardé sous 'courbes/acc_vs_time.png'")
    plt.clf()  # Clear the figure for the next plot

def plot_velocity():
    # Charger les données
    data_x = np.loadtxt("courbes/simulation_data_v_x.txt")
    data_y = np.loadtxt("courbes/simulation_data_v_y.txt")
    data_z = np.loadtxt("courbes/simulation_data_v_z.txt")

    temps_x = data_x[:, 0]
    vitesse_x = data_x[:, 1]

    temps_y = data_y[:, 0]
    vitesse_y = data_y[:, 1]

    temps_z = data_z[:, 0]
    vitesse_z = data_z[:, 1]

    # Tracer la courbe
    plt.plot(temps_x, vitesse_x, marker='o', linestyle='-', label="Vitesse X")
    plt.plot(temps_y, vitesse_y, marker='x', linestyle='--', label="Vitesse Y")
    plt.plot(temps_z, vitesse_z, marker='s', linestyle='-', label="Vitesse Z")

    plt.xlabel("Temps (ms)")
    plt.ylabel("Vitesse (m/s)")
    plt.title("Évolution des vitesses en fonction du temps")
    plt.grid()
    plt.legend()

    # Sauvegarder le graphique
    plt.savefig('courbes/v_vs_time.png')
    print("Graphique sauvegardé sous 'courbes/v_vs_time.png'")
    plt.clf()  # Clear the figure for the next plot

def plot_position():
    # Charger les données
    data_x = np.loadtxt("courbes/simulation_data_p_x.txt")
    data_y = np.loadtxt("courbes/simulation_data_p_y.txt")
    data_z = np.loadtxt("courbes/simulation_data_p_z.txt")

    temps_x = data_x[:, 0]
    position_x = data_x[:, 1]

    temps_y = data_y[:, 0]
    position_y = data_y[:, 1]

    temps_z = data_z[:, 0]
    position_z = data_z[:, 1]

    # Tracer la courbe
    plt.plot(temps_x, position_x, marker='o', linestyle='-', label="Position X")
    plt.plot(temps_y, position_y, marker='x', linestyle='--', label="Position Y")
    plt.plot(temps_z, position_z, marker='s', linestyle='-', label="Position Z")

    plt.xlabel("Temps (ms)")
    plt.ylabel("Position (m)")
    plt.title("Évolution des positions en fonction du temps")
    plt.grid()
    plt.legend()

    # Sauvegarder le graphique
    plt.savefig('courbes/pos_vs_time.png')
    print("Graphique sauvegardé sous 'courbes/pos_vs_time.png'")
    plt.clf()  # Clear the figure for the next plot

# Exécuter les trois tracés
plot_acceleration()
plot_velocity()
plot_position()
