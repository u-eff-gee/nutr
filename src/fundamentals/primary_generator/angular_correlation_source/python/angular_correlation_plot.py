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

from angular_correlation import angular_correlation

class AngularCorrelationPlot:
    def __init__(self, title, ini_sta, cas_ste):
        self.title=title
        self.ini_sta=ini_sta
        self.cas_ste=cas_ste

class AngularCorrelationPlotGrid:
    def __init__(self, ang_corr_list):
        self.ang_corr_list = ang_corr_list

    def plot(self, file_name):
        EULER_ANGLES = (0., 0.5*np.pi, 0.5*np.pi)

        N_THETA = 100
        N_PHI   = 100

        XLABEL = 'Beam direction'
        YLABEL = 'Polarization axis'
        ZLABEL = 'z'

        XLIM = (-2., 2.)
        YLIM = (-2., 2.)
        ZLIM = (-2., 2.)

        theta, phi = np.meshgrid(np.linspace(0., np.pi, N_THETA),
                                np.linspace(0., 2.*np.pi, N_PHI))

        n_lines = len(self.ang_corr_list)
        n_columns = len(self.ang_corr_list[0])

        ang_corr = np.zeros(np.shape(theta))
        x_y_z = np.zeros((3, np.shape(theta)[0], np.shape(theta)[1]))

        fig = plt.figure(figsize=(16, 8))

        for lin in range(n_lines):
            for col in range(n_columns):
                for i in range(N_THETA):
                    for j in range(N_PHI):

                        ang_corr[i][j] = angular_correlation(
                            theta[i][j], phi[i][j], 
                            self.ang_corr_list[lin][col].ini_sta,
                            self.ang_corr_list[lin][col].cas_ste,
                            *EULER_ANGLES
                        )

                        x_y_z[0][i][j] = ang_corr[i][j]*np.sin(theta[i][j])*np.cos(phi[i][j])
                        x_y_z[1][i][j] = ang_corr[i][j]*np.sin(theta[i][j])*np.sin(phi[i][j])
                        x_y_z[2][i][j] = ang_corr[i][j]*np.cos(theta[i][j])

                ax = fig.add_subplot(n_lines, n_columns, lin*n_columns+col + 1, projection='3d')
                ax.set_title(self.ang_corr_list[lin][col].title)
                ax.set_xlabel(XLABEL)
                ax.set_xlim(XLIM)
                ax.set_ylabel(YLABEL)
                ax.set_ylim(YLIM)
                ax.set_zlabel(ZLABEL)
                ax.set_zlim(ZLIM)


                # Note the importance of the choice of the color scheme.
                # While the 'rainbow' or 'jet' color schemes of matplotlib give appealing and colorful,
                # visualizations, they have been critized for not representing magnitudes well for 
                # human perception.
                # For a discussion of this issue, see
                # 
                # https://matplotlib.org/3.2.1/tutorials/colors/colormaps.html
                #
                # and references therein.
                color_map_max = np.max(ang_corr)
                color_map_min = np.min(ang_corr)
                color_map_norm = (ang_corr - color_map_min)/(color_map_max - color_map_min)
                color_map = mpl.cm.rainbow(color_map_norm)

                ax.plot_surface(x_y_z[0], x_y_z[1], x_y_z[2], facecolors=color_map)
                fig.colorbar(mpl.cm.ScalarMappable(norm=mpl.colors.Normalize(vmin=color_map_min, vmax=color_map_max), cmap=(plt.get_cmap('jet'))), shrink=0.8, label=r'$W(\theta, \varphi)$')

        plt.savefig(file_name)