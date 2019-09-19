
#ifndef __INDIVIDUALWFG2_H__
#define __INDIVIDUALWFG2_H__


#include <iostream>
#include <math.h>
#include <vector>
#include <Problem.h>
#include <BinaryRealSolutionType.h>
#include <RealSolutionType.h>
#include <ArrayRealSolutionType.h>
#include <XReal.h>
#include <Solution.h>


//#include "Individual.h"
using namespace std;

class WFG2: public Problem {
 
	public:
  
	// Funcion de inicializacion
	//bool init (const vector<string> &params);
	bool init (int nObj);
	
	void evaluate(Solution *solution);
	virtual ~WFG2();
	WFG2(string solutionType, int numberOfVariables = 24, int numberOfObjectives = 2);
	
	// Evaluacion y seleccion del individuo
	//void evaluate (void);
	void evaluate (vector<double> &X, vector<double> &Obj);
	
	// Clonacion
	//Individual* clone (void) const;
	
	// Rangos
	double inline getMaximum(const int i) const { return 1; }
	double inline getMinimum(const int i) const { return 0; }
	//unsigned int inline getOptDirection(const int i) const { return MINIMIZE; }
	inline int getDimension(){return this->nVar;}
	vector< double > WFG_normalise_z( const vector< double >& z );

 
  private:
	
	// Funciones auxiliares al problema
	vector<double> subvector(const vector<double> &v, const int head, const int tail);
	double correct_to_01(const double &a, const double &epsilon = 1.0e-10);
	vector<double> calculate_x (const vector<double> &t_p, const vector<short> &A);
  vector<double> calculate_f (const double &D, const vector<double> &x, const vector<double> &h,
								            	const vector<double> &S);

	// Transiciones genericas
	double s_linear (const double &y, const double &A);
	double r_nonsep(const vector<double> &y, const int A);
	double r_sum (const vector<double> &y, const vector<double> &w);
	
  // Superficies genericas
	double convex(const vector<double> &x, const int m);
  double disc (const vector<double> &x, const int A, const double &alpha, const double &beta);

	// Transiciones del problema WFG2
	vector<double> WFG2_t1(const vector<double> &y, const int k);
	vector<double> WFG2_t2(const vector<double> &y, const int k);
	vector<double> WFG2_t3(const vector<double> &y, const int k, const int M);
	
	// Superficie del problema WFG2
	vector<double> WFG2_shape(const vector<double> &t_p);

	// Funciones propias de los problemas WFG
	vector<short> WFG_create_A (const int M, const bool degenerate);
	vector<double> WFG_calculate_f (const vector< double >& x, const vector< double >& h);
	
	// Constantes del problema WFG2
	static int k;             // Parametros relacionados con la posicion
	static const int l = 20;  // Parametros relacionados con la distancia
//	static const int l = 87;  // Parametros relacionados con la distancia
	int nObj, nVar;

};

#endif
