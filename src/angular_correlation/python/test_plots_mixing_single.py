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

import os
from pathlib import Path
from time import time

from asymmetry_plot import AngularCorrelation, AsymmetryPlotter
import state as st
import transition as tr

_overwrite_existing = True

os.chdir('@ANGCORR_PYTHON_DIR@')
file_name_prefix = 'test_plot_'
file_name_2d_suffix = '_2d.pdf'
file_name_2d_exp_suffix = '_2d_exp.pdf'
file_name_3d_suffix = '_3d.pdf'

def create_delta_labels(angular_correlation):
    delta_labels = []
    n_cascade_steps = len(angular_correlation.cascade_steps)
    variable_delta_label = None

    for i, delta in enumerate(angular_correlation.delta_dict):
        if delta == 'delta_1':
            if i == 0 or i == n_cascade_steps - 1:
                delta_labels.append(r'$\delta_' + str(i+1) + '$' if variable_delta_label is None else variable_delta_label)
            else:
                delta_labels.append(r'$\delta_u$' if variable_delta_label is None else variable_delta_label)
            variable_delta_label = delta_labels[-1]
        else:
            delta_labels.append('')

    return (variable_delta_label, delta_labels)

ang_cors = [
    [
        '02_mixing_single', AngularCorrelation(
            st.State(0, st.POSITIVE),
            [
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(2, st.PARITY_UNKNOWN)],
            ],
            [0., 'delta_1']
        ),
        False, [-0.5, 0.1, 0.1], [-0.8, 0.2, 0.2], 
    ],
    [
        '03_mixing_elastic', AngularCorrelation(
            st.State(1, st.POSITIVE),
            [
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(3, st.POSITIVE)],
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(1, st.POSITIVE)],
            ],
            ['delta_1', 'delta_1']
        ),
        True, [0.1, 0.1, 0.1], [0.3, 0.1, 0.1],         
    ],
    [
        '05_0p_1p_2_asy', AngularCorrelation(
            st.State(0, st.POSITIVE),
            [
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(4, st.PARITY_UNKNOWN)],
            ],
            [0., 'delta_1']
        ),
        False, [0.2, 0.1, 0.1], [0.6, 0.1, 0.1],         
    ],
    [
        '06_0p_1p_3_asy', AngularCorrelation(
            st.State(0, st.POSITIVE),
            [
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(6, st.PARITY_UNKNOWN)],
            ],
            [0., 'delta_1']
        ),
        False, [0.2, 0.1, 0.1], [0.6, 0.1, 0.1],         
    ],
    [
        '07_0p_2p_1_asy', AngularCorrelation(
            st.State(0, st.POSITIVE),
            [
                [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(2, st.PARITY_UNKNOWN)],
            ],
            [0., 'delta_1']
        ),
        False, [0.2, 0.1, 0.1], [0.2, 0.1, 0.1],         
    ],
    [
        '08_0p_2p_2_asy', AngularCorrelation(
            st.State(0, st.POSITIVE),
            [
                [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(4, st.PARITY_UNKNOWN)],
            ],
            [0., 'delta_1']
        ),
        False, [0.1, 0.1, 0.1], [0.2, 0.1, 0.1],         
    ],
    [
        '09_0p_2p_3_asy', AngularCorrelation(
            st.State(0, st.POSITIVE),
            [
                [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(6, st.PARITY_UNKNOWN)],
            ],
            [0., 'delta_1']
        ),
        False, [0.1, 0.1, 0.1], [0.2, 0.1, 0.1],         
    ],
    [
        '10_0p_2p_4_asy', AngularCorrelation(
            st.State(0, st.POSITIVE),
            [
                [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(8, st.PARITY_UNKNOWN)],
            ],
            [0., 'delta_1']
        ),
        False, [0.1, 0.1, 0.1], [0.2, 0.1, 0.1],         
    ],
    [
        '11_0.5p_2.5p_0.5p_asy', AngularCorrelation(
            st.State(1, st.POSITIVE),
            [
                [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(5, st.POSITIVE)],
                [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(1, st.POSITIVE)],
            ],
            ['delta_1', 'delta_1']
        ),
        True, [0.3, 0.1, 0.1], [0.3, 0.1, 0.1],         
    ],
    [
        '12_1.5p_1.5p_1.5p_asy', AngularCorrelation(
            st.State(3, st.POSITIVE),
            [
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(3, st.POSITIVE)],
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(3, st.POSITIVE)],
            ],
            ['delta_1', 'delta_1']
        ),
        True, [-0.1, 0.05, 0.05], [-0.2, 0.05, 0.05],         
    ],
    [
        '13_1.5p_2.5p_1.5_asy', AngularCorrelation(
            st.State(3, st.POSITIVE),
            [
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(5, st.POSITIVE)],
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(3, st.POSITIVE)],
            ],
            ['delta_1', 'delta_1']
        ),
        True, [0.1, 0.05, 0.05], [0.3, 0.05, 0.05],         
    ],
    [
        '14_1.5p_3.5p_1.5_asy', AngularCorrelation(
            st.State(3, st.POSITIVE),
            [
                [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(7, st.POSITIVE)],
                [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(3, st.POSITIVE)],
            ],
            ['delta_1', 'delta_1']
        ),
        True, [0., 0.05, 0.05], [0.3, 0.05, 0.05],         
    ],
    [
        '15_2.5p_1.5p_2.5_asy', AngularCorrelation(
            st.State(5, st.POSITIVE),
            [
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(3, st.POSITIVE)],
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(5, st.POSITIVE)],
            ],
            ['delta_1', 'delta_1']
        ),
        True, [0., 0.05, 0.05], [0., 0.05, 0.05],         
    ],
    [
        '16_2.5p_2.5p_2.5_asy', AngularCorrelation(
            st.State(5, st.POSITIVE),
            [
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(5, st.POSITIVE)],
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(5, st.POSITIVE)],
            ],
            ['delta_1', 'delta_1']
        ),
        True, [-0.1, 0.05, 0.05], [0., 0.05, 0.05],         
    ],
    [
        '17_2.5p_3.5p_2.5_asy', AngularCorrelation(
            st.State(5, st.POSITIVE),
            [
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(7, st.POSITIVE)],
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(5, st.POSITIVE)],
            ],
            ['delta_1', 'delta_1']
        ),
        True, [0., 0.1, 0.1], [0., 0.1, 0.1],         
    ],
    [
        '18_2.5p_4.5p_2.5_asy', AngularCorrelation(
            st.State(5, st.POSITIVE),
            [
                [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(9, st.POSITIVE)],
                [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(5, st.POSITIVE)],
            ],
            ['delta_1', 'delta_1']
        ),
        True, [0.3, 0.1, 0.1], [0.2, 0.1, 0.1],         
    ],
    [
        '19_0_1_1_0_asy', AngularCorrelation(
            st.State(0, st.POSITIVE),
            [
                [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(2, st.PARITY_UNKNOWN)],
                [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(0, st.POSITIVE)]
            ],
            [0., 'delta_1', 0.]
        ),
        True, [0., 0.05, 0.05], [0., 0.05, 0.05],         
    ],
    [
        '20_0_2_2_0_asy', AngularCorrelation(
            st.State(0, st.POSITIVE),
            [
                [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(4, st.PARITY_UNKNOWN)],
                [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(0, st.POSITIVE)]
            ],
            [0., 'delta_1', 0.]
        ),
        True, [0.2, 0.05, 0.05], [-0.1, 0.05, 0.05],         
    ],
]

n_delta_steps = 101

t_start = time()

for ang_cor in ang_cors:
    
    arctan_deltas = None
    asy_plo = None
    variable_delta_label, delta_labels = create_delta_labels(ang_cor[1])

    print('{:6.1f} seconds'.format(time()-t_start), end=' : ')
    output_file_name_2d = Path('@PROJECT_BINARY_DIR@') / (file_name_prefix + ang_cor[0] + file_name_2d_suffix)

    if output_file_name_2d.exists() and not _overwrite_existing:
        print('File \'{}\' already exists. Remove it to create a new version.'.format(output_file_name_2d))
    else:
        print('Creating \'{}\' ...'.format(output_file_name_2d))

        if arctan_deltas is None:
            arctan_deltas, asy_45, asy_90 = ang_cor[1].asymmetry_grid(n_delta_steps=n_delta_steps)

        asy_plo = AsymmetryPlotter(
            ang_cor[1], arctan_deltas, asy_45, asy_90, scale_asymmetries=True,
            show_polarization=[True, False, False]
        )

        asy_plo.plot_single_2d(variable_delta_label, delta_labels, returns_to_initial_state=ang_cor[2], output_file=output_file_name_2d, transition_label_rotation=0)

    output_file_name_2d_exp = Path('@PROJECT_BINARY_DIR@') / (file_name_prefix + ang_cor[0] + file_name_2d_exp_suffix)

    print('{:6.1f} seconds'.format(time()-t_start), end=' : ')
    if output_file_name_2d_exp.exists() and not _overwrite_existing:
        print('File \'{}\' already exists. Remove it to create a new version.'.format(output_file_name_2d_exp))
    else:
        print('Creating \'{}\' ...'.format(output_file_name_2d_exp))

        if arctan_deltas is None:
            arctan_deltas, asy_45, asy_90 = ang_cor[1].asymmetry_grid(n_delta_steps=n_delta_steps)

        asy_plo = AsymmetryPlotter(
            ang_cor[1], arctan_deltas, asy_45, asy_90, scale_asymmetries=True,
            asy_45_exp=ang_cor[3],
            asy_90_exp=ang_cor[4], 
            show_polarization=[True, False, False]
        )

        asy_plo.plot_single_2d(variable_delta_label, delta_labels, returns_to_initial_state=ang_cor[2], output_file=output_file_name_2d_exp, transition_label_rotation=0)

    output_file_name_3d = Path('@PROJECT_BINARY_DIR@') / (file_name_prefix + ang_cor[0] + file_name_3d_suffix)

    print('{:6.1f} seconds'.format(time()-t_start), end=' : ')
    if output_file_name_3d.exists() and not _overwrite_existing:
        print('File \'{}\' already exists. Remove it to create a new version.'.format(output_file_name_3d))
    else:
        print('Creating \'{}\' ...'.format(output_file_name_3d))

        if arctan_deltas is None:
            arctan_deltas, asy_45, asy_90 = ang_cor[1].asymmetry_grid(n_delta_steps=n_delta_steps)

        if asy_plo is None:
            asy_plo = AsymmetryPlotter(
                ang_cor[1], arctan_deltas, asy_45, asy_90, scale_asymmetries=True,
                show_polarization=[True, False, False]
            )

        asy_plo.plot_single_3d(r'$\delta_2$', output_file_name_3d)

print('Execution took {:6.1f} seconds.'.format(time()-t_start))