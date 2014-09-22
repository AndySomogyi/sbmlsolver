/**
 * Filename    : render-package.i
 * Description : render swig file for bindings.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, WITHOUT EVEN THE IMPLIED WARRANTY OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  The software and
 * documentation provided hereunder is on an as is" basis, and the European
 * Media Laboratories gGmbH have no obligations to provide maintenance,
 * support, updates, enhancements or modifications.  In no event shall the
 * European Media Laboratory gGmbH be liable to any party for direct,
 * indirect, special, incidental or consequential damages, including lost
 * profits, arising out of the use of this software and its documentation,
 * even if the European Media Laboratories gGmbH have been advised of the
 * possibility of such damage.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 */

#ifdef USE_RENDER

%template(RenderPkgNamespaces) SBMLExtensionNamespaces<RenderExtension>;

%ignore Transformation2D::readAttributes;
%ignore GraphicalPrimitive1D::getDashArray;
%ignore GraphicalPrimitive1D::setDashArray;

%ignore operator<<;
%ignore operator+;
%ignore operator/;

%include <sbml/packages/render/extension/RenderExtension.h>
%include <sbml/packages/render/extension/RenderListOfLayoutsPlugin.h>
%include <sbml/packages/render/extension/RenderLayoutPlugin.h>
%include <sbml/packages/render/extension/RenderGraphicalObjectPlugin.h>

%include <sbml/packages/render/sbml/Transformation.h>
%include <sbml/packages/render/sbml/Transformation2D.h>
%include <sbml/packages/render/sbml/GraphicalPrimitive1D.h>
%include <sbml/packages/render/sbml/GraphicalPrimitive2D.h>
%include <sbml/packages/render/sbml/RenderInformationBase.h>
%include <sbml/packages/render/sbml/GradientBase.h>
%include <sbml/packages/render/sbml/Style.h>

%include <sbml/packages/render/sbml/ColorDefinition.h>
%include <sbml/packages/render/sbml/RelAbsVector.h>
%include <sbml/packages/render/sbml/Ellipse.h>
%include <sbml/packages/render/sbml/GlobalRenderInformation.h>
%include <sbml/packages/render/sbml/GlobalStyle.h>
%include <sbml/packages/render/sbml/GradientStop.h>
%include <sbml/packages/render/sbml/Image.h>
%include <sbml/packages/render/sbml/Text.h>
%include <sbml/packages/render/sbml/Rectangle.h>
%include <sbml/packages/render/sbml/RenderPoint.h>
%include <sbml/packages/render/sbml/RenderCubicBezier.h>
%include <sbml/packages/render/sbml/RenderCurve.h>
%include <sbml/packages/render/sbml/Polygon.h>
%include <sbml/packages/render/sbml/RenderGroup.h>
%include <sbml/packages/render/sbml/LinearGradient.h>
%include <sbml/packages/render/sbml/LineEnding.h>
%include <sbml/packages/render/sbml/ListOfCurveElements.h>
%include <sbml/packages/render/sbml/LocalRenderInformation.h>
%include <sbml/packages/render/sbml/LocalStyle.h>
%include <sbml/packages/render/sbml/RadialGradient.h>

%include std_deque.i
%include std_set.i
%include std_string.i

%template(StringSet) std::set<std::string>;


%include carrays.i
%array_class(double,double_array)
%array_class(int,int_array)

#endif /* USE_RENDER */