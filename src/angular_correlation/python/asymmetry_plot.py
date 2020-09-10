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
import warnings
os.chdir('@ANGCORR_PYTHON_DIR@')

import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib import gridspec
from matplotlib.colors import ListedColormap
from matplotlib.patches import Rectangle
from mpl_toolkits.mplot3d import axes3d
from mpl_toolkits.axes_grid1.inset_locator import inset_axes
import numpy as np

from angular_correlation import angular_correlation
from level_scheme_plotter import LevelSchemePlotter

def values_in_range(array, mean, sigma_low, sigma_up):
    """Find values in an array that agree with an asymmetric confidence interval

Given a 'measurement' with a mean value 'mean' and an asymmetric probability distribution, whose 
confidence interval is characterized by a lower and an upper 'standard deviation' 'sigma_low' and 
'sigma_up', this function returns a boolean array indicating which elements of an array 'array' 
fulfil the inequalities:

array[i] >= mean - sigma_low
array[i] <= mean + sigma_up

Here, 'array[i]' is an arbitrary element of the array.

Parameters
----------
array: array
    Array of values
mean: float
    Mean value of the measurement
sigma_low, sigma_up: float
    Lower and upper standard deviation of the measurement

Returns
-------
array of bool:
    An array whose i-th entry indicates whether the i-th element of 'array' is contained in the
confidence interval.
    """
    return (array >= mean-sigma_low)*(array <= mean+sigma_up)

def find_true_intervals(array):
    """Find nonzero intervals in an array

Given an array that contains zeros and other numbers, for example

array = [0, 1, 1, 1, 0, 0, 1, 0, 0, 1],
         0  1  2  3  4  5  6  7  8  9

(the numbers below the array definition indicate the indices) this function returns the indices 
where contiguous sequences of nonzero entries begin and end.

For the array given above, it would return

[[1, 4], [6, 7], [9, 10]], 

i.e.

array[1:4]  == [1, 1, 1]
array[6:7]  == [1]
array[9:10] == [1].

Parameters
----------
array: array
    Array of numbers

Returns
-------
(n,2) array:
    Pairs of indices indicating the beginnings and endings of contiguous sequences of nonzero 
entries in the input array.
    """
    true_intervals = []

    last_entry = False # Remembers whether the last entry was True or False.
    interval_start = 0 # Remembers where the contiguous interval of nonzero numbers started.
    if array[0] > 0.:
        last_entry = True # Initialize last_entry for first element

    for i, ele in enumerate(array):
        if ele != last_entry: # Compare the current element to the last one. If they are not the 
                                # same, ...
            if ele:
                # ... and the element is true, then a new contiguous interval starts.
                interval_start = i
                last_entry = True
            else:
                # ... and the element is false, then the end of a contiguous interval was found.
                # Add this interval to the output list.
                true_intervals.append([interval_start, i])
                last_entry = False

    if last_entry: # Special case: nonzero interval continues until the end of the array.
        true_intervals.append([interval_start, len(array)])

    return true_intervals

def find_interval_overlaps(intervals1, intervals2):
    """Given two lists of index ranges, find their intersection

For example, given two lists

intervals1 = [[1, 3], [5, 9], [10, 12]]
intervals2 = [[0, 2], [6, 7]]

this function would return

[[1, 2], [6, 7]]

since the index ranges 1:2 and 6:7 are included in both lists.
    
Parameters
----------
intervals1, intervals2: (n,2) arrays
    Lists of pairs of indices indicating ranges in an array.

Returns
-------
(n,2) array:
    Pairs of indices that represent the intersection of intervals1 and intervals2
    """

    overlaps = []

    for i1 in intervals1:
        for i2 in intervals2:
            if i1[0] >= i2[0] and i1[0] <= i2[1]:
                if i1[1] <= i2[1]:
                    overlaps.append(i1)
                else:
                    overlaps.append([i1[0], i2[1]])
            
            elif i1[1] >= i2[0] and i1[1] <= i2[1]:
                overlaps.append([i2[0], i1[1]])

            elif i1[0] <= i2[0] and i1[1] >= i2[1]:
                overlaps.append(i2)

    return overlaps

class AngularCorrelation:
    def __init__(self, ini_sta, cas_ste, delta_dict):
        self.initial_state = ini_sta
        self.cascade_steps = cas_ste
        self.delta_dict = delta_dict
        self.n_variable_deltas = 0
	
        delta_1_found = False
        delta_2_found = False

        for cas_ste_delta in self.delta_dict:
            if cas_ste_delta == 'delta_1' and not delta_1_found: 
                delta_1_found = True
                self.n_variable_deltas += 1
            if cas_ste_delta == 'delta_2' and not delta_2_found: 
                delta_2_found = True
                self.n_variable_deltas += 1

    def asymmetry_grid(self, n_delta_steps=101, abs_delta_max=100.):
        arctan_deltas = np.linspace(
            np.arctan(-abs_delta_max),
            np.arctan( abs_delta_max),
            n_delta_steps)
        deltas = np.tan(arctan_deltas)

        asy_45 = np.zeros([n_delta_steps]*self.n_variable_deltas)
        asy_90 = np.zeros([n_delta_steps]*self.n_variable_deltas)

        if self.n_variable_deltas == 1:
            for i, delta in enumerate(deltas):
                self.set_deltas([delta])
                asy_45[i], asy_90[i] = self.calculate_asymmetry()

        else:
            for i, delta1 in enumerate(deltas):
                for j, delta2 in enumerate(deltas):
                    self.set_deltas([delta1, delta2])
                    asy_45[i][j], asy_90[i][j] = self.calculate_asymmetry()
        
        return (arctan_deltas, asy_45, asy_90)
        
    def set_deltas(self, deltas):
        for i, cas_ste_delta in enumerate(self.delta_dict):
            if isinstance(cas_ste_delta, (int, float)):
                self.cascade_steps[i][0].delta = cas_ste_delta
            if cas_ste_delta == 'delta_1':
                self.cascade_steps[i][0].delta = deltas[0]
            if cas_ste_delta == 'delta_2':
                self.cascade_steps[i][0].delta = deltas[1]
            
    def calculate_asymmetry(self):
        w_45_0  = angular_correlation(0.25*np.pi, 0.       , self.initial_state, self.cascade_steps)
        w_45_90 = angular_correlation(0.25*np.pi, 0.5*np.pi, self.initial_state, self.cascade_steps)
        w_90_0  = angular_correlation(0.50*np.pi, 0.       , self.initial_state, self.cascade_steps)
        w_90_90 = angular_correlation(0.50*np.pi, 0.5*np.pi, self.initial_state, self.cascade_steps)
        
        return ((w_45_0 - w_45_90)/(w_45_0 + w_45_90),
                (w_90_0 - w_90_90)/(w_90_0 + w_90_90))

class AsymmetryPlotter:
    def __init__(self, ang_cor, arctan_deltas, asy_45, asy_90, scale_asymmetries=True, asy_45_exp=None, asy_90_exp=None, show_polarization=None):

        ## Input data
        self.ang_cor = ang_cor
        self.arctan_deltas = arctan_deltas
        self.asy_45 = asy_45
        self.asy_90 = asy_90

        ## Determines the plot range for asymmetries.
        # If scale_asymmetries is False, they will be plotted in the range [-1, 1],
        # otherwise in the range [min(A(δ)), max(A(δ))].
        self.scale_asymmetries = scale_asymmetries

        self.asy_45_exp = asy_45_exp
        self.asy_90_exp = asy_90_exp

        self.show_polarization = show_polarization

        # This factor can be used to create a margin around the plotted data.
        # If the data are in the range [a, b], a margin factor which is not unity
        # will set a plot range of [a - margin_factor*(b-a), b + margin_factor*(b-a)].
        self.margin_factor = 1.1

        # Axis labels for asymmetries
        self.asy_45_label = r'$A(\theta = 45^\circ)$'
        self.asy_90_label = r'$A(\theta = 90^\circ)$'

        self.asy_2d_color = 'black'

        self.asy_45_single_color = 'forestgreen'
        self.asy_45_single_style = '-'
        self.asy_90_single_color = 'crimson'
        self.asy_90_single_style = '-'
        self.asy_asy_single_color = 'purple'
        self.asy_asy_single_style = '-'

        del_ticks_for_arctan = np.array([-10., -1.5, -0.4, 0.,
                                         0.4, 1.5, 10.])
        self.del_ticks = np.arctan(del_ticks_for_arctan)
        self.del_tick_labels = ['{:4.1f}'.format(tick)
                                  for tick in del_ticks_for_arctan]

        self.asy_45_lim = [-self.margin_factor, self.margin_factor]
        self.asy_90_lim = [-self.margin_factor, self.margin_factor]

        if self.scale_asymmetries:
            asy_45_min = np.min(self.asy_45)
            asy_45_max = np.max(self.asy_45)
            asy_45_range = asy_45_max - asy_45_min
            self.asy_45_lim = [asy_45_min - 0.1*asy_45_range, asy_45_max + 0.1*asy_45_range]
            asy_90_min = np.min(self.asy_90)
            asy_90_max = np.max(self.asy_90)
            asy_90_range = asy_90_max - asy_90_min
            self.asy_90_lim = [asy_90_min - 0.1*asy_90_range, asy_90_max + 0.1*asy_90_range]

        self.asy_ticks = [-1., -0.5, 0., 0.5, 1.]
        self.asy_tick_labels = ['{:4.1f}'.format(tick) for tick in self.asy_ticks]

        self.arctan_del_lim = [-0.5*np.pi*self.margin_factor,
                   0.5*np.pi*self.margin_factor]
        self.arctan_del_ticks = [-0.5*np.pi, -0.25*np.pi, 0., 0.25*np.pi, 0.5*np.pi]
        self.arctan_del_tick_labels = [r'$-\pi/2$', r'$-\pi/4$', 
                           '$0$', r'$\pi/4$', r'$\pi/2$']

    def plot_single_2d(self, delta_label, delta_labels_level_scheme, returns_to_initial_state=True, output_file=None, auxiliary_lines_and_markers=False, transition_label_rotation=90):

        if not isinstance(self.asy_45[0], (int, float)):
            warnings.warn('Line plot requested when two multipole mixing ratios were varied. Assuming delta_1 = delta_2.')
            asy_45_single = np.diag(self.asy_45)
            asy_90_single = np.diag(self.asy_90)

            # Calculate new plot limits
            asy_45_min = np.min(asy_45_single)
            asy_45_max = np.max(asy_45_single)
            asy_45_range = asy_45_max - asy_45_min
            asy_45_single_lim = [asy_45_min - 0.1*asy_45_range, asy_45_max + 0.1*asy_45_range]
            asy_90_min = np.min(asy_90_single)
            asy_90_max = np.max(asy_90_single)
            asy_90_range = asy_90_max - asy_90_min
            asy_90_single_lim = [asy_90_min - 0.1*asy_90_range, asy_90_max + 0.1*asy_90_range]

        else:
            asy_45_single = self.asy_45
            asy_90_single = self.asy_90
            
            asy_45_single_lim = self.asy_45_lim
            asy_90_single_lim = self.asy_90_lim

        if self.asy_45_exp is not None:
            asy_45_delta_ranges_single = find_true_intervals(
                values_in_range(asy_45_single, self.asy_45_exp[0], self.asy_45_exp[1], self.asy_45_exp[2])
            )
            print('A(θ=45°) = {:+.2e} - {:.2e} + {:.2e}'.format(self.asy_45_exp[0], self.asy_45_exp[1], self.asy_45_exp[2]))
            for i, r in enumerate(asy_45_delta_ranges_single):
                print('\tδ_{:d} ∈ [{:+.2e}, {:+.2e}] (arctan(δ_{:d}) ∈ [{:+.2e}, {:+.2e}])'.format(
                    i+1, np.tan(self.arctan_deltas[r[0]]), np.tan(self.arctan_deltas[r[1]-1]), 
                    i+1, self.arctan_deltas[r[0]], self.arctan_deltas[r[1]-1]
                ))

        if self.asy_90_exp is not None:
            asy_90_delta_ranges_single = find_true_intervals(
                values_in_range(asy_90_single, self.asy_90_exp[0], self.asy_90_exp[1], self.asy_90_exp[2])
            )
            print('A(θ=90°) = {:+.2e} - {:.2e} + {:.2e}'.format(self.asy_90_exp[0], self.asy_90_exp[1], self.asy_90_exp[2]))
            for i, r in enumerate(asy_90_delta_ranges_single):
                print('\tδ_{:d} ∈ [{:+.2e}, {:+.2e}] (arctan(δ_{:d}) ∈ [{:+.2e}, {:+.2e}])'.format(
                    i+1, np.tan(self.arctan_deltas[r[0]]), np.tan(self.arctan_deltas[r[1]-1]), 
                    i+1, self.arctan_deltas[r[0]], self.arctan_deltas[r[1]-1]
                ))

        if self.asy_45_exp is not None and self.asy_90_exp is not None:
            asy_45_90_delta_ranges_single = find_interval_overlaps(
                asy_45_delta_ranges_single, asy_90_delta_ranges_single
            )
            print('Combined result')
            if not len(asy_45_90_delta_ranges_single):
                print('\tNo value of δ is in agreement with the experimental limits.')
            else:
                for i, r in enumerate(asy_45_90_delta_ranges_single):
                    print('\tδ_{:d} ∈ [{:+.2e}, {:+.2e}] (arctan(δ_{:d}) ∈ [{:+.2e}, {:+.2e}])'.format(
                        i+1, np.tan(self.arctan_deltas[r[0]]), np.tan(self.arctan_deltas[r[1]-1]), 
                        i+1, self.arctan_deltas[r[0]], self.arctan_deltas[r[1]-1]
                    ))

        aux_line_asy_45_style = '--'
        aux_line_asy_90_style = '--'
        aux_line_color = 'black'

        fontsize_ticks = 9

        markersize = 8
        markersize_asy_45 = 14
        markersize_asy_90 = 8
        delta_zero_marker = 'o'
        delta_infinity_marker = 'o'
        asy_45_min_marker = 'o'
        asy_45_max_marker = 'o'
        asy_90_min_marker = 'o'
        asy_90_max_marker = 'o'

        exp_arctan_delta = -0.5*np.pi
        exp_band_alpha = 0.5
        exp_band_color = 'gray'
        exp_final_band_color = 'chocolate'
        exp_capsize = 4
        exp_color = 'black'

        delta_zero_index = np.argmin(np.abs(self.arctan_deltas))
        if self.arctan_deltas[delta_zero_index] != 0.:
            warnings.warn('The value closest to δ = 0 which could be found is δ = {:f}'.format(self.arctan_deltas[delta_zero_index]))
        asy_45_min_index = np.argmin(asy_45_single)
        asy_45_max_index = np.argmax(asy_45_single)
        asy_90_min_index = np.argmin(asy_90_single)
        asy_90_max_index = np.argmax(asy_90_single)

        fig, ax = plt.subplots(2,2, figsize=(7, 7))
        plt.subplots_adjust(wspace=0.1, hspace=0.1)

        ax[0][0].tick_params(labelsize=fontsize_ticks)
        ax[0][0].set_xlim(asy_45_single_lim)
        ax[0][0].set_xticks([])
        ax[0][0].set_ylabel('arctan(' + delta_label + ')')
        ax[0][0].set_ylim(self.arctan_del_lim)
        ax[0][0].set_yticks(self.arctan_del_ticks)
        ax[0][0].set_yticklabels(self.arctan_del_tick_labels)

        if auxiliary_lines_and_markers:
            ax[0][0].plot([asy_45_single[delta_zero_index]]*2,
                        [self.arctan_del_lim[0], self.arctan_deltas[delta_zero_index]],
                        aux_line_asy_45_style, color=aux_line_color)
            ax[0][0].plot([asy_45_single[asy_45_min_index]]*2,
                        [self.arctan_del_lim[0], self.arctan_deltas[asy_45_min_index]],
                        aux_line_asy_45_style, color=aux_line_color)
            ax[0][0].plot([asy_45_single[asy_45_max_index]]*2,
                        [self.arctan_del_lim[0], self.arctan_deltas[asy_45_max_index]],
                        aux_line_asy_45_style, color=aux_line_color)

        ax[0][0].plot(asy_45_single, self.arctan_deltas, self.asy_45_single_style, color=self.asy_2d_color)

        if auxiliary_lines_and_markers:
            ax[0][0].plot(asy_45_single[delta_zero_index], [0.], delta_zero_marker,
                        markersize=markersize, color='black')
            ax[0][0].plot(asy_45_single[0], self.arctan_deltas[0], delta_infinity_marker,
                        markersize=markersize, color='black')
            ax[0][0].plot(asy_45_single[-1], self.arctan_deltas[-1], delta_infinity_marker,
                        markersize=markersize, color='black')
            ax[0][0].plot(asy_45_single[asy_45_min_index], self.arctan_deltas[asy_45_min_index],
                        marker=asy_45_min_marker, markersize=markersize_asy_45, color='black')
            ax[0][0].plot(asy_45_single[asy_45_max_index], self.arctan_deltas[asy_45_max_index],
                        marker=asy_45_max_marker, markersize=markersize_asy_45, color='black')

        if self.asy_45_exp is not None:
            ax[0][0].fill_betweenx(self.arctan_del_lim, [self.asy_45_exp[0]-self.asy_45_exp[1]]*2, [self.asy_45_exp[0]+self.asy_45_exp[2]], color=exp_band_color, alpha=exp_band_alpha)
            ax[0][0].errorbar([self.asy_45_exp[0]], [exp_arctan_delta], xerr=[[self.asy_45_exp[1]], [self.asy_45_exp[2]]], fmt='o', color=exp_color, capsize=exp_capsize)

            for r in asy_45_delta_ranges_single:
                ax[0][0].fill_between(asy_45_single_lim, [self.arctan_deltas[r[0]]]*2, [self.arctan_deltas[r[1]-1]]*2, color=exp_band_color, alpha=exp_band_alpha)

            if self.asy_90_exp is not None:
                for r in asy_45_90_delta_ranges_single:
                    ax[0][0].fill_between(asy_45_single_lim, [self.arctan_deltas[r[0]]]*2, [self.arctan_deltas[r[1]-1]]*2, color=exp_final_band_color, alpha=exp_band_alpha)

        ax00x = ax[0][0].twiny()
        ax00x.tick_params(labelsize=fontsize_ticks)
        ax00x.set_xlabel(self.asy_45_label)
        ax00x.set_xlim(asy_45_single_lim)
        if not self.scale_asymmetries:
            ax00x.set_xticks(self.asy_ticks)

        ax00y = ax[0][0].twinx()
        ax00y.tick_params(labelsize=fontsize_ticks)
        ax00y.set_ylabel(delta_label)
        ax00y.set_ylim(self.arctan_del_lim)
        ax00y.set_yticks(self.del_ticks)
        ax00y.set_yticklabels(self.del_tick_labels)

        ax[0][1].axis('off')
        ax[0][1].set_xlim(-1., 1.)
        ax[0][1].set_ylim(-1., 1.)
        lsplt = LevelSchemePlotter(ax[0][1], self.ang_cor.initial_state, self.ang_cor.cascade_steps,
                                   delta_labels_level_scheme, show_polarization=self.show_polarization,
                                   returns_to_initial_state=returns_to_initial_state, offset=(0.1, 0.1),
                                   transition_label_rotation=transition_label_rotation, em_variable_symbol='', parity_variable_symbol='')
        lsplt.plot()

        ax[1][0].tick_params(labelsize=fontsize_ticks)
        ax[1][0].set_xlabel(self.asy_45_label)
        ax[1][0].set_xlim(asy_45_single_lim)
        if not self.scale_asymmetries:
            ax[1][0].set_xticks(self.asy_ticks)
            ax[1][0].set_xticklabels(self.asy_tick_labels)
        ax[1][0].set_ylabel(self.asy_90_label)
        ax[1][0].set_ylim(asy_90_single_lim)
        if not self.scale_asymmetries:
            ax[1][0].set_yticks(self.asy_ticks)
            ax[1][0].set_yticklabels(self.asy_tick_labels)

        if auxiliary_lines_and_markers:
            ax[1][0].plot([asy_45_single[delta_zero_index], self.asy_45_lim[1]],
                        [asy_90_single[delta_zero_index]]*2, aux_line_asy_90_style, color=aux_line_color)
            ax[1][0].plot([asy_45_single[asy_90_max_index], self.asy_45_lim[1]],
                        [asy_90_single[asy_90_max_index]]*2, aux_line_asy_90_style, color=aux_line_color)
            ax[1][0].plot([asy_45_single[asy_90_min_index], self.asy_45_lim[1]],
                        [asy_90_single[asy_90_min_index]]*2, aux_line_asy_90_style, color=aux_line_color)

            ax[1][0].plot([asy_45_single[delta_zero_index]]*2,
                        [asy_90_single[delta_zero_index], self.asy_90_lim[1]],
                        aux_line_asy_45_style, color=aux_line_color)
            ax[1][0].plot([asy_45_single[asy_45_min_index]]*2,
                        [asy_90_single[asy_45_min_index], self.asy_90_lim[1]],
                        aux_line_asy_45_style, color=aux_line_color)
            ax[1][0].plot([asy_45_single[asy_45_max_index]]*2,
                        [asy_90_single[asy_45_max_index], self.asy_90_lim[1]],
                        aux_line_asy_45_style, color=aux_line_color)

        ax[1][0].plot(asy_45_single, asy_90_single, self.asy_asy_single_style, color=self.asy_2d_color)

        if auxiliary_lines_and_markers:
            ax[1][0].plot(asy_45_single[delta_zero_index], asy_90_single[delta_zero_index], delta_zero_marker,
                        markersize=markersize, color='black')
            ax[1][0].plot(asy_45_single[0], asy_90_single[0], delta_infinity_marker,
                        markersize=markersize, color='black')
            ax[1][0].plot(asy_45_single[asy_45_min_index], asy_90_single[asy_45_min_index],
                        marker=asy_45_min_marker, markersize=markersize_asy_45, color='black')
            ax[1][0].plot(asy_45_single[asy_45_max_index], asy_90_single[asy_45_max_index],
                        marker=asy_45_max_marker, markersize=markersize_asy_45, color='black')
            ax[1][0].plot(asy_45_single[asy_90_min_index], asy_90_single[asy_90_min_index],
                        marker=asy_90_min_marker, markersize=markersize_asy_90, color='black')
            ax[1][0].plot(asy_45_single[asy_90_max_index], asy_90_single[asy_90_max_index],
                        marker=asy_90_max_marker, markersize=markersize_asy_90, color='black')

        if self.asy_45_exp is not None:
            ax[1][0].fill_betweenx(self.asy_90_lim, [self.asy_45_exp[0]-self.asy_45_exp[1]]*2, [self.asy_45_exp[0]+self.asy_45_exp[2]], color=exp_band_color, alpha=exp_band_alpha)
        if self.asy_90_exp is not None:
            ax[1][0].fill_between(self.asy_45_lim, [self.asy_90_exp[0]-self.asy_90_exp[1]]*2, [self.asy_90_exp[0]+self.asy_90_exp[2]], color=exp_band_color, alpha=exp_band_alpha)
        if None not in (self.asy_45_exp, self.asy_90_exp):
            ax[1][0].errorbar([self.asy_45_exp[0]], [self.asy_90_exp[0]], xerr=[[self.asy_45_exp[1]], [self.asy_45_exp[2]]],
            yerr=[[self.asy_90_exp[1]], [self.asy_90_exp[2]]], fmt='o', color=exp_color, capsize=exp_capsize)

        ax[1][1].tick_params(labelsize=fontsize_ticks)
        ax[1][1].set_xlabel('arctan(' + delta_label + ')')
        ax[1][1].set_xlim(self.arctan_del_lim)
        ax[1][1].set_xticks(self.arctan_del_ticks)
        ax[1][1].set_xticklabels(self.arctan_del_tick_labels)
        ax[1][1].set_yticks([])
        ax[1][1].set_ylim(asy_90_single_lim)

        if auxiliary_lines_and_markers:
            ax[1][1].plot([self.arctan_del_lim[0], 0.], [asy_90_single[delta_zero_index]]*2,
                        aux_line_asy_90_style, color=aux_line_color)
            ax[1][1].plot([self.arctan_del_lim[0], self.arctan_deltas[asy_90_min_index]],
                        [asy_90_single[asy_90_min_index]]*2,
                        aux_line_asy_90_style, color=aux_line_color)
            ax[1][1].plot([self.arctan_del_lim[0], self.arctan_deltas[asy_90_max_index]],
                        [asy_90_single[asy_90_max_index]]*2,
                        aux_line_asy_90_style, color=aux_line_color)

        ax[1][1].plot(self.arctan_deltas, asy_90_single, self.asy_90_single_style, color=self.asy_2d_color)

        if auxiliary_lines_and_markers:
            ax[1][1].plot(self.arctan_deltas[0], asy_90_single[0], delta_infinity_marker,
                        markersize=markersize, color='black')
            ax[1][1].plot(self.arctan_deltas[-1], asy_90_single[-1], delta_infinity_marker,
                        markersize=markersize, color='black')
            ax[1][1].plot([0.], asy_90_single[delta_zero_index], delta_zero_marker,
                        markersize=markersize, color='black')
            ax[1][1].plot(self.arctan_deltas[asy_90_min_index], asy_90_single[asy_90_min_index],
                        marker=asy_90_min_marker, markersize=markersize_asy_90, color='black')
            ax[1][1].plot(self.arctan_deltas[asy_90_max_index], asy_90_single[asy_90_max_index],
                        marker=asy_90_max_marker, markersize=markersize_asy_90, color='black')

        if self.asy_90_exp is not None:
            ax[1][1].fill_between(self.arctan_del_lim, [self.asy_90_exp[0]-self.asy_90_exp[1]]*2, [self.asy_90_exp[0]+self.asy_90_exp[2]], color=exp_band_color, alpha=exp_band_alpha)
            ax[1][1].errorbar([exp_arctan_delta], [self.asy_90_exp[0]], yerr=[[self.asy_90_exp[1]], [self.asy_90_exp[2]]], fmt='o', color=exp_color, capsize=exp_capsize)

            for r in asy_90_delta_ranges_single:
                ax[1][1].fill_betweenx(asy_90_single_lim, [self.arctan_deltas[r[0]]]*2, [self.arctan_deltas[r[1]-1]]*2, color=exp_band_color, alpha=exp_band_alpha)

            if self.asy_45_exp is not None:
                for r in asy_45_90_delta_ranges_single:
                    ax[1][1].fill_betweenx(asy_90_single_lim, [self.arctan_deltas[r[0]]]*2, [self.arctan_deltas[r[1]-1]]*2, color=exp_final_band_color, alpha=exp_band_alpha)

        ax11x = ax[1][1].twinx()
        ax11x.tick_params(labelsize=fontsize_ticks)
        ax11x.set_ylim(asy_90_single_lim)
        if not self.scale_asymmetries:
            ax11x.set_yticks(self.asy_ticks)
            ax11x.set_yticklabels(self.asy_tick_labels)
        ax11x.set_ylabel(self.asy_90_label)

        ax11y = ax[1][1].twiny()
        ax11y.tick_params(labelsize=fontsize_ticks)
        ax11y.set_xlabel(delta_label)
        ax11y.set_xlim(self.arctan_del_lim)
        ax11y.set_xticks(self.del_ticks)
        ax11y.set_xticklabels(self.del_tick_labels)

        if output_file:
            plt.savefig(output_file)

        plt.close()

    def plot_single_3d(self, delta_label, output_file=None):

        if not isinstance(self.asy_45[0], (int, float)):
            warnings.warn('Line plot requested when two multipole mixing ratios were varied. Assuming delta_1 = delta_2.')
            asy_45_single = np.diag(self.asy_45)
            asy_90_single = np.diag(self.asy_90)

            # Calculate new plot limits
            asy_45_min = np.min(asy_45_single)
            asy_45_max = np.max(asy_45_single)
            asy_45_range = asy_45_max - asy_45_min
            asy_45_single_lim = [asy_45_min - 0.1*asy_45_range, asy_45_max + 0.1*asy_45_range]
            asy_90_min = np.min(asy_90_single)
            asy_90_max = np.max(asy_90_single)
            asy_90_range = asy_90_max - asy_90_min
            asy_90_single_lim = [asy_90_min - 0.1*asy_90_range, asy_90_max + 0.1*asy_90_range]

        else:
            asy_45_single = self.asy_45
            asy_90_single = self.asy_90
            
            asy_45_single_lim = self.asy_45_lim
            asy_90_single_lim = self.asy_90_lim
        
        line_width = 3
        
        fig = plt.figure(figsize=(6,5))
        ax = fig.gca(projection='3d')

        ax.set_xlabel(self.asy_45_label)
        ax.set_xlim(asy_45_single_lim)

        ax.set_ylabel(self.asy_90_label)
        ax.set_ylim(asy_90_single_lim)

        ax.set_zlabel('arctan(' + delta_label + ')')
        ax.set_zlim(self.arctan_del_lim)
        ax.set_zticks(self.arctan_del_ticks)
        ax.set_zticklabels(self.arctan_del_tick_labels)

        ax.plot(asy_45_single, [asy_90_single_lim[1]]*len(self.arctan_deltas),
                self.arctan_deltas, color=self.asy_45_single_color, linewidth=line_width)
        ax.plot([asy_45_single_lim[0]]*len(self.arctan_deltas), asy_90_single, 
                self.arctan_deltas, color=self.asy_90_single_color, linewidth=line_width)
        ax.plot(asy_45_single, asy_90_single, 
                [self.arctan_del_lim[0]]*len(self.arctan_deltas),
                color=self.asy_asy_single_color, linewidth=line_width)
        
        for i in range(len(self.arctan_deltas)):
            ax.plot([asy_45_single[i]]*2, [asy_90_single[i], asy_90_single_lim[1]],
                    [self.arctan_deltas[i]]*2, color=self.asy_45_single_color, alpha=0.2)
            ax.plot([asy_45_single_lim[0], asy_45_single[i]], [asy_90_single[i]]*2, 
                    [self.arctan_deltas[i]]*2, color=self.asy_90_single_color, alpha=0.2)
            ax.plot([asy_45_single[i]]*2, [asy_90_single[i]]*2, 
                    [self.arctan_del_lim[0], self.arctan_deltas[i]],
                    color=self.asy_asy_single_color, alpha=0.2)

        ax.plot(asy_45_single, asy_90_single, self.arctan_deltas, 
                color='black', linewidth=line_width)

        plt.tight_layout()
        
        if output_file:
            plt.savefig(output_file)

    def plot_double_contour(self, delta_labels, output_file=None):

        arctan_deltas_1, arctan_deltas_2 = np.meshgrid(self.arctan_deltas, self.arctan_deltas)
        if isinstance(self.asy_45[0], (int, float)):
            warnings.warn('Contour plot requested when only a single multipole mixing ratio was varied.')
            asy_45_double = np.zeros((len(self.arctan_deltas), len(self.arctan_deltas)))
            asy_90_double = np.zeros((len(self.arctan_deltas), len(self.arctan_deltas)))
            for i in range(len(self.arctan_deltas)):
                asy_45_double[i] = self.asy_45
                asy_90_double[i] = self.asy_90
        else:
            asy_45_double = self.asy_45
            asy_90_double = self.asy_90

        contour_levels = 10
        
        alpha_shadow = 0.6
        color_shadow = 'pink'
        fontsize_axis_label = 12

        fig = plt.figure(figsize=(6, 8))
        gs = gridspec.GridSpec(2, 2, width_ratios=(1, 0.1))

        ax0 = plt.subplot(gs[0])
        ax0.set_xticks([])
        ax0.set_xlim(self.arctan_del_lim)        
        ax0.set_yticks(self.arctan_del_ticks)
        ax0.set_yticklabels(self.arctan_del_tick_labels)
        ax0.set_ylim(self.arctan_del_lim)

        cs0 = ax0.contourf(arctan_deltas_1, arctan_deltas_2, asy_45_double,
                           levels=contour_levels, cmap='viridis')
        ax0.contour(arctan_deltas_1, arctan_deltas_2, asy_45_double,
                    levels=contour_levels, colors='black')
        ax0.contour(arctan_deltas_1, arctan_deltas_2, asy_90_double,
                    levels=contour_levels, colors=color_shadow, alpha=alpha_shadow)
        ax0.set_ylabel('arctan(' + delta_labels[1] + ')', fontsize=fontsize_axis_label)
        ax1 = plt.subplot(gs[1])
        cb0 = fig.colorbar(cs0, ax1)
        cb0.set_label(self.asy_45_label, fontsize=fontsize_axis_label)

        ax0x = ax0.twiny()
        ax0x.set_xlim(self.arctan_del_lim)
        ax0x.set_xlabel(delta_labels[0], fontsize=fontsize_axis_label)
        ax0x.set_xticks(self.del_ticks)
        ax0x.set_xticklabels(self.del_tick_labels)

        ax0y = ax0.twinx()
        ax0y.set_ylim(self.arctan_del_lim)
        ax0y.set_ylabel(delta_labels[1], fontsize=fontsize_axis_label)
        ax0y.set_yticks(self.del_ticks)
        ax0y.set_yticklabels(self.del_tick_labels)        

        ax2 = plt.subplot(gs[2])
        ax2.set_xticks(self.arctan_del_ticks)
        ax2.set_xticklabels(self.arctan_del_tick_labels)
        ax2.set_xlim(self.arctan_del_lim)        
        ax2.set_yticks(self.arctan_del_ticks)
        ax2.set_yticklabels(self.arctan_del_tick_labels)
        ax2.set_ylim(self.arctan_del_lim)        
        cs2 = ax2.contourf(arctan_deltas_1, arctan_deltas_2, asy_90_double, 
                           levels=contour_levels, cmap='viridis')
        ax2.contour(arctan_deltas_1, arctan_deltas_2, asy_90_double, 
                    levels=contour_levels, colors='black')
        ax2.contour(arctan_deltas_1, arctan_deltas_2, asy_45_double, 
                    levels=contour_levels, colors=color_shadow, alpha=alpha_shadow)
        ax2.set_xlabel('arctan(' + delta_labels[0] + ')', fontsize=fontsize_axis_label)
        ax2.set_ylabel('arctan(' + delta_labels[1] + ')', fontsize=fontsize_axis_label)

        ax2y = ax2.twinx()
        ax2y.set_ylim(self.arctan_del_lim)
        ax2y.set_ylabel(delta_labels[1], fontsize=fontsize_axis_label)
        ax2y.set_yticks(self.del_ticks)
        ax2y.set_yticklabels(self.del_tick_labels) 

        ax3 = plt.subplot(gs[3])
        cb2 = fig.colorbar(cs2, ax3)
        cb2.set_label(self.asy_90_label, fontsize=fontsize_axis_label)

        plt.tight_layout()
        
        if output_file:
            plt.savefig(output_file)

        plt.close()

    def plot_double_contour_inverse(self, delta_labels, output_file=None, returns_to_initial_state=True):

        if isinstance(self.asy_45[0], (int, float)):
            raise ValueError('Inverse contour plot requested when only one multipole mixing ratio was varied. This is not possible.')

        asy_45_asy_90_plot_width = 0.42
        asy_45_asy_90_plot_height = 0.33
        asy_45_asy_90_max_plot_y = 0.67
        asy_45_asy_90_min_plot_y = 0.36
        asy_45_asy_90_delta_1_plot_x = 0.
        asy_45_asy_90_delta_2_plot_x = 0.37
        asy_45_asy_90_plot_width_in_bbox = '80%'
        asy_45_asy_90_plot_height_in_bbox = '85%'

        color_none = 'white'
        color_val_none = 0.
        color_asy_45 = 'limegreen'
        color_val_delta1 = 0.33
        color_asy_90 = 'orange'
        color_val_delta2 = 0.66
        color_both = 'black'
        color_val_both = 1.

        color_bar_x = 0.88
        color_bar_y = 0.4
        color_bar_width = 0.05
        color_bar_height = 0.45
        color_bar_width_in_bbox = '80%'
        color_bar_height_in_bbox = '80%'

        cmap = ListedColormap([color_none, color_asy_45, color_asy_90, color_both])

        exclusion_plot_x = 0.39
        exclusion_plot_y = 0.
        exclusion_plot_width = 0.42
        exclusion_plot_height = 0.33
        exclusion_plot_width_in_bbox = '80%'
        exclusion_plot_height_in_bbox = '85%'

        exp_capsize = 4
        exp_capthick = 2
        exp_color = 'black'
        exp_elinewidth = 2

        fontsize_axis_label = 13
        fontsize_legend = 11
        fontsize_text = 15
        fontsize_ticks = 12

        label_delta = r'$\delta$'
        label_delta_min = r'$\delta_{min}$'
        label_delta_max = r'$\delta_{max}$'
        label_delta_diff = label_delta_max + ' - ' + label_delta_min

        level_scheme_x = 0.05
        level_scheme_y = 0.03
        level_scheme_width = 0.42
        level_scheme_height = 0.33
        level_scheme_width_in_bbox = '100%'
        level_scheme_height_in_bbox = '100%'

        min_max_line_width = 2
        min_max_line_style = ':'

        text_x = self.asy_45_lim[0] + 0.1*(self.asy_45_lim[1] - self.asy_45_lim[0])
        text_y = self.asy_90_lim[1] - 0.1*(self.asy_90_lim[1] - self.asy_90_lim[0])

        n_contour_levels = 9

        label_arctan_delta = r'$\mathrm{arctan}(\delta)$'
        label_arctan_delta_min = r'$\mathrm{arctan}(\delta_{min})$'
        label_arctan_delta_max = r'$\mathrm{arctan}(\delta_{max})$'
        label_arctan_delta_diff = label_arctan_delta_max + ' - ' + label_arctan_delta_min

        arctan_delta_ticks_diff = [0., 0.25*np.pi, 0.5*np.pi, 0.75*np.pi, np.pi]
        arctan_delta_labels_diff = [r'$0$', r'$\pi/4$', '$\pi/2$', r'$3\pi/4$', r'$\pi$']

        arctan_deltas_1, arctan_deltas_2 = np.meshgrid(self.arctan_deltas, self.arctan_deltas)

        n_bins = 100
        bins_asy_45 = np.linspace(self.asy_45_lim[0], self.asy_45_lim[1], n_bins)
        bins_asy_90 = np.linspace(self.asy_90_lim[0], self.asy_90_lim[1], n_bins)

        hist_min_1 = np.ones((n_bins, n_bins))*np.pi
        hist_max_1 = np.ones((n_bins, n_bins))*-np.pi
        hist_counts_1 = np.zeros((n_bins, n_bins))
        hist_min_2 = np.ones((n_bins, n_bins))*np.pi
        hist_max_2 = np.ones((n_bins, n_bins))*-np.pi
        hist_counts_2 = np.zeros((n_bins, n_bins))

        hist_deltas = np.ones((len(self.arctan_deltas), len(self.arctan_deltas)))*color_val_none
        bins_arctan_delta_1_grid, bins_arctan_delta_2_grid = np.meshgrid(self.arctan_deltas, self.arctan_deltas)

        arctan_delta_1_min_exp = 0.5*np.pi
        arctan_delta_1_max_exp = -0.5*np.pi
        arctan_delta_2_min_exp = 0.5*np.pi
        arctan_delta_2_max_exp = -0.5*np.pi

        for i in range(len(self.arctan_deltas)):
            for j in range(len(self.arctan_deltas)):
                asy_45_value = self.asy_45[i][j]
                asy_90_value = self.asy_90[i][j]

                if self.asy_45_exp is not None and self.asy_90_exp is not None:
                    if self.asy_45_exp[0] - self.asy_45_exp[1] <= asy_45_value and self.asy_45_exp[0] + self.asy_45_exp[2] >= asy_45_value:
                        hist_deltas[i][j] = color_val_delta1
                    if self.asy_90_exp[0] - self.asy_90_exp[1] <= asy_90_value and self.asy_90_exp[0] + self.asy_90_exp[2] >= asy_90_value:
                        hist_deltas[i][j] = color_val_delta2
                    if self.asy_45_exp[0] - self.asy_45_exp[1] <= asy_45_value and self.asy_45_exp[0] + self.asy_45_exp[2] >= asy_45_value and self.asy_90_exp[0] - self.asy_90_exp[1] <= asy_90_value and self.asy_90_exp[0] + self.asy_90_exp[2] >= asy_90_value:
                        hist_deltas[i][j] = color_val_both
                        if bins_arctan_delta_1_grid[i][j] < arctan_delta_1_min_exp:
                            arctan_delta_1_min_exp = bins_arctan_delta_1_grid[i][j]
                        elif bins_arctan_delta_1_grid[i][j] > arctan_delta_1_max_exp:
                            arctan_delta_1_max_exp = bins_arctan_delta_1_grid[i][j]
                        if bins_arctan_delta_2_grid[i][j] < arctan_delta_2_min_exp:
                            arctan_delta_2_min_exp = bins_arctan_delta_2_grid[i][j]
                        elif bins_arctan_delta_2_grid[i][j] > arctan_delta_2_max_exp:
                            arctan_delta_2_max_exp = bins_arctan_delta_2_grid[i][j]

                asy_45_bin = np.argmin(np.abs(bins_asy_45-asy_45_value))
                asy_90_bin = np.argmin(np.abs(bins_asy_90-asy_90_value))

                arctan_delta_1 = arctan_deltas_1[i][j]

                if arctan_delta_1 < hist_min_1[asy_90_bin][asy_45_bin]:
                    hist_min_1[asy_90_bin][asy_45_bin] = arctan_delta_1
                if arctan_delta_1 > hist_max_1[asy_90_bin][asy_45_bin]:
                    hist_max_1[asy_90_bin][asy_45_bin] = arctan_delta_1
                hist_counts_1[asy_90_bin][asy_45_bin] += 1

                arctan_delta_2 = arctan_deltas_2[i][j]

                if arctan_delta_2 < hist_min_2[asy_90_bin][asy_45_bin]:
                    hist_min_2[asy_90_bin][asy_45_bin] = arctan_delta_2
                if arctan_delta_2 > hist_max_2[asy_90_bin][asy_45_bin]:
                    hist_max_2[asy_90_bin][asy_45_bin] = arctan_delta_2
                hist_counts_2[asy_90_bin][asy_45_bin] += 1

        print('arctan(δ_1) ∈ [{:+.2e} π, {:+.2e} π] (δ_1 ∈ [{:+.2e}, {:+.2e}])'.format(arctan_delta_1_min_exp/np.pi, arctan_delta_1_max_exp/np.pi, np.tan(arctan_delta_1_min_exp), np.tan(arctan_delta_1_max_exp)))
        print('arctan(δ_2) ∈ [{:+.2e} π, {:+.2e} π] (δ_1 ∈ [{:+.2e}, {:+.2e}])'.format(arctan_delta_2_min_exp/np.pi, arctan_delta_2_max_exp/np.pi, np.tan(arctan_delta_2_min_exp), np.tan(arctan_delta_2_max_exp)))

        bins_asy_45_grid, bins_asy_90_grid = np.meshgrid(bins_asy_45, bins_asy_90)

        hist_min_1[hist_counts_1 < 1] = np.nan
        hist_max_1[hist_counts_1 < 1] = np.nan
        hist_min_2[hist_counts_2 < 1] = np.nan
        hist_max_2[hist_counts_2 < 1] = np.nan

        fig, ax = plt.subplots(1, 1, figsize=(10, 11))
        ax.axis('off')

        ax_del1_max = inset_axes(ax, width=asy_45_asy_90_plot_width_in_bbox, height=asy_45_asy_90_plot_height_in_bbox, bbox_to_anchor=
        [
            asy_45_asy_90_delta_1_plot_x, asy_45_asy_90_max_plot_y,
            asy_45_asy_90_plot_width,
            asy_45_asy_90_plot_height
        ], bbox_transform=ax.transAxes)
        ax_del1_max.set_xlim(self.asy_45_lim)
        ax_del1_max.tick_params(labelsize=fontsize_ticks)
        ax_del1_max.set_xticklabels([])
        ax_del1_max.set_ylabel(r'$A(\theta = 90^\circ)$', fontsize=fontsize_axis_label)
        ax_del1_max.set_ylim(self.asy_90_lim)
        if None not in (self.asy_45_exp, self.asy_90_exp):
            ax_del1_max.errorbar([self.asy_45_exp[0]], [self.asy_90_exp[0]], xerr=[[self.asy_45_exp[1]], [self.asy_45_exp[2]]],
            yerr=[[self.asy_90_exp[1]], [self.asy_90_exp[2]]], fmt='o', elinewidth=exp_elinewidth, color=exp_color, capsize=exp_capsize, capthick=exp_capthick)

        cs0 = ax_del1_max.contourf(
            bins_asy_45_grid,
            bins_asy_90_grid,
            hist_max_1,
            levels=np.linspace(-0.5*np.pi, 0.5*np.pi, n_contour_levels),
            cmap='coolwarm'
        )

        ax_del1_max_x2 = ax_del1_max.twinx()
        ax_del1_max_x2.tick_params(labelsize=fontsize_ticks)
        ax_del1_max_x2.set_ylim(self.asy_90_lim)
        ax_del1_max_x2.set_yticklabels([])
        ax_del1_max_y2 = ax_del1_max.twiny()
        ax_del1_max_y2.tick_params(labelsize=fontsize_ticks)
        ax_del1_max_y2.set_xlim(self.asy_45_lim)
        ax_del1_max_y2.set_xticklabels([])

        ax_del1_max.text(text_x, text_y, r'$\delta_{1, \mathrm{max}}$', fontsize=fontsize_text, verticalalignment='center')

        ax_del = inset_axes(ax, width=color_bar_width_in_bbox, height=color_bar_height_in_bbox, bbox_to_anchor=
        [
            color_bar_x, 
            color_bar_y,
            color_bar_width,
            color_bar_height
        ], bbox_transform=ax.transAxes)
        cb_del = fig.colorbar(cs0, ax_del)
        cb_del.set_label(label_delta, fontsize=fontsize_axis_label)
        cb_del.ax.tick_params(labelsize=fontsize_ticks)
        cb_del.set_ticks(self.del_ticks)
        cb_del.ax.set_yticklabels(self.del_tick_labels)

        ax_del2_max = inset_axes(ax, width=asy_45_asy_90_plot_width_in_bbox, height=asy_45_asy_90_plot_height_in_bbox, bbox_to_anchor=
        [
            asy_45_asy_90_delta_2_plot_x, asy_45_asy_90_max_plot_y,
            asy_45_asy_90_plot_width,
            asy_45_asy_90_plot_height
        ], bbox_transform=ax.transAxes)
        ax_del2_max.set_xlim(self.asy_45_lim)
        ax_del2_max.tick_params(labelsize=fontsize_ticks)
        ax_del2_max.set_xticklabels([])
        ax_del2_max.set_ylim(self.asy_90_lim)
        ax_del2_max.set_yticklabels([])
        if None not in (self.asy_45_exp, self.asy_90_exp):
            ax_del2_max.errorbar([self.asy_45_exp[0]], [self.asy_90_exp[0]], xerr=[[self.asy_45_exp[1]], [self.asy_45_exp[2]]],
            yerr=[[self.asy_90_exp[1]], [self.asy_90_exp[2]]], fmt='o', elinewidth=exp_elinewidth, color=exp_color, capsize=exp_capsize, capthick=exp_capthick)
        cs2 = ax_del2_max.contourf(
            bins_asy_45_grid,
            bins_asy_90_grid,
            hist_max_2,
            levels=np.linspace(-0.5*np.pi, 0.5*np.pi, n_contour_levels),
            cmap='coolwarm'
        )

        ax_del2_max_x2 = ax_del2_max.twinx()
        ax_del2_max_x2.tick_params(labelsize=fontsize_ticks)
        ax_del2_max_x2.set_ylim(self.asy_90_lim)
        ax_del2_max_x2.set_yticklabels([])
        ax_del2_max_y2 = ax_del2_max.twiny()
        ax_del2_max_y2.tick_params(labelsize=fontsize_ticks)
        ax_del2_max_y2.set_xlim(self.asy_45_lim)
        ax_del2_max_y2.set_xticklabels([])

        ax_del2_max.text(text_x, text_y, r'$\delta_{2, \mathrm{max}}$', fontsize=fontsize_text, verticalalignment='center')

        ax_arctan_del = inset_axes(ax, width=color_bar_width_in_bbox, height=color_bar_height_in_bbox, bbox_to_anchor=
        [
            color_bar_x, 
            color_bar_y,
            color_bar_width,
            color_bar_height
        ], bbox_transform=ax.transAxes)
        cb_arctan_del = fig.colorbar(cs2, ax_arctan_del, ticklocation='left')
        cb_arctan_del.set_label(label_arctan_delta, fontsize=fontsize_axis_label)
        cb_arctan_del.ax.tick_params(labelsize=fontsize_ticks)
        cb_arctan_del.set_ticks(self.arctan_del_ticks)
        cb_arctan_del.ax.set_yticklabels(self.arctan_del_tick_labels)

        ax_del1_min = inset_axes(ax, width=asy_45_asy_90_plot_width_in_bbox, height=asy_45_asy_90_plot_height_in_bbox, bbox_to_anchor=
        [
            asy_45_asy_90_delta_1_plot_x, asy_45_asy_90_min_plot_y,
            asy_45_asy_90_plot_width,
            asy_45_asy_90_plot_height
        ], bbox_transform=ax.transAxes)
        ax_del1_min.set_xlim(self.asy_45_lim)
        ax_del1_min.set_ylim(self.asy_90_lim)
        ax_del1_min.tick_params(labelsize=fontsize_ticks)
        if self.asy_45_exp is not None and self.asy_90_exp is not None:
            ax_del1_min.set_xlabel(r'$A(\theta = 45^\circ)$', fontsize=fontsize_axis_label)
        else:
            ax_del1_min.set_xticklabels([])
        ax_del1_min.set_ylabel(r'$A(\theta = 90^\circ)$', fontsize=fontsize_axis_label)
        if None not in (self.asy_45_exp, self.asy_90_exp):
            ax_del1_min.errorbar([self.asy_45_exp[0]], [self.asy_90_exp[0]], xerr=[[self.asy_45_exp[1]], [self.asy_45_exp[2]]],
            yerr=[[self.asy_90_exp[1]], [self.asy_90_exp[2]]], fmt='o', elinewidth=exp_elinewidth, color=exp_color, capsize=exp_capsize, capthick=exp_capthick)

        cs4 = ax_del1_min.contourf(
            bins_asy_45_grid,
            bins_asy_90_grid,
            hist_min_1,
            levels=np.linspace(-0.5*np.pi, 0.5*np.pi, n_contour_levels),
            cmap='coolwarm'
        )

        ax_del1_min_x2 = ax_del1_min.twinx()
        ax_del1_min_x2.tick_params(labelsize=fontsize_ticks)
        ax_del1_min_x2.set_ylim(self.asy_90_lim)
        ax_del1_min_x2.set_yticklabels([])
        ax_del1_min_y2 = ax_del1_min.twiny()
        ax_del1_min_y2.tick_params(labelsize=fontsize_ticks)
        ax_del1_min_y2.set_xlim(self.asy_45_lim)
        ax_del1_min_y2.set_xticklabels([])

        ax_del1_min.text(text_x, text_y, r'$\delta_{1, \mathrm{min}}$', fontsize=fontsize_text, verticalalignment='center')

        ax_del2_min = inset_axes(ax, width=asy_45_asy_90_plot_width_in_bbox, height=asy_45_asy_90_plot_height_in_bbox, bbox_to_anchor=
        [
            asy_45_asy_90_delta_2_plot_x, asy_45_asy_90_min_plot_y,
            asy_45_asy_90_plot_width,
            asy_45_asy_90_plot_height
        ], bbox_transform=ax.transAxes)
        ax_del2_min.set_xlim(self.asy_45_lim)
        ax_del2_min.set_ylim(self.asy_90_lim)
        ax_del2_min.tick_params(labelsize=fontsize_ticks)
        if self.asy_45_exp is not None and self.asy_90_exp is not None:
            ax_del2_min.set_xlabel(r'$A(\theta = 45^\circ)$', fontsize=fontsize_axis_label)
        else:
            ax_del2_min.set_xticklabels([])

        ax_del2_min.set_yticklabels([])
        if None not in (self.asy_45_exp, self.asy_90_exp):
            ax_del2_min.errorbar([self.asy_45_exp[0]], [self.asy_90_exp[0]], xerr=[[self.asy_45_exp[1]], [self.asy_45_exp[2]]],
            yerr=[[self.asy_90_exp[1]], [self.asy_90_exp[2]]], fmt='o', elinewidth=exp_elinewidth, color=exp_color, capsize=exp_capsize, capthick=exp_capthick)

        cs6 = ax_del2_min.contourf(
            bins_asy_45_grid,
            bins_asy_90_grid,
            hist_min_2,
            levels=np.linspace(-0.5*np.pi, 0.5*np.pi, n_contour_levels),
            cmap='coolwarm'
        )

        ax_del2_min_x2 = ax_del2_min.twinx()
        ax_del2_min_x2.tick_params(labelsize=fontsize_ticks)
        ax_del2_min_x2.set_ylim(self.asy_90_lim)
        ax_del2_min_x2.set_yticklabels([])
        ax_del2_min_y2 = ax_del2_min.twiny()
        ax_del2_min_y2.tick_params(labelsize=fontsize_ticks)
        ax_del2_min_y2.set_xlim(self.asy_45_lim)
        ax_del2_min_y2.set_xticklabels([])

        ax_del2_min.text(text_x, text_y, r'$\delta_{2, \mathrm{min}}$', fontsize=fontsize_text, verticalalignment='center')

        if self.asy_45_exp is not None and self.asy_90_exp is not None:
            ax_lvl = inset_axes(ax, width=level_scheme_width_in_bbox, height=level_scheme_height_in_bbox, bbox_to_anchor=
            [
                level_scheme_x, level_scheme_y,
                level_scheme_width,
                level_scheme_height
            ], bbox_transform=ax.transAxes)            
            ax_lvl.axis('off')
            ax_lvl.set_xlim(-1., 1.)
            ax_lvl.set_ylim(-1., 1.)
            lsplt = LevelSchemePlotter(ax_lvl, self.ang_cor.initial_state, self.ang_cor.cascade_steps,
                                    delta_labels, show_polarization=self.show_polarization,
                                    returns_to_initial_state=returns_to_initial_state, fontsize=16,
                                    offset=(-0.1, 0.), state_line_width=3, arrow_width=3, transition_label_rotation=0, em_variable_symbol='', parity_variable_symbol='')
            lsplt.plot()

            ax_delta = inset_axes(ax, width=exclusion_plot_width_in_bbox, height=exclusion_plot_height_in_bbox, bbox_to_anchor=
            [
                exclusion_plot_x, exclusion_plot_y,
                exclusion_plot_width,
                exclusion_plot_height
            ], bbox_transform=ax.transAxes) 
            ax_delta.set_xlabel(r'$\mathrm{arctan}(\delta_1)$', fontsize=fontsize_axis_label)
            ax_delta.tick_params(labelsize=fontsize_ticks)
            ax_delta.set_xlim(self.arctan_deltas[0], self.arctan_deltas[-1])
            ax_delta.set_xticks(self.arctan_del_ticks)
            ax_delta.set_xticklabels(self.arctan_del_tick_labels)
            ax_delta.set_ylim(self.arctan_deltas[0], self.arctan_deltas[-1])
            ax_delta.set_yticks(self.arctan_del_ticks)
            ax_delta.set_yticklabels([])
            ax_delta.imshow(
                hist_deltas, interpolation='none', cmap=cmap, origin='lower', extent=[self.arctan_deltas[0], self.arctan_deltas[-1], self.arctan_deltas[0], self.arctan_deltas[-1]], 
                aspect='auto'
            )
            ax_delta.legend(
                (
                    Rectangle((0., 0.), 1., 1., color=color_asy_45),
                    Rectangle((0., 0.), 1., 1., color=color_asy_90),
                    Rectangle((0., 0.), 1., 1., color=color_both),
                ), (
                    r'$A(45^\circ)$',
                    r'$A(90^\circ)$',
                    r'$A(45^\circ) \wedge A(90^\circ)$',
                ),
                loc='lower left',
                fontsize=fontsize_legend
            )

            ax_delta.plot(
                [arctan_delta_1_min_exp, arctan_delta_1_min_exp, arctan_delta_1_max_exp, arctan_delta_1_max_exp, arctan_delta_1_min_exp],
                [arctan_delta_2_min_exp, arctan_delta_2_max_exp, arctan_delta_2_max_exp, arctan_delta_2_min_exp, arctan_delta_2_min_exp],
                min_max_line_style, lw=min_max_line_width, color='black'
            )

            ax_delta_x2 = ax_delta.twinx()
            ax_delta_x2.tick_params(labelsize=fontsize_ticks)
            ax_delta_x2.set_ylabel(r'$\mathrm{arctan}(\delta_2)$', fontsize=fontsize_axis_label)
            ax_delta_x2.set_yticks(self.arctan_del_ticks)
            ax_delta_x2.set_yticklabels(self.arctan_del_tick_labels)
            ax_delta_y2 = ax_delta.twiny()
            ax_delta_y2.tick_params(labelsize=fontsize_ticks)
            ax_delta_y2.set_xlim(self.arctan_deltas[0], self.arctan_deltas[-1])
            ax_delta_y2.set_xticks(self.arctan_del_ticks)
            ax_delta_y2.set_xticklabels([])

        plt.tight_layout()

        if output_file:
            plt.savefig(output_file)

        plt.close()