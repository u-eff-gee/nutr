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

    Copyright (C) 2020 Udo Friman-Gayer
*/

/**
 * \brief Plot angular distributions using OpenGl rendering
 * 
 * The OpenGL bindings of ROOT are probably the quickest way to plot a closed surface in three
 * dimensions.
 * Note the brevity of the code below, which simply defines a wrapper for the function and the 
 * ranges of the angles.
 * With this minimalistic input, a very smooth plot is obtained.
 * 
 * I found this possibility by browsing the ROOT user forum for suggestions on 3D plotting 
 * of surfaces after I tried TGraph2D and TH2 first.
 *      * The TGraph2D class has the drawback that it is not made for an intuitive drawing of
 * closed surfaces.
 * While the drawing of functions of the type z = f(x, y) on a grid of x- and y values works quite
 * well, the associated Delaunay triangulation algorithm fails to draw closed surfaces between a 
 * given set of (x, y, z) tuples and the plotted objects
 * look like crumpled paper instead of having a smooth surface.
 *      * In principle, the TH2 method employs all the usual methods for histogram drawing, and I expected
 *  that the "SPH" drawing option, which is supposed to plot the histogram data in a 
 * spherical coordinate system was convenient.
 * However, there is almost no documentation on what the "SPH" option actually does, and I
 * failed at plotting even the most simple shapes because the definition of the angles 
 * \f$\theta\f$ and \f$\varphi\f$ is not obvious.
 * There are even discussions in the ROOT forum from several years ago where people asked whether
 * the manual confuses 'latitude' and 'longitude'.
 * No matter if it does or not, the entire option seemed confusing enough for me to not pursue this
 * way further.
 * 
 * Despite its ease of use, the OpenGL method with a TGLParametricEquation has some serious drawbacks:
 * 
 *   * There is almost no documentation. The only two examples that use this class do exactly what is shown below and nothing more. Without diving deeply into OpenGL itself, I think there is no way to find out how to customize (axes, labels, colors, ...) plots. The missing documentation and examples are a general problem for the OpenGL bindings. The TGLParametricEquation in particular derives completely from the extremely general classes TNamed and TObject and has few methods of its own, so there is no way to find any customization options by browsing the class hierarchy.
 *   * The angular correlations/distributions cannot be passed to TGLParametricEquation easily. To avoid repeated computations of the same quantity, the classes are implemented as 'functors', i.e. classes with an operator() method which can be called like functions. I tried several ways to pass an angular distribution in the format that is required by TGLParametricEquation: wrapping it in another functor (both as a static function or as a nonstatic function), wrapping it in a lambda function, wrapping it in a std::function, but none of them worked. The working example below shows a primitive implementation using a globally defined object and function. This makes it complicated to loop over different angular distributions. As I see it, that would require preprocessor or template programming.
 */

#include "TApplication.h"
#include "TCanvas.h"
#include "TGLParametric.h"
#include "TGLUtil.h"
#include "TGLViewer.h"
#include "TMath.h"
#include "TStyle.h"

#include "W_pol_dir.hh"

W_pol_dir w_pol_dir(
    State(0, positive), 
    {
        {Transition(electric, 4, magnetic, 6, 0.), State(4, positive)}, 
        {Transition(electric, 4, magnetic, 6, 0.), State(0, positive)}
    }
);

void angdist_wrapper(TGLVertex3 &dst, double theta, double phi) {

    dst.X() = w_pol_dir(theta, phi)*sin(theta)*cos(phi);
    dst.Y() = w_pol_dir(theta, phi)*sin(theta)*sin(phi);
    dst.Z() = w_pol_dir(theta, phi)*cos(theta);
}

int main(int argc, char *argv[]){

    TApplication* app = new TApplication("Plots", &argc, argv);
    gStyle->SetCanvasPreferGL(kTRUE);
    TCanvas *canvas = new TCanvas("Angular Distribution");
    TGLParametricEquation *peq = new TGLParametricEquation("Angular Distribution", angdist_wrapper, 0., TMath::Pi(), 0., TMath::TwoPi());
    peq->Draw();
    canvas->Modified();
    canvas->Update();
    // Did not work to output a PDF image, so chose PNG instead.
    canvas->SaveAs("test_plots_tgl.png");
    
    delete app;
    // Replace 'delete app' by the following statement to get an interactive view.
    // app->Run();
}