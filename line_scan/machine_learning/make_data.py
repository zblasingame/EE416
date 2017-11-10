import numpy as np
import csv

# define signal
pulse = lambda c, d: np.vectorize(
    lambda n: 0 if n >= c-(d/2) and n < c+(d/2) else 1
)

data = []

x = np.arange(128).astype(float)

for i in range(500):
    for centre in x:
        y = 0xFFFF * pulse(centre, 30)(x).astype(float)
        y *= np.cos(np.pi * (x - (x.shape[0]/2)) / x.shape[0])
        y += 0.8 * np.std(y) * (np.random.randn(*x.shape) + np.mean(y))

        data.append(np.append([centre], y, axis=0))

with open('data.csv', 'w') as f:
    csv.writer(f).writerows(data)
