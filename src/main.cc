#include <iostream>
#include "Util.h"
#include "Mesh.h"
#include "ModalAnalysis.h"
#include "NodeComponents.h"
#include "Eigen/Dense"


int main(int argc, char *argv[])
{
  // Define mesh
  double ro = 0.025*0.5;
  Eigen::MatrixXd m(4, 1); m << 1000, // Length [mm]
                                  ro, // Outer radius
                                 0.0, // Inner radius
                                   4; // Partition num

  m.row(0) = m.row(0)*0.001; // Convert from [mm] to [m]

  Mesh *mesh = new Mesh(m);
  mesh->setDensity(2770.0);
  mesh->setEmod((double) 69.0e9);


  // Setup modal analysis
  ModalAnalysis* modAnalysis = new ModalAnalysis(0.0f, mesh->elements);
  modAnalysis->printInfo();


  // Define machine elements
  Eigen::Matrix4d bearing_k1; bearing_k1 << 10e9,  0.0, 0.0, 0.0,
                                             0.0, 10e9, 0.0, 0.0,
                                             0.0,  0.0, 0.0, 0.0,
                                             0.0,  0.0, 0.0, 0.0;
  Bearing* bearing1 = new Bearing(bearing_k1);
  Disc* disc1       = new Disc(1, 0.5, 0.5);

  // Mount machine elements
  modAnalysis->addNodeComponent(1, *bearing1);
  modAnalysis->addNodeComponent(4, *bearing1);
  modAnalysis->addNodeComponent(2, *disc1);


  // Compute
  modAnalysis->solve();

  // Print solution
  util::printEigenvalues(modAnalysis->eigenSolution, "eigenvalues", 4);
  util::printEigenvalues(modAnalysis->eigenSolution, "eigenfrequencies", 4);

  // Cleanup
  delete mesh;
  delete modAnalysis;
}
