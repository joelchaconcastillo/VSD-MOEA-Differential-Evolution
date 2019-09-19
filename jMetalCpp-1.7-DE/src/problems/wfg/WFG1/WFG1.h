#ifndef __INDIVIDUALWFG1_H__
#define __INDIVIDUALWFG1_H__


#include <iostream>
#include <math.h>
#include <vector>
#include <Problem.h>
#include <BinaryRealSolutionType.h>
#include <RealSolutionType.h>
#include <ArrayRealSolutionType.h>
#include <XReal.h>
#include <Solution.h>



using namespace std;
//#include "Individual.h"

class WFG1: public Problem {
 
  public:
  
	// Funcion de inicializacion
	bool init (int nObj);
	void evaluate(Solution *solution);
	virtual ~WFG1();
	WFG1(string solutionType, int numberOfVariables = 24, int numberOfObjectives = 2);
	// Evaluacion y seleccion del individuo
	void evaluate (vector<double> &X, vector<double> &Obj);
	// Rangos
	double inline getMaximum(const int i) const { return 1; }
	double inline getMinimum(const int i) const { return 0; }
//	unsigned int inline getOptDirection(const int i) const { return MINIMIZE; }
	inline int getDimension(){return this->nVar;}
 
  private:

	// Funciones auxiliares al problema
	vector<double> subvector(const vector<double> &v, const int head, const int tail);
	double correct_to_01(const double &a, const double &epsilon = 1.0e-10);
	vector<double> calculate_x (const vector<double> &t_p, const vector<short> &A);
  vector<double> calculate_f (const double &D, const vector<double> &x, const vector<double> &h,
								            	const vector<double> &S);

	// Transiciones genericas
	double s_linear (const double &y, const double &A);
	double b_poly(const double &y, const double &alpha);
	double b_flat (const double &y, const double &A, const double &B, const double &C);
	double r_sum (const vector<double> &y, const vector<double> &w);
	
  // Superficies genericas
	double convex(const vector<double> &x, const int m);
	double mixed(const vector<double> &x, const int A, const double &alpha);

	// Transiciones del problema WFG1
	vector<double> WFG1_t1(const vector<double> &y, const int k);
	vector<double> WFG1_t2(const vector<double> &y, const int k);
	vector<double> WFG1_t3(const vector<double> &y);
	vector<double> WFG1_t4(const vector<double> &y, const int k, const int M);
	
	// Superficie del problema WFG1
	vector<double> WFG1_shape(const vector<double> &t_p);

	// Funciones propias de los problemas WFG
	vector<short> WFG_create_A (const int M, const bool degenerate);
	vector<double> WFG_calculate_f (const vector< double >& x, const vector< double >& h);
	
	vector< double > WFG_normalise_z( const vector< double >& z );
	// Constantes del problema WFG1
	static int k;             // Parametros relacionados con la posicion
//	static const int l = 87;  // Parametros relacionados con la distancia
	//static const int l = 18;  // Parametros relacionados con la distancia
	static const int l = 20;  // Parametros relacionados con la distancia
       
	int nObj, nVar;
};

#endif
