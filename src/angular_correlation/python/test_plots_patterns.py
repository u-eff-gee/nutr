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
os.chdir('@ANGCORR_PYTHON_DIR@')
from angular_correlation_plot import AngularCorrelationPlot, AngularCorrelationPlotGrid
import state as st
import transition as tr
from time import time

from pathlib import Path

file_name_prefix = 'test_plot_'
file_name_suffix = '.pdf'

ang_corr_grids = [
    [
        '01_polarization', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$0 \to 1 \to 0$', 
                    st.State(0, st.PARITY_UNKNOWN), 
                    [
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(2, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 0$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ],
                ),
                AngularCorrelationPlot(r'$0^+ \to 1^- \to 0$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 2, tr.MAGNETIC, 4, 0.), st.State(2, st.NEGATIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ],
                ),
            ],
            [
                AngularCorrelationPlot(r'$0 \to 2 \to 0$', 
                    st.State(0, st.PARITY_UNKNOWN), 
                    [
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(4, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 0$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 2^- \to 0$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 4, tr.ELECTRIC, 6, 0.), st.State(4, st.NEGATIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '02_mixing_single', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 1 ~~, ~~ \delta_2 = 0$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(2, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 1 ~~, ~~ \delta_2 \to \infty$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(2, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 1 ~~, ~~ \delta_2 = -1$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.), st.State(2, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 1 ~~, ~~ \delta_2 = +1$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(2, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '03_mixing_elastic', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$\frac{1}{2}^+ \to \frac{3}{2}^+ \to \frac{1}{2} ~~, ~~ \delta_1 = \delta_2 = 0$', 
                    st.State(1, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(3, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(1, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{1}{2}^+ \to \frac{3}{2}^+ \to \frac{1}{2} ~~, ~~ \delta_1 = \delta_2 \to \infty$', 
                    st.State(1, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 100.), st.State(3, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(1, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$\frac{1}{2}^+ \to \frac{3}{2}^+ \to \frac{1}{2} ~~, ~~ \delta_1 = \delta_2 = -1$', 
                    st.State(1, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, -1.), st.State(3, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.), st.State(1, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{1}{2}^+ \to \frac{3}{2}^+ \to \frac{1}{2} ~~, ~~ \delta_1 = \delta_2 =-1$', 
                    st.State(1, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 1.), st.State(3, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(1, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '04_mixing_inelastic', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$\frac{3}{2}^+ \to \frac{7}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = 2.3, \delta_2 = 1.3$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 2.3), st.State(7, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.3), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{3}{2}^+ \to \frac{7}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = 2.3, \delta_2 = -1.3$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 2.3), st.State(7, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.3), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$\frac{3}{2}^+ \to \frac{7}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = -2.3, \delta_2 = 1.3$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, -2.3), st.State(7, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.3), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{3}{2}^+ \to \frac{7}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = -2.3, \delta_2 = -1.3$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, -2.3), st.State(7, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.3), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '05_0p_1p_2_asy', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 2 ~~, ~~ \delta_2 = 0$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(4, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 2 ~~, ~~ \delta_2 \to \infty$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(4, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 2 ~~, ~~ \delta_2 = -1$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.), st.State(4, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 2 ~~, ~~ \delta_2 = +1$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(4, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '06_0p_1p_3', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 3 ~~, ~~ \delta_2 = 0$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(6, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 3 ~~, ~~ \delta_2 \to \infty$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 100.), st.State(6, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 3 ~~, ~~ \delta_2 = -1$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, -1.), st.State(6, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 3 ~~, ~~ \delta_2 = +1$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 1.), st.State(6, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '07_0p_2p_1', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 1 ~~, ~~ \delta_2 = 0$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(2, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 1 ~~, ~~ \delta_2 \to \infty$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(2, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 1 ~~, ~~ \delta_2 = -1$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.), st.State(2, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 1 ~~, ~~ \delta_2 = +1$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(2, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '08_0p_2p_2', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 2 ~~, ~~ \delta_2 = 0$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(4, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 2 ~~, ~~ \delta_2 \to \infty$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(4, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 2 ~~, ~~ \delta_2 = -1$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.), st.State(4, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 2 ~~, ~~ \delta_2 = +1$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(4, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '09_0p_2p_3_asy', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 3 ~~, ~~ \delta_2 = 0$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(6, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 3 ~~, ~~ \delta_2 \to \infty$',
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(6, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 3 ~~, ~~ \delta_2 = -1$',
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.), st.State(6, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 3 ~~, ~~ \delta_2 = +1$',
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(6, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '10_0p_2p_4', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 4 ~~, ~~ \delta_2 = 0$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(8, st.PARITY_UNKNOWN)]
                    ]
                ),
            AngularCorrelationPlot(r'$0^+ \to 2^+ \to 4 ~~, ~~ \delta_2 \to \infty$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 100.), st.State(8, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
            AngularCorrelationPlot(r'$0^+ \to 2^+ \to 4 ~~, ~~ \delta_2 = -1$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, -1.), st.State(8, st.PARITY_UNKNOWN)]
                    ]
                ),
            AngularCorrelationPlot(r'$0^+ \to 2^+ \to 4 ~~, ~~ \delta_2 \to \infty$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 1.), st.State(8, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '11_0.5p_2.5p_0.5', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$\frac{1}{2}^+ \to \frac{5}{2}^+ \to \frac{1}{2} ~~, ~~ \delta_1 = \delta_2 = 0$', 
                    st.State(1, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(5, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(1, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{1}{2}^+ \to \frac{5}{2}^+ \to \frac{1}{2} ~~, ~~ \delta_1 = \delta_2 \to \infty$', 
                    st.State(1, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 100.), st.State(5, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 100.), st.State(1, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$\frac{1}{2}^+ \to \frac{5}{2}^+ \to \frac{1}{2} ~~, ~~ \delta_1 = \delta_2 = -1$', 
                    st.State(1, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, -1.), st.State(5, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, -1.), st.State(1, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{1}{2}^+ \to \frac{5}{2}^+ \to \frac{1}{2} ~~, ~~ \delta_1 = \delta_2 = 1$', 
                    st.State(1, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 1.), st.State(5, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 1.), st.State(1, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '12_1.5p_1.5p_1.5p', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$\frac{1}{2}^+ \to \frac{1}{2}^+ \to \frac{1}{2} ~~, ~~ \delta_1 = \delta_2 = 0$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(3, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(3, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{1}{2}^+ \to \frac{1}{2}^+ \to \frac{1}{2} ~~, ~~ \delta_1 = \delta_2 \to \infty$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 100.), st.State(3, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(3, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$\frac{1}{2}^+ \to \frac{1}{2}^+ \to \frac{1}{2} ~~, ~~ \delta_1 = \delta_2 = -1$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, -1.), st.State(3, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.), st.State(3, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{1}{2}^+ \to \frac{1}{2}^+ \to \frac{1}{2} ~~, ~~ \delta_1 = \delta_2 = +1$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 1.), st.State(3, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(3, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '13_1.5p_2.5p_1.5p', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$\frac{3}{2}^+ \to \frac{5}{2}^+ \to \frac{3}{2} ~~, ~~ \delta_1 = \delta_2 = 0$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(5, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(3, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{3}{2}^+ \to \frac{5}{2}^+ \to \frac{3}{2} ~~, ~~ \delta_1 = \delta_2 \to \infty$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 100.), st.State(5, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(3, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$\frac{3}{2}^+ \to \frac{5}{2}^+ \to \frac{3}{2} ~~, ~~ \delta_1 = \delta_2 = -1$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, -1.), st.State(5, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.), st.State(3, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{3}{2}^+ \to \frac{5}{2}^+ \to \frac{3}{2} ~~, ~~ \delta_1 = \delta_2 = +1$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 1.), st.State(5, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(3, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '14_1.5p_3.5p_1.5p', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$\frac{3}{2}^+ \to \frac{7}{2}^+ \to \frac{3}{2} ~~, ~~ \delta_1 = \delta_2 = 0$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(7, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(3, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{3}{2}^+ \to \frac{7}{2}^+ \to \frac{3}{2} ~~, ~~ \delta_1 = \delta_2 \to \infty$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 100.), st.State(7, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 100.), st.State(3, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$\frac{3}{2}^+ \to \frac{7}{2}^+ \to \frac{3}{2} ~~, ~~ \delta_1 = \delta_2 = -1$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, -1.), st.State(7, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, -1.), st.State(3, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{3}{2}^+ \to \frac{7}{2}^+ \to \frac{3}{2} ~~, ~~ \delta_1 = \delta_2 = +1$', 
                    st.State(3, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 1.), st.State(7, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 1.), st.State(3, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])        
    ],
    [
        '15_2.5p_1.5p_2.5p', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{3}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 = 0$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(3, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{3}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 \to \infty$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 100.), st.State(3, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{3}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 = -1$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, -1.), st.State(3, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{3}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 = -1$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 1.), st.State(3, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '16_2.5p_2.5p_2.5p', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{5}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 = 0$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(5, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{5}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 \to \infty$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 100.), st.State(5, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{5}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 = -1$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, -1.), st.State(5, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{5}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 = -1$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 1.), st.State(5, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '17_2.5p_3.5p_2.5p', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{7}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 = 0$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(7, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{7}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 \to \infty$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 100.), st.State(7, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{7}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 = -1$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, -1.), st.State(7, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, -1.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{7}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 = -1$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 1.), st.State(7, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '18_2.5p_4.5p_2.5p', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{9}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 = 0$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(9, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{9}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 \to \infty$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 100.), st.State(9, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 100.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{9}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 = -1$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, -1.), st.State(9, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, -1.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$\frac{5}{2}^+ \to \frac{9}{2}^+ \to \frac{5}{2} ~~, ~~ \delta_1 = \delta_2 = +1$', 
                    st.State(5, st.POSITIVE), 
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 1.), st.State(9, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 1.), st.State(5, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '19_0_1_1_0', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 1 \to 0 ~~ , ~~ \delta_2 = 0$', 
                    st.State(0, st.POSITIVE),
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(2, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 1 \to 0 ~~ , ~~ \delta_2 = 1$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(2, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 1^+ \to 1 \to 0 ~~ , ~~ \delta_2 \to \infty$', 
                    st.State(0, st.POSITIVE), 
                    [
                        [tr.Transition(tr.MAGNETIC, 2, tr.ELECTRIC, 4, 0.), st.State(2, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(2, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$0 \to 1 \to 1 \to 0 ~~ , ~~ \delta_2 = 0$', 
                    st.State(0, st.PARITY_UNKNOWN), 
                    [
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(2, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(2, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0 \to 1 \to 1 \to 0 ~~ , ~~ \delta_2 = 1$', 
                    st.State(0, st.PARITY_UNKNOWN), 
                    [
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(2, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(2, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0 \to 1 \to 1 \to 0 ~~ , ~~ \delta_2 \to \infty$', 
                    st.State(0, st.PARITY_UNKNOWN), 
                    [
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(2, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(2, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ],
    [
        '20_0_2_2_0', AngularCorrelationPlotGrid([
            [
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 2 \to 0 ~~ , ~~ \delta_2 = 0$', 
                    st.State(0, st.POSITIVE),
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(4, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 2 \to 0 ~~ , ~~ \delta_2 = 1$', 
                    st.State(0, st.POSITIVE),
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(4, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0^+ \to 2^+ \to 2 \to 0 ~~ , ~~ \delta_2 \to \infty$', 
                    st.State(0, st.POSITIVE),
                    [
                        [tr.Transition(tr.ELECTRIC, 4, tr.MAGNETIC, 6, 0.), st.State(4, st.POSITIVE)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(4, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
            [
                AngularCorrelationPlot(r'$0 \to 2 \to 2 \to 0 ~~ , ~~ \delta_2 = 0$', 
                    st.State(0, st.PARITY_UNKNOWN), 
                    [
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(4, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 0.), st.State(4, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0 \to 2 \to 2 \to 0 ~~ , ~~ \delta_2 = 1$', 
                    st.State(0, st.PARITY_UNKNOWN), 
                    [
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(4, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 1.), st.State(4, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
                AngularCorrelationPlot(r'$0 \to 2 \to 2 \to 0 ~~ , ~~ \delta_2 \to \infty$', 
                    st.State(0, st.PARITY_UNKNOWN), 
                    [
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(4, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 2, tr.EM_UNKNOWN, 4, 100.), st.State(4, st.PARITY_UNKNOWN)],
                        [tr.Transition(tr.EM_UNKNOWN, 4, tr.EM_UNKNOWN, 6, 0.), st.State(0, st.PARITY_UNKNOWN)]
                    ]
                ),
            ],
        ])
    ]
]

output_file_name = ''

start = time()

for ang_corr_grid in ang_corr_grids:
    print('{:6.1f} seconds'.format(time()-start), end=' : ')
    output_file_name = Path('@PROJECT_BINARY_DIR@') / (file_name_prefix + ang_corr_grid[0] + file_name_suffix)
    if output_file_name.exists():
        print('File \'{}\' already exists. Remove it to create a new version.'.format(output_file_name))
    else:
        print('Creating \'{}\' ...'.format(output_file_name))
        ang_corr_grid[1].plot(output_file_name)

print('Execution took {:6.1f} seconds.'.format(time()-start))