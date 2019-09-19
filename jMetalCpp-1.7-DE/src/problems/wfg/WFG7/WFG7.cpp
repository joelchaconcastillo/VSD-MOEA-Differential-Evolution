#include <vector>
#include <math.h>

#include "WFG7.h"
using namespace std;
// Constantes y variables estaticas
const double PI = 3.1415926535897932384626433832795;
int WFG7::k;
WFG7::WFG7(string solutionType, int numberOfVariables, int numberOfObjectives) {
	init(numberOfObjectives);
	numberOfVariables_ = numberOfVariables;
	numberOfObjectives_  = numberOfObjectives;
	numberOfConstraints_ = 0;
	problemName_ 				 = "WFG7";

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
		lowerLimit_[i] = 0.0;
		upperLimit_[i] = 2*(i+1);
	}

	if(solutionType.compare("Real") == 0) {
		solutionType_ = new RealSolutionType(this) ;
		//cout << "Tipo seleccionado Real" << endl;
	}
	else if (solutionType.compare("ArrayReal") == 0)
		solutionType_ = new ArrayRealSolutionType(this) ;
	else {
		cout << "Error: solution type " << solutionType << " invalid" << endl;
		exit(-1) ;
	}
}
void WFG7::evaluate (Solution *solution) {
	
        Variable **variables = solution->getDecisionVariables();
	// Inicializa el vector y
  	vector<double> X(numberOfVariables_);
	for (int i = 0; i < numberOfVariables_; i++)
    		X[i] = variables[i]->getValue() ;



	vector<double> y = WFG_normalise_z( X );
	vector<double> F(numberOfObjectives_); 
	// Aplica las transiciones al vector y
	y = WFG7_t1(y, k);
	y = WFG7_t2(y, k);
	y = WFG7_t3(y, k, numberOfObjectives_);
	// Calcula los valores de las funciones objetivo
	F = WFG7_shape(y);
	for (int i = 0; i < F.size(); i++)
	   solution->setObjective(i, F[i]);	
//	cout << Obj[i]<<endl;
	//  setObj(i, result[i]);
}
/**
 * Destructor
 */
WFG7::~WFG7() {
  delete [] lowerLimit_ ;
  delete [] upperLimit_ ;
  delete solutionType_ ;
} // ~WFG7


bool WFG7::init (int nObj) {
	this->nObj = nObj;
	if (nObj < 2) {
	  cout << "Error WFG7: Parameter number_of_objectives must be greater than 2" << endl;
		return false;
	}

	if (nObj >= 3)
		k = 2 * (nObj - 1);
	else
	  k = 4;
	this->nVar = k+l;
//	setNumberOfVar(k + l);  // Parametros de posicion + Parametros de distancia
	//setNumberOfObj(nObj);   // Numero de objetivos
	return true;
}
// Evaluacion de un individuo WFG7
void WFG7::evaluate (vector<double> &X, vector<double> &Obj) {

	// Inicializa el vector y
	 vector<double> y = WFG_normalise_z( X );
  	// Aplica las transiciones al vector y
	y = WFG7_t1(y, k);
	y = WFG7_t2(y, k);
	y = WFG7_t3(y, k, this->nObj);

	// Calcula los valores de las funciones objetivo
	Obj = WFG7_shape(y);

	// Fija dichos valores
//	for (int i = 0; i < result.size(); i++)
//	  setObj(i, result[i]);

}
				
// Clonacion de un individuo WFG7
//Individual* WFG7::clone (void) const {
//	return new WFG7();
//}

/******************************************************************************
 * Funciones auxiliares al problema
 * ****************************************************************************/

// Escala un valor al intervalo [0, 1]
double WFG7::correct_to_01 (const double &a, const double &epsilon) {

  const double min = 0.0;
	const double max = 1.0;

	const double min_epsilon = min - epsilon;
	const double max_epsilon = max + epsilon;

  if ((a <= min) && (a >= min_epsilon)) {
	  return min;
 	}
  else if ((a >= max) && (a <= max_epsilon)) {
    return max;
  }
  else {
    return a;
	}
}

// Construye un vector con los elementos v[head], ..., v[tail - 1]
vector<double> WFG7::subvector (const vector<double> &v, const int head, const int tail) {

  vector< double > result;

	for(int i = head; i < tail; i++) {
    result.push_back(v[i]);
 	}

  return result;
}

vector<double> WFG7::calculate_x (const vector<double> &t_p, const vector<short> &A) {

  vector<double> result;

	for (int i = 0; i < t_p.size() - 1; i++) {
		const double tmp1 = max<double>(t_p.back(), A[i]);
	 	result.push_back(tmp1 * (t_p[i] - 0.5) + 0.5); 
	}
 
 	result.push_back(t_p.back());
	return result;
}

vector<double>  WFG7::calculate_f (const double &D, const vector<double> &x, const vector<double> &h,
                                   const vector<double> &S) {

  vector< double > result;

  for (int i = 0; i < h.size(); i++) {
    result.push_back(D * x.back() + S[i] * h[i]);
  }

  return result;
}

/******************************************************************************
 * Transiciones genericas
 *****************************************************************************/

double WFG7::b_param (const double &y, const double &u, const double &A, const double &B, const double &C) {
  const double v = A - (1.0 - 2.0 * u) * fabs(floor(0.5 - u) + A);
  return correct_to_01(pow(y, B + (C - B) * v));
}

double WFG7::s_linear (const double &y, const double &A) {
  return correct_to_01(fabs(y - A) / fabs(floor(A - y) + A));
}

double WFG7::r_sum (const vector<double> &y, const vector<double> &w) {

  double numerator   = 0.0;
  double denominator = 0.0;

  for(int i = 0; i < y.size(); i++) {
	  numerator   += w[i] * y[i];
	  denominator += w[i];
	}

	return correct_to_01(numerator / denominator);
}

/******************************************************************************
 * Superficies genericas
 * ****************************************************************************/

double WFG7::concave(const vector<double> &x, const int m) {

  const int M = x.size();
  double result = 1.0;

  for (int i = 1; i <= M - m; i++) {
    result *= sin(x[i - 1] * PI / 2.0);
  }

  if( m != 1 ) {
    result *= cos(x[M - m] * PI / 2.0);
  }

  return correct_to_01(result);
}

/******************************************************************************
 * Funciones propias del problema WFG7
 * ****************************************************************************/

// Primera transicion para WFG7
vector<double> WFG7::WFG7_t1 (const vector<double> &y, const int k) {

  const int n = y.size();
  const vector<double> w(n, 1.0);
  vector<double> t;

  for (int i = 0; i < k; i++) {
    const vector<double> &y_sub = subvector(y, i + 1, n);
    const vector<double> &w_sub = subvector(w, i + 1, n);
    const double u = r_sum(y_sub, w_sub);
    t.push_back(b_param(y[i], u, 0.98 / 49.98, 0.02, 50));
  }

  for (int i = k; i < n; i++) {
    t.push_back(y[i]); 
  } 
  
	return t;
}

// Segunda transicion para WFG7
vector<double> WFG7::WFG7_t2 (const vector<double> &y, const int k) {

  const int n = y.size();
  vector<double> t;

  for(int i = 0; i < k; i++) {
    t.push_back(y[i]);
 	}

  for(int i = k; i < n; i++) {
    t.push_back(s_linear(y[i], 0.35));
	}

  return t;

}

// Tercera transicion para WFG7
vector<double> WFG7::WFG7_t3 (const vector<double> &y, const int k, const int M) {
														  
  const int n = y.size();
  const vector<double>  w(n, 1.0);
  vector< double > t;

  for (int i = 1; i <= M - 1; i++) {
    const int head = (i - 1) * k / (M - 1);
    const int tail = i * k / (M - 1);
    const vector<double> &y_sub = subvector(y, head, tail);
    const vector<double> &w_sub = subvector(w, head, tail);
    t.push_back(r_sum(y_sub, w_sub));
  }

  const vector<double> &y_sub = subvector(y, k, n);
  const vector<double> &w_sub = subvector(w, k, n);

  t.push_back(r_sum(y_sub, w_sub));

  return t;

}

// Superficie para WFG7
vector<double> WFG7::WFG7_shape (const vector<double> &t_p) {

  const int M = t_p.size();
  const vector<short> &A = WFG_create_A(M, false);
  const vector<double> &x = calculate_x(t_p, A);
  vector<double> h;

  for(int m = 1; m <= M; m++) {
    h.push_back(concave(x, m));
 	}

	return WFG_calculate_f(x, h);

}

/******************************************************************************
 * Funciones propias de los problemas WFG
 * ****************************************************************************/

// Construye un vector de longitud M - 1 con los valores "1,0,0,..." si
// "degenerate" es verdadero. En otro caso, lo construye con los valores
// "1,1,1,..."
vector<short> WFG7::WFG_create_A (const int M, const bool degenerate) {
    
  if (degenerate) {
    vector<short> A(M - 1, 0);
    A[0] = 1;
    return A;
  }
  else {
    return vector<short>(M - 1, 1);
  }
}

// Calcula los valores de fitness escalados para un problema WFG 
vector<double> WFG7::WFG_calculate_f (const vector< double >& x, const vector< double >& h) {
  
  const int M = h.size();
	vector<double> S; 
	
	for(int m = 1; m <= M; m++) {
    S.push_back(m * 2.0);
 	}

	return calculate_f(1.0, x, h, S);
}
vector< double > WFG7::WFG_normalise_z( const vector< double >& z )
{
  vector< double > result;

  for( int i = 0; i < static_cast< int >( z.size() ); i++ )
  {
    const double bound = 2.0*( i+1 );

    result.push_back( z[i] / bound );
  }

  return result;
}
