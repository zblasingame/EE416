import numpy as np
import matplotlib.pyplot as plt

# code to test algorithm
n = np.arange(128)
start = 0
stop = 10

traces = {}

pulse = lambda L, M: np.vectorize(lambda n: 1 if n >= L and n < M else 0)

ideal_dist = pulse(44, 84)(n)
traces['ideal_dist'] = ideal_dist
ideal_dist = pulse(44, 84)(np.arange(1000))

for i in range(118):

    measured_dist = pulse(start + i, stop + i)(n)
    traces['measured_dist'] = measured_dist

    correlation = np.vectorize(lambda n: np.sum(
        [measured_dist[m] * ideal_dist[m + n] for m in range(128)]
    ))
    traces['correlation'] = correlation(n)


    plt.plot(n, traces['ideal_dist'], 'g')
    plt.plot(n, traces['measured_dist'], 'b')
    plt.plot(n, traces['correlation'], 'r')
    plt.title('Correlation')
    plt.grid(True)
    plt.xlabel('n')
    plt.savefig('graphs/out-{:04d}.png'.format(i))
    plt.close()
