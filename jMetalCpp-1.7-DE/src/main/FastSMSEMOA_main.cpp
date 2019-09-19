//  FastSMSEMOA_main.cpp
//
//  Author:
//        Esteban López-Camacho <esteban@lcc.uma.es>
//
//  Copyright (c) 2014 Antonio J. Nebro
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


#include <Problem.h>
#include <Solution.h>
#include <SBXHybridCrossover.h>
#include <SBXCrossover.h>
#include <DifferentialEvolutionCrossover.h>
#include <DifferentialEvolutionSelection.h>
#include <PolynomialMutation.h>
#include <RandomSelection.h>
#include <iostream>
#include <FastSMSEMOA.h>
#include <ProblemFactory.h>
#include <string.h>
#include <time.h>



int main(int argc, char ** argv) {

  clock_t t_ini, t_fin;
  
  Problem   * problem   ; // The problem to solve
  Algorithm * algorithm ; // The algorithm to use
  Operator  * crossover ; // Crossover operator
  Operator  * mutation  ; // Mutation operator
  Operator  * selection ; // Selection operator
  int Seed = 1;
   string DirReport;
// Los primeros dos argumentos es el nombre del programa + Nombre del algoritmo + Instancia + Numero de variables + Numero de objetivos + Semilla
   if(argc == 7)
   {
	Seed = atoi(argv[5]);
	argc--;
	DirReport = string(argv[6]);
	argc--;
   }

  if (argc>=2) {
    problem = ProblemFactory::getProblem(argc, argv);
    cout << "Selected problem: " << problem->getName() << endl;
  } else {
    cout << "No problem selected." << endl;
    cout << "Default problem will be used: Fonseca" << endl;
    problem = ProblemFactory::getProblem(const_cast<char *>("Fonseca"));
  }
srand(Seed);
  RandomGenerator::SEED = Seed;
  //TODO: Quality Indicators
  //QualityIndicator indicators ; // Object to get quality indicators
  //indicators = null ;

  algorithm = new FastSMSEMOA(problem);

  // Algorithm parameters
  int populationSize = 100;
  int generations = 250000;
  //int generations = 1000;
  int maxEvaluations= generations*populationSize;
  double offset = 100;
  string FileNameObjective = DirReport + "/OBJ_FastSMSEMOA_SBX_POLYNOMIAL_EVALUATIONS_" +to_string(maxEvaluations) +"_"+problem->getName() +"_SEED_"+to_string(Seed)+"_"+to_string(problem->getNumberOfObjectives())+".txt";
  string FileNameVariable = DirReport + "/VAR_FastSMSEMOA_SBX_POLYNOMIAL_EVALUATIONS_" +to_string(maxEvaluations) +"_"+problem->getName() +"_SEED_"+to_string(Seed)+"_"+to_string(problem->getNumberOfObjectives())+".txt";

  algorithm->setInputParameter("FileNameObjective",&FileNameObjective);
  algorithm->setInputParameter("FileNameVariable",&FileNameVariable);
  algorithm->setInputParameter("Seed",&Seed);

  algorithm->setInputParameter("populationSize",&populationSize);
  algorithm->setInputParameter("maxEvaluations",&maxEvaluations);
  algorithm->setInputParameter("offset",&offset);

  // Mutation
  map<string, void *> parameters;
//  double crossoverProbability = 0.9;
//  double crossoverDistributionIndex = 20.0;
//  parameters["probability"] =  &crossoverProbability;
//  parameters["distributionIndex"] = &crossoverDistributionIndex;
//  crossover = new SBXCrossover(parameters);
//  //crossover = new SBXHybridCrossover(parameters);
//
//  //Crossover
//  parameters.clear();
//  double mutationProbability = 1.0/problem->getNumberOfVariables();
//  double mutationDistributionIndex = 50.0;
//  parameters["probability"] = &mutationProbability;
//  parameters["distributionIndex"] = &mutationDistributionIndex;
//  mutation = new PolynomialMutation(parameters);



  double crParameter = 0.9;
  double fParameter  = 0.5;
  //double fParameter  = 1.0/(problem->getNumberOfVariables());
  parameters["CR"] =  &crParameter;
  parameters["F"] = &fParameter;
  crossover = new DifferentialEvolutionCrossover(parameters);

  // Selection Operator
  parameters.clear();
 selection = new DifferentialEvolutionSelection(parameters) ;

  parameters.clear();
  double mutationProbability = 1.0/problem->getNumberOfVariables();
  double mutationDistributionIndex = 50.0;
  parameters["probability"] = &mutationProbability;
  parameters["distributionIndex"] = &mutationDistributionIndex;
  mutation = new PolynomialMutation(parameters);


	// Add the operators to the algorithm
  algorithm->addOperator("crossover",crossover);
  algorithm->addOperator("selection",selection);
  algorithm->addOperator("mutation",mutation);

  // Add the indicator object to the algorithm
  //algorithm->setInputParameter("indicators", indicators) ;

	// Execute the Algorithm
	t_ini = clock();

	SolutionSet * population = algorithm->execute();

	t_fin = clock();
	double secs = (double) (t_fin - t_ini);
	secs = secs / CLOCKS_PER_SEC;

	// Result messages
	cout << "Total execution time: " << secs << "s" << endl;
	cout << "Variables values have been written to file VAR" << endl;
	//population->printVariablesToFile("VAR");
	cout << "Objectives values have been written to file FUN" << endl;
	//population->printObjectivesToFile("FUN");

	delete mutation;
	delete crossover;
	delete selection;
  delete population;
	delete algorithm;

} // main
