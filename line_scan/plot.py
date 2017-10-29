import numpy as np
import matplotlib.pyplot as plt

# plotter
n = np.arange(1, 200)
S = 64

T_1 = np.vectorize(lambda n: np.sum(
    [np.sum([1 for m in range(n-k)]) for k in range(n)]
))

T_2 = np.vectorize(lambda n: np.sum(
    [np.sum([1 for m in range(max(0, S-k), n-k)]) for k in range(n)]
))

print(T_1(128))
print(T_2(128))
print((T_1(128) - T_2(128)) / T_1(128) * 100)

plt.plot(n, T_1(n), 'r')
plt.plot(n, T_2(n), 'g')
plt.grid(True)
plt.show()
