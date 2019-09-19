#ifndef MEASURES_HPP_INCLUDED
#define MEASURES_HPP_INCLUDED
#include <vector>
#include <map>
#define SPACING 100
#define SPREAD 101
#define HYPERVOLUME 102
#define HYPERVOLUMERATIO 103
#define GENERATIONALDISTANCE 104
#define INVERTEDGENERATIONALDISTANCE 105
#define CLOSESTDISTANCE 106
#define ALLSPACING 107
#define ALLSPREAD 108
#define ALLHYPERVOLUME 109
#define ALLHYPERVOLUMERATIO 110
#define ALLGENERATIONALDISTANCE 111
#define ALLINVERTEDGENERATIONALDISTANCE 112
#define ALLCLOSESTDISTANCE 113


using namespace std;
class Measures
{
    private:
        vector<vector<double> > SetPareto;
        vector<vector<double> > ParetoOptimalFront;
        vector<vector<double> > Variables;
        vector<int> IndexBoundsObjectives;
	vector<double> MinimumDistance(vector<vector<double>> &Front1, vector<vector<double>> &Front2);
        /**
            Obtiene las distancia entre las soluciones
        **/
        vector<double> DistanceSolutions();
        /**

        **/
        double EuclideanDistance(vector<double> SolutionA, vector<double> SolutionB );
        double DifferenceAbsolute(vector<double> SolutionA, vector<double> SolutionB );
        double Mean(vector<double> &data);
        double Sum(vector<double> &data);
        vector<double> DistancesExtremes();
	vector<double> OwnMinimalDifference(vector<vector<double>> &Front);

        struct ObjetiveSort
        {
            bool operator()(vector<double> A, vector<double> B) { return A[1] > B[1];  }
        };
	
    public:
        Measures(vector<vector<double> >&SetPareto, vector<vector<double>> &ParetoOptimalFront);
	Measures(string FileSetPareto, string FileParetoOptimalFront);
	Measures(string FileParetoOptimalFront);
	Measures();
	vector<vector<double>> ReadFile(string FileName);

	vector<string> getFiles(string basename);

	void RuningMeasureGenerationalDistance(string FileSetPareto, ofstream &File);
	void RuningMeasureInvertedGenerationalDistance(string FileSetPareto, ofstream &File);
	void RuningMeasureSpread(string FileSetPareto, ofstream &File);
	void RuningMeasureHyperVolume(string FileSetPareto, ofstream &File);
	void RuningMeasureHyperVolumeRatio(string FileSetPareto, ofstream &File);
	void RuningMeasureSpacing(string FileSetPareto, ofstream &File);
	void RuningClosestDistance(string FilenameVariables, ofstream &File);
	void PrintObjectiveSpace(string FileSetPareto,  string FilenameSummary);

        /**
            Medicion de la convergencia
        **/
        double GenerationalDistance();
        double InvertedGenerationalDistance();

        /**
            Medicion de la dispersion
        **/
        double Spacing();
        double Spread();
        double HyperVolume();
        double HyperVolume(vector<double> &W);
        double HyperVolumeRatio();
        double HyperVolumeRatio(vector<double> &W);
        double HyperDistance(vector<double> &W);

	/**
	   Metricas de la diversidad en el espacio 
	   de las variables..
	**/
	double ClosestDistance(vector<vector<double>> &X);
	
	void StartMetrics( map<int, string> Configuration, string PathFiles ,string basename, string PathResult, string FP);
	string IdtoNameMetric(int Id);

	void NormalizeFront(vector<vector<double>> &Front, vector<double> &Min, vector<double> &Max );
	void NormalizeFront(vector<vector<double>> &Front);
	void getExtremePoints(vector<vector<double>> &Front ,vector<double> &Min, vector<double> &Max);
};


#endif // MEASURES_HPP_INCLUDED
