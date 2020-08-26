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

import matplotlib.pyplot as plt
import numpy as np

class LevelSchemePlotter:
    def __init__(self, axis, initial_state, cascade_steps, delta_labels, returns_to_initial_state=False, show_polarization=None, fontsize=12, state_line_width=2, arrow_width=2, offset=(0, 0)):
        self.ax = axis
        self.min_x, self.max_x = axis.get_xlim()
        self.range_x = self.max_x - self.min_x
        self.min_y, self.max_y = axis.get_ylim()
        self.range_y = self.max_y - self.min_y
        self.ini_sta = initial_state
        self.cas_ste = cascade_steps
        self.del_lab = delta_labels
        self.returns_to_initial_state = returns_to_initial_state
        self.show_polarization = [False]*len(cascade_steps)
        if show_polarization is not None:
            self.show_polarization = show_polarization

        ## Parameters for the plot
        # Fonts
        self.fontsize = fontsize
        self.fontsize_multipoles = 1.2*fontsize

        # State lines
        self.state_line_width=state_line_width
        self.state_x = 0.4*self.range_x + self.min_x + offset[0]*self.range_x
        self.state_width = 0.4*self.range_x
        self.intermediate_state_x = 0.55*self.range_x + self.min_x + offset[0]*self.range_x
        self.intermediate_state_width = 0.25*self.range_x

        self.initial_state_y = 0.2*self.range_y + self.min_y + offset[1]*self.range_y
        self.excited_state_y = 0.8*self.range_y + self.min_y + offset[1]*self.range_y

        # State labels
        self.state_label_left_x = 0.25*self.range_x + self.min_x + offset[0]*self.range_x
        self.state_label_right_x = 0.9*self.range_x + self.min_x + offset[0]*self.range_x

        # Transition arrows
        self.arrow_width = arrow_width
        self.excitation_arrow_x = 0.5*self.range_x + self.min_x + offset[0]*self.range_x
        self.decay_arrow_x = 0.7*self.range_x + self.min_x + offset[0]*self.range_x
        self.arrow_head_length = 0.04*self.range_y
        self.arrow_head_width = 0.03*self.arrow_width
        self.excitation_arrow_color = 'blue'
        self.decay_arrow_color = 'red'

        # Transition labels
        self.excitation_label_left_x = 0.18*self.range_x + self.min_x + offset[0]*self.range_x
        self.decay_label_right_x = 0.85*self.range_x + self.min_x + offset[0]*self.range_x

        # Multipole mixing ratio (delta) labels
        self.delta_label_left_x = 0.4*self.range_x + self.min_x + offset[0]*self.range_x
        self.delta_label_right_x = 0.74*self.range_x + self.min_x + offset[0]*self.range_x

        # Order of drawing
        self.zorder_states = 0
        self.zorder_arrows = 1

    def plot(self):
        # Initial and excited state
        self.ax.plot([self.state_x, self.state_x + self.state_width],
                     [self.initial_state_y]*2, color='black', linewidth=self.state_line_width, zorder=self.zorder_states)
        self.ax.text(self.state_label_left_x, self.initial_state_y, self.ini_sta.tex(),
                     verticalalignment='center', fontsize=self.fontsize)
        self.ax.plot([self.state_x, self.state_x + self.state_width],
                     [self.excited_state_y]*2, color='black', linewidth=self.state_line_width, zorder=self.zorder_states)
        self.ax.text(self.state_label_left_x, self.excited_state_y, self.cas_ste[0][1].tex(),
                     verticalalignment='center', fontsize=self.fontsize)

        # Excitation
        self.ax.plot([self.excitation_arrow_x]*2, [self.initial_state_y, self.excited_state_y - self.arrow_head_length], '-', linewidth=self.arrow_width, color=self.excitation_arrow_color, zorder=self.zorder_arrows)
        self.ax.arrow(self.excitation_arrow_x, self.excited_state_y-self.arrow_head_length, 0., 1.e-5*self.range_y,
                      head_length=self.arrow_head_length,
                      head_width=self.arrow_head_width,
                      facecolor=self.excitation_arrow_color, edgecolor=self.excitation_arrow_color, zorder=self.zorder_arrows)
        self.ax.text(self.excitation_label_left_x,
                     0.5*(self.excited_state_y-self.initial_state_y)+self.initial_state_y,
                     self.cas_ste[0][0].tex(always_show_secondary=False, show_polarization=self.show_polarization[0]),
                     verticalalignment='center', fontsize=self.fontsize_multipoles)
        self.ax.text(self.delta_label_left_x,
                     0.5*(self.excited_state_y-self.initial_state_y)+self.initial_state_y,
                     self.del_lab[0],
                     verticalalignment='center', fontsize=self.fontsize)

        # Calculate position of states in decay cascade
        n_decay_steps = len(self.cas_ste)-1
        excitation_delta_y = self.excited_state_y - self.initial_state_y
        cascade_states_y = np.arange(1, n_decay_steps+1)

        if self.returns_to_initial_state:
            cascade_states_y = self.excited_state_y - cascade_states_y*excitation_delta_y/(n_decay_steps)
        else:
            cascade_states_y = self.excited_state_y - cascade_states_y*excitation_delta_y/(n_decay_steps+1)

        # States in cascade
        for i in range(n_decay_steps if not self.returns_to_initial_state else n_decay_steps-1):
            self.ax.plot([self.intermediate_state_x, self.intermediate_state_x + self.intermediate_state_width],
                         [cascade_states_y[i]]*2, color='black', linewidth=self.state_line_width, zorder=self.zorder_states)            
            self.ax.text(self.state_label_right_x,
                         cascade_states_y[i], self.cas_ste[i+1][1].tex(),
                         verticalalignment='center', fontsize=self.fontsize)
            
        # First transition in cascade
        self.ax.plot([self.decay_arrow_x]*2, [self.excited_state_y, cascade_states_y[0] + self.arrow_head_length],
                  '--' if n_decay_steps > 1 else '-', linewidth=self.arrow_width, color=self.decay_arrow_color, zorder=self.zorder_arrows)
        self.ax.arrow(self.decay_arrow_x, cascade_states_y[0]+self.arrow_head_length,
                      0., -1.e-5*self.range_y, 
                      head_length=self.arrow_head_length,
                      head_width=self.arrow_head_width,
                      color=self.decay_arrow_color, zorder=self.zorder_arrows)
        self.ax.text(self.decay_label_right_x,
                     0.5*(self.excited_state_y-cascade_states_y[0]) + cascade_states_y[0],
                     self.cas_ste[1][0].tex(show_polarization=self.show_polarization[1]),
                     verticalalignment='center', fontsize=self.fontsize_multipoles)
        self.ax.text(self.delta_label_right_x,
                     0.5*(self.excited_state_y-cascade_states_y[0]) + cascade_states_y[0],
                     self.del_lab[1],
                     verticalalignment='center', fontsize=self.fontsize)

        # Transitions in cascade
        for i in range(1, n_decay_steps):
            self.ax.plot([self.decay_arrow_x]*2, [cascade_states_y[i-1], cascade_states_y[i] + self.arrow_head_length],
                         '--' if i < n_decay_steps - 1 else '-', linewidth=self.arrow_width, color=self.decay_arrow_color, zorder=self.zorder_arrows)
            self.ax.arrow(self.decay_arrow_x, cascade_states_y[i]+self.arrow_head_length,
                      0., -1.e-5*self.range_y,
                      head_length=self.arrow_head_length,
                      head_width=self.arrow_head_width,
                      color=self.decay_arrow_color, zorder=self.zorder_arrows)
            self.ax.text(self.decay_label_right_x,
                         0.5*(cascade_states_y[i-1]-cascade_states_y[i]) + cascade_states_y[i],
                         self.cas_ste[i][0].tex(show_polarization=self.show_polarization[i]),
                         verticalalignment='center', fontsize=self.fontsize_multipoles)