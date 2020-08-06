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
    def __init__(self, axis, initial_state, cascade_steps, delta_labels, returns_to_initial_state=False):
        self.ax = axis
        self.min_x, self.max_x = axis.get_xlim()
        self.range_x = self.max_x - self.min_x
        self.min_y, self.max_y = axis.get_ylim()
        self.range_y = self.max_y - self.min_y
        self.ini_sta = initial_state
        self.cas_ste = cascade_steps
        self.del_lab = delta_labels
        self.returns_to_initial_state = returns_to_initial_state        

        ## Parameters for the plot
        # Fonts
        self.fontsize = 12

        # State lines
        self.state_x = 0.4*self.range_x + self.min_x
        self.state_width = 0.4*self.range_x
        self.intermediate_state_x = 0.55*self.range_x + self.min_x
        self.intermediate_state_width = 0.25*self.range_x

        self.initial_state_y = 0.2*self.range_y + self.min_y
        self.excited_state_y = 0.8*self.range_y + self.min_y

        # State labels
        self.state_label_left_x = 0.25*self.range_x + self.min_x
        self.state_label_right_x = 0.9*self.range_x + self.min_x

        # Transition arrows
        self.excitation_arrow_x = 0.5*self.range_x + self.min_x
        self.decay_arrow_x = 0.7*self.range_x + self.min_x
        self.arrow_head_length = 0.04*self.range_y
        self.arrow_head_width = 0.04*self.range_x

        # Transition labels
        self.excitation_label_left_x = 0.2*self.range_x + self.min_x
        self.decay_label_right_x = 0.85*self.range_x + self.min_x

        # Multipole mixing ratio (delta) labels
        self.delta_label_left_x = 0.4*self.range_x + self.min_x
        self.delta_label_right_x = 0.74*self.range_x + self.min_x

    def plot(self):
        # Initial and excited state
        self.ax.plot([self.state_x, self.state_x + self.state_width],
                     [self.initial_state_y]*2, color='black')
        self.ax.text(self.state_label_left_x, self.initial_state_y, self.ini_sta.tex(),
                     verticalalignment='center', fontsize=self.fontsize)
        self.ax.plot([self.state_x, self.state_x + self.state_width],
                     [self.excited_state_y]*2, color='black')
        self.ax.text(self.state_label_left_x, self.excited_state_y, self.cas_ste[0][1].tex(),
                     verticalalignment='center', fontsize=self.fontsize)

        # Excitation
        self.ax.arrow(self.excitation_arrow_x, self.initial_state_y, 0., 
                      self.excited_state_y-self.initial_state_y-self.arrow_head_length, width=0.01,
                      head_length=self.arrow_head_length,
                      head_width=self.arrow_head_width,
                      facecolor='blue', edgecolor='blue')
        self.ax.text(self.excitation_label_left_x,
                     0.5*(self.excited_state_y-self.initial_state_y)+self.initial_state_y,
                     self.cas_ste[0][0].tex(always_show_secondary=False),
                     verticalalignment='center', fontsize=self.fontsize)
        self.ax.text(self.delta_label_left_x,
                     0.5*(self.excited_state_y-self.initial_state_y)+self.initial_state_y,
                     self.del_lab[0],
                     verticalalignment='center', fontsize=self.fontsize, rotation=90)

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
                         [cascade_states_y[i]]*2, color='black')            
            self.ax.text(self.state_label_right_x,
                         cascade_states_y[i], self.cas_ste[i+1][1].tex(),
                         verticalalignment='center', fontsize=self.fontsize)
            
        # First transition in cascade
        self.ax.plot([self.decay_arrow_x]*2, [self.excited_state_y, cascade_states_y[0] + self.arrow_head_length],
                  '--' if n_decay_steps > 1 else '-', color='red')
        self.ax.arrow(self.decay_arrow_x, cascade_states_y[0]+self.arrow_head_length,
                      0., -1.e-5*self.range_y, 
                      head_length=self.arrow_head_length,
                      head_width=self.arrow_head_width,
                      color='red')
        self.ax.text(self.decay_label_right_x,
                     0.5*(self.excited_state_y-cascade_states_y[0]) + cascade_states_y[0],
                     self.cas_ste[1][0].tex(),
                     verticalalignment='center', fontsize=self.fontsize)
        self.ax.text(self.delta_label_right_x,
                     0.5*(self.excited_state_y-cascade_states_y[0]) + cascade_states_y[0],
                     self.del_lab[1],
                     verticalalignment='center', fontsize=self.fontsize, rotation=90)

        # Transitions in cascade
        for i in range(1, n_decay_steps):
            self.ax.plot([self.decay_arrow_x]*2, [cascade_states_y[i-1], cascade_states_y[i] + self.arrow_head_length],
                         '--' if i < n_decay_steps - 1 else '-', color='red')
            self.ax.arrow(self.decay_arrow_x, cascade_states_y[i]+self.arrow_head_length,
                      0., -1.e-5*self.range_y,
                      head_length=self.arrow_head_length,
                      head_width=self.arrow_head_width,
                      color='red')
            self.ax.text(self.decay_label_right_x,
                         0.5*(cascade_states_y[i-1]-cascade_states_y[i]) + cascade_states_y[i],
                         self.cas_ste[i][0].tex(),
                         verticalalignment='center', fontsize=self.fontsize)