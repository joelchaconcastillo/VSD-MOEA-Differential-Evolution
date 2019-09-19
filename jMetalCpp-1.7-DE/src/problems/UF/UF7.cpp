//  UF7.cpp
//
//  Author:
//       Antonio J. Nebro <antonio@lcc.uma.es>
//
//  Copyright (c) 2011 Antonio J. Nebro, Juan J. Durillo
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <UF7.h>

const double UF7::PI = 3.141592653589793;

UF7::UF7(string solutionType, int numberOfVariables, int numberOfObjectives) {
	numberOfVariables_   = numberOfVariables;
	numberOfObjectives_  = numberOfObjectives;
	numberOfConstraints_ = 0;
	problemName_ 				 = "UF7";

	lowerLimit_ = new double[numberOfVariables_];//(double *)malloc(sizeof(double)*numberOfVariables);
	if (lowerLimit_ == NULL) {
		cout << "Impossible to reserve memory for storing the variable lower limits" << endl;
		exit(-1);
	}

	upperLimit_ = new double[numberOfVariables_];//(double *)malloc(sizeof(double)*numberOfVariables);
	if (upperLimit_ == NULL) {
		cout << "Impossible to reserve memory for storing the variable lower limits" << endl;
		exit(-1);
	}

	for (int i = 0; i < numberOfVariables_; i++) {
		lowerLimit_[i] = -2.0;
		upperLimit_[i] = 2.0;
	}
		lowerLimit_[0] = 0.0;
		upperLimit_[0] = 1.0;

	if (solutionType.compare("BinaryReal") == 0)
		solutionType_ = new BinaryRealSolutionType(this) ;
	else if (solutionType.compare("Real") == 0) {
		solutionType_ = new RealSolutionType(this) ;
		//cout << "Tipo seleccionado Real" << endl;
	}
	else if (solutionType.compare("ArrayReal") == 0)
		solutionType_ = new ArrayRealSolutionType(this) ;
	else {
		cout << "Error: solution type " << solutionType << " invalid" << endl;
		exit(-1) ;
	}

	fx_ = new double[numberOfObjectives_] ;
  x_ = new double[numberOfVariables_];
}

UF7::~UF7() {
  delete [] lowerLimit_ ;
  delete [] upperLimit_ ;
  delete solutionType_ ;
	delete [] fx_ ;
	delete [] x_ ;
}

/**
 * Evaluates a solution
 * @param solution The solution to evaluate
 */
void UF7::evaluate(Solution *solution) {
	XReal * vars = new XReal(solution);


       for (int i = 0; i < numberOfVariables_; i++)
        x_[i] = vars->getValue(i);

	unsigned int nx = numberOfVariables_;

	unsigned int j, count1, count2;

	double sum1, sum2, yj;
	sum1   = sum2   = 0.0;
	count1 = count2 = 0;
	for(j = 2; j <= nx; j++) 
	{
		yj = x_[j-1] - sin(6.0*PI*x_[0]+j*PI/nx);
		if (j % 2 == 0) 
		{
			sum2  += yj*yj;
			count2++;
		} 
		else 
		{
			sum1  += yj*yj;
			count1++;
		}
	}
	yj = pow(x_[0],0.2);
	fx_[0] = yj+ 2.0*sum1 / (double)count1;
	fx_[1] = 1.0 - yj + 2.0*sum2 / (double)count2;

  for (int i = 0; i < numberOfObjectives_; i++)
    solution->setObjective(i, fx_[i]);

	delete vars ;
} // evaluate
