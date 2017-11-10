import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter


# define algorithm
def metric(weights):
    return np.sum(
        x*p for x, p in enumerate(weights)
    ) / (np.sum(p for p in weights)+1E-9)


def lpf(x):
    y = np.zeros_like(x)
    N = 5

    for n in range(y.shape[0]):
        y[n] = np.sum([1/(N+1) * x[n-i] for i in range(min(n, N+1))])

    return y


def metric_lpf(x):
    y = np.zeros_like(x)

    # x1 = x
    x = lpf(x)

    for i in range(y.shape[0]):
        y[i] = 1 if x[i] >= np.max(x)/2 else 0

    # y = lpf(y)

    # plt.plot(np.arange(128), x1, 'r')
    # plt.plot(np.arange(128), y, 'b')
    # plt.show()

    return np.sum(
        i*p for i, p in enumerate(y)
    ) / (np.sum(p for p in y)+1e-9)


def metric_simple(weights):
    a = b = 0

    for i, w in enumerate(weights):
        w = 0 if w < 0.5 else 1
        if w == 1 and a == 0:
            a = i
        elif a > 0 and w == 0 and b == 0:
            b = i

    return (a+b)/2


# define signal
pulse = lambda c, d: np.vectorize(
    lambda n: 1 if n >= c-(d/2) and n < c+(d/2) else 0
)


def avg_deriv(data):
    return np.mean([data[i] - data[i-1] for i in range(1, len(data))])


def plot_err(f, name):
    widths = np.arange(5, 55, 5)
    centres = np.linspace(25, 103, 100)
    # centres = np.linspace(1, 128, 100)
    noise_strength = np.linspace(0, 1, 10)
    x = np.arange(128)
    errors = []
    avg_errors = []

    for n in noise_strength:
        error = []
        for c in centres:
            for w in widths:
                m = f(np.clip(pulse(c, w)(x) + np.random.normal(0, n, 128), 0, 1))
                error.append(abs(c-m)/c)

        errors.append(error)
        avg_errors.append(np.mean(error))

    print('Sensitivity of {} is {:.3f}'.format(
        name,
        avg_deriv(avg_errors)
    ))

    print('Avg Error {} is {:.3f}'.format(
        name,
        np.mean(errors)
    ))

    # Plot test
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.boxplot(errors, showfliers=False)
    ax.set_xticklabels(['{:.2f}'.format(n) for n in noise_strength])
    # ax.set_ylim([0, 200])
    plt.title('Performance of {}'.format(name))
    plt.xlabel('Noise Strength')
    plt.ylabel('Percent Error')
    plt.savefig('graphs/{}.png'.format(name.lower().replace(' ', '_')))
    plt.close()


plot_err(metric, 'Expectation Centre')
plot_err(metric_lpf, 'Expectation Centre with LPF')
plot_err(metric_simple, 'Simple Centre Alg')
