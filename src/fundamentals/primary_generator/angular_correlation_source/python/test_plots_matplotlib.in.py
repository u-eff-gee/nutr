# This file is part of nutr.

# nutr is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# nutr is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with nutr.  If not, see <https://www.gnu.org/licenses/>.

# Copyright (C) 2020 Udo Friman-Gayer

import matplotlib.pyplot as plt
import matplotlib as mpl
import numpy as np

import os
os.chdir('@PROJECT_BINARY_DIR@/src/fundamentals/primary_generator/angular_correlation_source/python/')
from angular_correlation import angular_correlation
from state import *
from transition import *

from pathlib import Path

class AngularDistributionPlot:
    def __init__(self, title, filename, ini_sta, cas_ste):
        self.title=title
        self.filename=filename
        self.ini_sta=ini_sta
        self.cas_ste=cas_ste

OUTPUT_DIR = Path('@PROJECT_BINARY_DIR@')

AXIS_BEAM_LABEL = 'Beam direction'
AXIS_POLARIZATION_LABEL = 'Polarization axis'
AXIS_LAST_LABEL = ''
AXES_LABELS = {0: AXIS_BEAM_LABEL, 1: AXIS_POLARIZATION_LABEL, 2: AXIS_LAST_LABEL}
AXES_INDICES = {label: index for index, label in AXES_LABELS.items()}

XLIM = (-2., 2.)
YLIM = (-2., 2.)
ZLIM = (-2., 2.)

# A matplotlib color map maps scalar values in a range [n_0, n_1] to colors in a range [c_0, c_1].
# Here, it is assumed that n_0 = 0.
# If several angular distributions are visualized in different panels, it may be beneficial to 
# use the same mapping for all of them. 
# Having the same magnitude-color mapping in each panel means the magnitude can be compared more
# easily.
# On the other hand, if the magnitude of a particular angular distribution is much smaller 
# than n_1, its visualization will not make use of the entire color map and the contrast may be 
# too low.
# The parameter COLOR_MAP_MAX sets the value of n_1 to a nonzero positive value, which will 
# be the upper limit for all visualizations.
# If COLOR_MAP_MAX is set to a negative value or zero, n_1 will be set to the maximum value
# for each specific angular distribution, which is the most efficient use of the color map.
# 
# In this context, also note the importance of the choice of a color scheme.
# While the 'rainbow' or 'jet' color schemes of matplotlib give appealing and colorful,
# visualizations, they have been critized for not representing magnitudes well for 
# human perception.
# For a discussion of this issue, see
# 
# https://matplotlib.org/3.2.1/tutorials/colors/colormaps.html
#
# and references therein.
# According to these considerations, this script will also use different color maps for the two 
# fundamentally different settings of COLOR_MAP_MAX.
COLOR_MAP_MAX = -1.

N_THETA = 100
N_PHI   = 100

theta, phi = np.meshgrid(np.linspace(0., np.pi, N_THETA),
                         np.linspace(0., 2.*np.pi, N_PHI))

ang_corr = np.zeros(np.shape(theta))
x_y_z = np.zeros((3, np.shape(theta)[0], np.shape(theta)[1]))

angular_distributions = [
    AngularDistributionPlot(r'$0 \to 1 \to 0$', '0_1_0.pdf', 
    State(0, PARITY_UNKNOWN), 
    [
        [Transition(EM_UNKNOWN, 2, EM_UNKNOWN, 4, 0.), State(2, PARITY_UNKNOWN)],
        [Transition(EM_UNKNOWN, 2, EM_UNKNOWN, 4, 0.), State(0, PARITY_UNKNOWN)]
    ]),
    AngularDistributionPlot(r'$0 \to 2 \to 0$', '0_2_0.pdf', 
    State(0, PARITY_UNKNOWN), 
    [
        [Transition(EM_UNKNOWN, 4, EM_UNKNOWN, 6, 0.), State(4, PARITY_UNKNOWN)],
        [Transition(EM_UNKNOWN, 4, EM_UNKNOWN, 6, 0.), State(0, PARITY_UNKNOWN)]
    ]),

    AngularDistributionPlot(r'$0^+ \to 1^+ \to 0$', '0p_1p_0.pdf', 
    State(0, POSITIVE), 
    [
        [Transition(MAGNETIC, 2, ELECTRIC, 4, 0.), State(2, POSITIVE)],
        [Transition(EM_UNKNOWN, 2, EM_UNKNOWN, 4, 0.), State(0, PARITY_UNKNOWN)]
    ]),
    AngularDistributionPlot(r'$0^+ \to 1^- \to 0$', '0p_1m_0.pdf', 
    State(0, POSITIVE), 
    [
        [Transition(ELECTRIC, 2, MAGNETIC, 4, 0.), State(2, NEGATIVE)],
        [Transition(EM_UNKNOWN, 2, EM_UNKNOWN, 4, 0.), State(0, PARITY_UNKNOWN)]
    ]),
    AngularDistributionPlot(r'$0^+ \to 2^+ \to 0$', '0p_2p_0.pdf', 
    State(0, POSITIVE), 
    [
        [Transition(ELECTRIC, 4, MAGNETIC, 6, 0.), State(4, POSITIVE)],
        [Transition(EM_UNKNOWN, 4, EM_UNKNOWN, 6, 0.), State(0, PARITY_UNKNOWN)]
    ]),
    AngularDistributionPlot(r'$0^+ \to 2^- \to 0$', '0p_2m_0.pdf', 
    State(0, POSITIVE), 
    [
        [Transition(MAGNETIC, 4, ELECTRIC, 6, 0.), State(4, NEGATIVE)],
        [Transition(EM_UNKNOWN, 4, EM_UNKNOWN, 6, 0.), State(0, PARITY_UNKNOWN)]
    ]),
]

for ang_dist_plot in angular_distributions:
    output_file_path = OUTPUT_DIR / ang_dist_plot.filename
    if os.path.isfile(output_file_path):
        print('File {} already exists. Skipping calculation ...'.format(output_file_path))
        continue

    for i in range(N_THETA):
        for j in range(N_PHI):

            ang_corr[i][j] = angular_correlation(theta[i][j], phi[i][j], ang_dist_plot.ini_sta, ang_dist_plot.cas_ste)            

            x_y_z[AXES_INDICES[AXIS_POLARIZATION_LABEL]][i][j] = ang_corr[i][j]*np.sin(theta[i][j])*np.cos(phi[i][j])
            x_y_z[AXES_INDICES[AXIS_LAST_LABEL]][i][j] = ang_corr[i][j]*np.sin(theta[i][j])*np.sin(phi[i][j])
            x_y_z[AXES_INDICES[AXIS_BEAM_LABEL]][i][j] = ang_corr[i][j]*np.cos(theta[i][j])

    if COLOR_MAP_MAX > 0.:
        color_map_max = COLOR_MAP_MAX
        color_map_norm = ang_corr/COLOR_MAP_MAX
        color_map = mpl.cm.rainbow(color_map_norm)
    else:
        color_map_max = np.max(ang_corr)
        color_map_norm = ang_corr/color_map_max
        color_map = mpl.cm.inferno(color_map_norm)

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_title(ang_dist_plot.title)
    ax.set_xlabel(AXES_LABELS[0])
    ax.set_xlim(XLIM)
    ax.set_ylabel(AXES_LABELS[1])
    ax.set_ylim(YLIM)
    ax.set_zlabel(AXES_LABELS[2])
    ax.set_zlim(ZLIM)

    ax.plot_surface(x_y_z[0], x_y_z[1], x_y_z[2], facecolors=color_map)

    fig.colorbar(mpl.cm.ScalarMappable(norm=mpl.colors.Normalize(vmin=0., vmax=color_map_max), cmap=(plt.get_cmap('rainbow') if COLOR_MAP_MAX > 0. else plt.get_cmap('inferno'))), shrink=0.8)

    plt.savefig(output_file_path)
    print('Created output file {} for angular distribution {}'.format(output_file_path, ang_dist_plot.title))
    plt.close(fig)