import numpy as np

theta = 4
N = 128
B = 20
L = 90
M = 120


def write_array(data, name, M=8):
    data = data.reshape(-1, M)
    lines = 'static const int8_t {}[128] = {{\n'.format(name)
    content_str = '\t' + '{:d}, '*(M-1) + '{:d},\n'

    for points in data:
        lines += content_str.format(*points)

    lines = lines[:-2] + '\n'
    lines += '}'

    return lines


weights = np.linspace(-theta, theta, N).astype(np.int16)
ns = np.arange(0, N)
ideal_dist = np.piecewise(
    ns,
    [np.logical_and(ns > N/2 - B, ns < N/2 + B)],
    [1, 0]
)
p = np.vectorize(lambda t: 1 if t >= L and t < M else 0)
pulse = p(ns)

lines = write_array(weights, 'weights')
lines += '\n\n'
lines += write_array(ideal_dist, 'ideal_dist')
lines += '\n\n'
lines += write_array(pulse, 'signal')



with open('magic_numbers', 'w') as f:
    f.write(lines)
