import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load the CSV file
def plot_from_csv(file_path):
    # Load the CSV into a DataFrame, assigning headers directly
    headers = ['Iterationnr', 'x-axis (mm)', 'y-axis (mm)', 'z-axis (mm)', 'z-force (g)', 'x-force (g)']
    data = pd.read_csv(file_path, header=None, names=headers)

    # Drop rows with missing data in critical columns
    data = data.dropna(subset=['x-axis (mm)', 'z-force (g)', 'x-force (g)'])

    # Create the plots
    fig, axs = plt.subplots(2, 1, figsize=(10, 8))

    # Plot x-axis and z-axis positions vs iteration number
    axs[0].plot(data['Iterationnr'], data['x-axis (mm)'], label='X-Axis Position (mm)', color='g', linestyle='-')
    axs[0].plot(data['Iterationnr'], data['z-axis (mm)'], label='Z-Axis Position (mm)', color='orange', linestyle='-')
    axs[0].set_title('Positions Over Iterations')
    axs[0].set_xlabel('Iteration Number')
    axs[0].set_ylabel('Position (mm)')
    axs[0].legend()
    axs[0].grid()

    # Plot z-force and x-force over iterations
    axs[1].plot(data['Iterationnr'], data['z-force (g)'], label='Z-Force (g)', color='r', linestyle='-')
    axs[1].plot(data['Iterationnr'], data['x-force (g)'], label='X-Force (g)', color='b', linestyle='-')
    axs[1].set_title('Forces Over Iterations')
    axs[1].set_xlabel('Iteration Number')
    axs[1].set_ylabel('Force (g)')
    axs[1].legend()
    axs[1].grid()

    # Adjust layout
    plt.tight_layout()

    # Show the plots
    plt.show()

# Example usage
file_path = "vat test 9.csv"  # Replace with the path to your CSV file

# vat test 6 (0.05 mm down) Good results
# Tests including vat 7 moves x in 1 mm, after only 0,5 

plot_from_csv(file_path)