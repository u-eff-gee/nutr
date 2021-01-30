/*
    This file is part of nutr.

    nutr is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    nutr is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with nutr.  If not, see <https://www.gnu.org/licenses/>.

    Copyright (C) 2020, 2021 Udo Friman-Gayer
*/

/**
 * \brief Plot angular distributions using ROOT
 * 
 * This plot of the angular distribution is based on the 'tornado' tutorial of ROOT, which can be 
 * found in:
 * 
 * ${ROOTSYS}/tutorials/graphics/tornado.C
 * 
 * This tutorial plots a three-dimensional object which looks like a tornado by defining every
 * single 3D point as an (x, y, z) tuple.
 * Since this is the most straightforward way of specifying an object in 3D, it is also 
 * straightforward to define a custom color scheme.
 * For the present application, it is desirable to have a color scheme for the deviation of the
 * angular distribution from sphere, which illustrates the non-isotropic character.
 * The code below shows how to achieve this.
 * 
 * However, this method of plotting a surface in 3D has the serious drawback that the visualization
 * will not know in which order to plot the points.
 * For example, if one plots a box and looks at it from the top, it is not guaranteed that the
 * points on the top of the box are plotted in front of the points at the bottom of the box.
 * This produces strange optical illusions, which make the plotted shapes difficult to interpret.
 * To avoid this, one would have to change the order of plotting for each desired view point 
 * separately, which is complicated.
 * In fact, this shadowing behavior can also be seen in the ROOT tornado example, where it 
 * is impossible to view the tornado from the tip.
 * The largest circle is plotted last, therefore it is always in front of the others.
 */

#include <iostream>

#include "TApplication.h"
#include "TArrayI.h"
#include "TAxis3D.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TPolyMarker3D.h"
#include "TStyle.h"
#include "TView.h"
#include "TView3D.h"

#include "SpherePointSampler.hh"
#include "W_pol_dir.hh"

int main(int argc, char *argv[]){

    /*** Set up canvas for plotting ***/
    TApplication* app = new TApplication("Plots", &argc, argv);
    TCanvas *canvas = new TCanvas("Angular Distribution");
    TPad *p1 = new TPad("p1", "p1", 0., 0., 1., 1.);
    p1->Draw();
    p1->cd();
    TView3D *view = (TView3D*) TView::CreateView(1);
    
    /*** Customize plot ***/
    
    // Color scheme
    gStyle->SetPalette(kRainBow);
    const TArrayI color_palette = TColor::GetPalette();
    const int n_colors = color_palette.GetSize();
    const double plot_range = 2.5;

    // Range
    view->SetRange(-plot_range, -plot_range, -plot_range, plot_range, plot_range, plot_range);
    view->ShowAxis();
    const double range_color_conversion = (double) n_colors / plot_range;
    //p1->GetView()->RotateView(-0., 0.);

    // Axis color
    TAxis3D *axis = TAxis3D::GetPadAxis();
    axis->GetXaxis()->SetTitle("x");
    axis->GetYaxis()->SetTitle("y");
    axis->GetZaxis()->SetTitle("z");
    axis->SetLabelColor(kBlack);

    /*** Sample grid points ***/
    const unsigned int n = 2000;
    SpherePointSampler sph_pt_samp;

    array<vector<double>, 2> theta_phi = sph_pt_samp.sample(n);

    /*** Evaluate function on grid and plot points ***/
    double sine_theta{0.}, w{0.};

    W_pol_dir w_pol_dir(
		State(0, positive), 
        {
            {Transition(electric, 4, magnetic, 6, 0.), State(4, positive)},
            {Transition(electric, 4, magnetic, 6, 0.), State(0, positive)}
        }
    );

    TPolyMarker3D *marker = nullptr;

    for(size_t i = 1; i <= n; ++i){

        sine_theta = sin(sin(theta_phi[0][i]));
        w = w_pol_dir(theta_phi[0][i], theta_phi[1][i]);

        marker = new TPolyMarker3D(1);
        marker->SetPoint(1, 
            w*sine_theta*cos(theta_phi[1][i]), 
            w*sine_theta*sin(theta_phi[1][i]),
            w*cos(theta_phi[0][i])
        );

        marker->SetMarkerColor(color_palette[(int) (w*range_color_conversion)]);
        marker->SetMarkerStyle(20);
        marker->Draw();
    }

    canvas->Modified();
    canvas->Update();
    canvas->SaveAs("test_plots_tpolymarker3d.pdf");    

    delete app;
    // Replace 'delete app' by the following statement to get an interactive view.
    // app->Run();
}