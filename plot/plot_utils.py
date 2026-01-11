###############################################################################
# Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    #
#                                                                             #
# (C) Copyright 2024, Phil Underwood. All rights reserved.                    #
#                                                                             #
# This software is licensed under the terms of the 3-Clause BSD License       #
# which can be obtained from https://opensource.org/license/bsd-3-clause/.    #
###############################################################################

import numpy as np
import matplotlib.pyplot as plt

def area_like_matlab(layers, labels=None, ax=None, x=None, **kwargs):
    """
    Plot stacked areas like MATLAB's area(), guaranteed to show all layers.
    """
    if len(layers) == 0:
        raise ValueError("No layers provided to plot.")
    
    layers = [np.asarray(l) for l in layers]
    n_points = len(layers[0])
    for l in layers:
        if len(l) != n_points:
            raise ValueError("All layers must have the same length.")
    
    if x is None:
        x = np.arange(n_points)
    else:
        x = np.asarray(x)
        if len(x) != n_points:
            raise ValueError("x must have the same length as layers.")
    
    if ax is None:
        fig, ax = plt.subplots()
    
    cum = np.zeros_like(layers[0])
    polys = []
    for i, layer in enumerate(layers):
        lower = cum
        upper = cum + layer
        poly = ax.fill_between(x, lower, upper, **kwargs)
        polys.append(poly)
        cum = upper
    
    if labels is not None:
        if len(labels) != len(layers):
            raise ValueError("Number of labels must match number of layers.")
        ax.legend(polys, labels, loc='upper left')
    
    return ax, polys