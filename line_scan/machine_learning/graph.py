import numpy as np
import matplotlib.pyplot as plt

# define signal
pulse = lambda c, d: np.vectorize(
    lambda n: 0 if n >= c-(d/2) and n < c+(d/2) else 1
)

x = np.arange(128).astype(float)
y = pulse(64, 30)(x).astype(float)
y *= np.cos(np.pi * (x - (x.shape[0]/2)) / x.shape[0])
y += 0.8 * np.std(y) * (np.random.randn(*x.shape) + np.mean(y))

# Plot test
fig = plt.figure()
plt.plot(x, y)
plt.xlabel('Pixel')
plt.ylabel('Intensity')
plt.show()
