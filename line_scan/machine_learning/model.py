"""Implementation of Linear Regression in Numpy

Author:         Zander Blasingame
Institution:    Clarkson University
Lab:            CAMEL
"""

import numpy as np
from sklearn.metrics import confusion_matrix

import datasets as ds


class LR:
    """Linear Regression for Anomaly Detection

    Args:
       num_features (int): Number of features for the classifier.
        normalize (str = 'rescaling'): Normalization mode
            from the set {'rescaling', 'vector_norm', 'none'}
        reg_param (float = 1e-8): Regularization parameter.
    """

    def __init__(self, num_features, **kwargs):
        defaults = {
            'normalize': 'rescaling',
            'reg_param': 1e-8
        }

        self.num_features = num_features
        vars(self).update({p: kwargs.get(p, d) for p, d in defaults.items()})

    def train(self, X, Y):
        """Train LR model given benign samples.

        Args:
            X (np.array): Training matrix of shape (samples, features)
        """

        X = X.astype(np.float64)

        # normalize X
        if self.normalize == 'rescaling':
            self.min = X.min(axis=0)
            self.max = X.max(axis=0)
            X = ds.rescale(X, self.min, self.max, 0, 1)

        # add bias vector to all samples
        X = np.concatenate((X, np.ones((X.shape[0], 1))), axis=1)

        xTx = X.T.dot(X)
        inv = np.linalg.inv(xTx + self.reg_param * np.eye(self.num_features+1))
        self.theta = inv.dot(X.T).dot(Y)

        return self.theta


    def test(self, X, Y):
        """Evaluate model performance.

        Args:
            X (np.array): Testing features.
            Y (np.array): Testing labels.
        """

        X = X.astype(np.float64)

        # normalize X
        if self.normalize == 'rescaling':
            X = ds.rescale(X, self.min, self.max, 0, 1)

        # add bias vector to all samples
        X = np.concatenate((X, np.ones((X.shape[0], 1))), axis=1)

        scores = np.dot(self.theta, X.T)

        return np.mean(scores - Y)
