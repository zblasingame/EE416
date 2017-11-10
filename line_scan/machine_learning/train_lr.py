from model import LR
import numpy as np
import csv

m = LR(128, normalize=False)

with open('data.csv', 'r') as f:
    data = np.array([row for row in csv.reader(f)]).astype(np.float)

x = data[:, 1:]
y = data[:, 0]

weights = m.train(x[:10000], y[:10000])
print(m.test(x[:-1000], y[:-1000]))

with open('weights', 'w') as f:
    f.write('const static double weights[129] = {\n')
    f.write(','.join([str(w) for w in weights]))
    f.write('\n}')
