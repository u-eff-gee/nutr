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

from pathlib import Path

ang_corr_grid = AngularCorrelationPlotGrid([
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

ang_corr_grid.plot(Path('@PROJECT_BINARY_DIR@') / 'test_plot_01_polarization.pdf')