import tensorflow as tf
import numpy as np
import csv


X = tf.placeholder('float32', [None, 128], name='X')
Y = tf.placeholder('float32', [None], name='Y')

W = tf.get_variable(
    'W',
    [128, 1],
    initializer=tf.contrib.layers.xavier_initializer()
)

b = tf.get_variable(
    'b',
    [1],
    initializer=tf.contrib.layers.xavier_initializer()
)

Z = tf.nn.relu(tf.matmul(X, W) + b)

loss = tf.reduce_mean(tf.square(Y - Z))
solver = tf.train.RMSPropOptimizer(0.01).minimize(loss)

get_weights = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES)

init_op = tf.global_variables_initializer()

with open('data.csv', 'r') as f:
    data = np.array([row for row in csv.reader(f)]).astype(np.float)

with tf.Session() as sess:
    sess.run(init_op)

    x = data[:, 1:]
    x = (x - np.min(x)) / (np.max(x) - np.min(x))
    y = data[:, 0]

    for epoch in range(1000):
        for i in range(0, x.shape[0], 1000):
            # print('x')
            # print(np.atleast_2d(x[i:i+100]))
            # print('y')
            # print(np.atleast_1d(y[i:i+100]))

            _, l = sess.run([solver, loss], feed_dict={
                X: np.atleast_2d(x[i:i+1000]),
                Y: np.atleast_1d(y[i:i+1000])
            })

            print(l)

    print(sess.run(get_weights))
