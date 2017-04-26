//
// This file is part of the libWetHair open source project
//
// Copyright 2017 Yun (Raymond) Fei, Henrique Teles Maia, Christopher Batty,
// Changxi Zheng, and Eitan Grinspun
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef DEFAULT_XML_SCENE_H
#define DEFAULT_XML_SCENE_H

const static char* default_xml_scene_short_name = "default";

const static char* default_xml_scene_name = "default.xml";

const static char* default_xml_str =
"<scene dim=\"3\">\n"
"  <simtype type=\"DiscreteElasticRods\"/>\n"
"  <description text=\"Two strands hanging.\"/>\n"
"  <camera dist=\"9.7485\" radius=\"3.33418\" fov=\"40\">\n"
"    <rotation x=\"0\" y=\"0\" z=\"0\" w=\"1\"/>\n"
"    <center x=\"0.05\" y=\"2.35\" z=\"0\"/>\n"
"  </camera>\n"
"  <duration time=\"4.0\"/>\n"
"  <integrator type=\"preconditioned-compliant-euler\" maxnewton=\"0\" criterion=\"1e-7\" maxiters=\"1000\"/>\n"
"  <liquid rho=\"1.0\" sigma=\"72.0\" type=\"cylinder\" theta=\"1.0471975512\" maxetaprop=\"6.0\" collisionstiffness=\"10000.0\" dampingmultiplier=\"0.000\" radiusmultiplier=\"1.5\" massupdate=\"momentum\" regularizershell=\"0.0\" heightsmooth=\"1.0\" dt=\"0.004\" hairstep=\"4\" swestep=\"4\"/>\n"
"  <fluidsim ox=\"-3.95\" oy=\"-1.65\" oz=\"-4.0\" width=\"8.0\" gx=\"48\" gy=\"48\" gz=\"48\" init=\"none\" rl=\"0.6\" lsv=\"0.5\">\n"
"    <boundary type=\"circle\" cx=\"0.0\" cy=\"2.35\" cz=\"0.0\" radius=\"3.25\" inside=\"1\"/>\n"
"  </fluidsim>\n"
"  <StrandParameters>\n"
"    <radius value=\"0.005\"/>\n"
"    <youngsModulus value=\"3.4e9\"/>\n"
"    <shearModulus value=\"5.4e7\"/>\n"
"    <density value=\"1.3\"/>\n"
"    <viscosity value=\"5e7\"/>\n"
"    <baseRotation value=\"0.0\"/>\n"
"    <accumulateWithViscous value=\"1\"/>\n"
"    <accumulateViscousOnlyForBendingModes value=\"1\"/>\n"
"  </StrandParameters>\n"
"  <StrandMaterialForces params=\"0\">\n"
"    <particle x=\"0.0 4.7 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" fixed=\"1\"/>\n"
"    <particle x=\"0.0 4.58537 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" fixed=\"1\"/>\n"
"    <particle x=\"0.0 4.47073 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 4.3561 0.0\" v=\"0.0 0.0 0.0\"   eta=\"0.05\" />\n"
"    <particle x=\"0.0 4.24146 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 4.12683 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 4.0122 0.0\" v=\"0.0 0.0 0.0\"   eta=\"0.05\" />\n"
"    <particle x=\"0.0 3.89756 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 3.78293 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 3.66829 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 3.55366 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 3.43902 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 3.32439 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 3.20976 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 3.09512 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 2.98049 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 2.86585 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 2.75122 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 2.63659 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 2.52195 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 2.40732 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 2.29268 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 2.17805 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 2.06341 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 1.94878 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 1.83415 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 1.71951 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 1.60488 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 1.49024 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 1.37561 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 1.26098 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 1.14634 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 1.03171 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 0.917073 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 0.802439 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 0.687805 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 0.573171 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 0.458537 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 0.343902 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 0.229268 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 0.114634 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.0 0.0 0.0\" v=\"0.0 0.0 0.0\" eta=\"0.05\"/>\n"
"  </StrandMaterialForces>\n"
"  <StrandMaterialForces params=\"0\">\n"
"    <particle x=\"0.05 4.7 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" fixed=\"1\"/>\n"
"    <particle x=\"0.05 4.58537 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" fixed=\"1\"/>\n"
"    <particle x=\"0.05 4.47073 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 4.3561 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 4.24146 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 4.12683 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 4.0122 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 3.89756 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 3.78293 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 3.66829 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 3.55366 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 3.43902 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 3.32439 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 3.20976 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 3.09512 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 2.98049 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 2.86585 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 2.75122 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 2.63659 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 2.52195 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 2.40732 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 2.29268 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 2.17805 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 2.06341 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 1.94878 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 1.83415 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 1.71951 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 1.60488 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 1.49024 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 1.37561 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 1.26098 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 1.14634 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 1.03171 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 0.917073 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 0.802439 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 0.687805 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 0.573171 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 0.458537 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 0.343902 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 0.229268 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 0.114634 0.0\" v=\"0.0 0.0 0.0\"  eta=\"0.05\" />\n"
"    <particle x=\"0.05 0.0 0.0\" v=\"0.0 0.0 0.0\" eta=\"0.05\"/>\n"
"  </StrandMaterialForces>\n"
"</scene>\n";


#endif
