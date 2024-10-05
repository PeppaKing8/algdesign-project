import numpy as np

import matplotlib.pyplot as plt

# # Read data from file
# data_file = './data.txt'
# min_data = []
# fast_data = []

# with open(data_file, 'r') as file:
#     for line in file:
#         # Assuming each line contains a single number
#         time_str = line.split()[5][:-1]  # Extract the time after "Time:"
#         if "Min" in line:
#             min_data.append(float(time_str))
#         else:
#             fast_data.append(float(time_str))

# # Process data (example: sort the data)
# min_avg = sum(min_data) / len(min_data) if min_data else 0
# fast_avg = sum(fast_data) / len(fast_data) if fast_data else 0

# print(f"Average time for Min: {min_avg}")
# print(f"Average time for Fast: {fast_avg}")

index = [200, 300, 400, 500, 600, 700]
min_cut = [0.5267558900000001, 2.094876100000001, 5.455971599999998, 11.097854100000001, 20.372906000000004, 32.872039]
fast_cut = [1.057735682, 3.0368996100000007, 7.168114840000004, 14.01775702,  22.725273680000004, 34.68207300000001]
# Fit a polynomial trend line (degree 2 for example)
min_trend = np.poly1d(np.polyfit(index, min_cut, 3))
fast_trend = np.poly1d(np.polyfit(index, fast_cut, 3))

# Generate x values for the trend lines
trend_x = np.linspace(min(index), max(index)*1.50, 100)
# Extend the trend lines beyond the original data range
extended_trend_x = np.linspace(min(index), max(index) * 1.5, 150)
# Plot the trend lines with dashed lines
plt.plot(trend_x, min_trend(trend_x), 'r--', label='Min Cut Trend')
plt.plot(trend_x, fast_trend(trend_x), 'b--', label='Fast Cut Trend')
plt.plot(index, min_cut, label='Min Cut', marker='o')
plt.plot(index, fast_cut, label='Fast Cut', marker='x')
plt.yscale('log')
plt.xlabel('Number of Vertices')
plt.ylabel('Time')
plt.title('Min Cut vs Fast Cut')
plt.legend()
plt.grid(True)

plt.savefig('MinCut.png')