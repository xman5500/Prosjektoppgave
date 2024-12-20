import pandas as pd
import matplotlib.pyplot as plt

# Filepath to your CSV file
filename = "Test.csv"

# Read the file while handling headers and incomplete lines
data = []
with open(filename, 'r') as file:
    for line in file:
        # Skip non-numeric lines
        if line.strip() and line[0].isdigit():
            values = line.strip().split(',')
            if len(values) == 6:  # Only process complete rows
                data.append(values)

# Convert data into a pandas DataFrame
columns = ["Index", "Value1", "Value2", "Value3", "Value4", "LastValue"]
df = pd.DataFrame(data, columns=columns)

# Convert numeric columns to appropriate types
df = df.astype({"Index": int, "LastValue": float})

# Plot using matplotlib
plt.figure(figsize=(10, 5))
plt.plot(df["Index"], df["LastValue"], linestyle='-', color='b', label="Value")
plt.xlabel("Index (First Column)")
plt.ylabel("Value (Last Column)")
plt.title("Plot of First vs Last Column")
plt.legend()
plt.grid(True)
plt.tight_layout()

# Display the plot
plt.show()
