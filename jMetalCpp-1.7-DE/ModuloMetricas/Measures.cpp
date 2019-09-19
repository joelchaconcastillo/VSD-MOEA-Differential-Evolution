#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <cmath>
#include "Measures.hpp"
#define LASTGENERATION -1
using namespace std;

Measures::Measures(vector<vector<double> >&SetPareto, vector<vector<double>> &ParetoOptimalFront)
{
    this->SetPareto = SetPareto;
    this->ParetoOptimalFront= ParetoOptimalFront;
}
Measures::Measures(string FileSetPareto, string ParetoOptimalFront)
{
	this->SetPareto = ReadFile(FileSetPareto);
	this->ParetoOptimalFront = ReadFile(ParetoOptimalFront);
}
Measures::Measures(string ParetoOptimalFront)
{
	//Cargar el archivo que contiene el frente de pareto optimo...	
	this->ParetoOptimalFront = ReadFile(ParetoOptimalFront);
}
Measures::Measures()
{

}
/**
	SECCIÓN PARA OBTENER INFORMACIÓN DE UN CONJUNTO DE EJECUCIONES
**/
/**
Obtener los archivos dado el nombre base proporcionado.
**/
vector<string> Measures::getFiles(string basename)
{
	vector<string> files;
	string tmp = "ls "+basename+"*";
	FILE *fp;
	char file_type[400];
	const char *Term = tmp.c_str();
	fp = popen(Term, "r");
	if(!fp)
	{
		cout << "Failed to read files";
		exit(-1);
	}
	while(fgets(file_type, sizeof(file_type), fp) != NULL)
	{
		//Para quitar espacios en blanco del final.
		stringstream trimmer;
		trimmer << string(file_type);
		string tmp;
		trimmer >> tmp;
		files.push_back(tmp);
	}
	pclose(fp);
	return files;
}
/**
	Se revisan las métricas a procesar y su respectiva configuración
**/
void Measures::StartMetrics( map<int, string> Configuration, string PathFiles, string basename, string PathResult, string FP)
{
	//Se obtienen todos los archivos con el prefijo proporcionado..
	string FitnessFile = "Fitness_" + basename;
	string FenotypeFile = "Fenotype_" + basename;
	string SummaryFile = "SummaryFront_" + basename;
		
	vector<string> AllFilesFitness = getFiles(PathFiles + "/" + FitnessFile);
	vector<string> AllFilesFenotype = getFiles(PathFiles + "/" + FenotypeFile);
	vector<string> AllFilesSummary = getFiles(PathFiles + "/" + SummaryFile);
	//Almacenar el frente optimo en memoria
	this->ParetoOptimalFront = ReadFile(FP);
	
	
	//Existen dos modalidades para las métricas
	//1) Se implementa la métrica a la última generación. (SPACING)
	//2) Se promedian las N ejecuciones y se almacenen los 
	// valores de cada generación. (ALLSPACING)

	/**
	   Para cada configuración se genera un archivo con nombre de formato:
		Metrica + basename + .txt
	**/
	//Si ya existen los archivos se sobreescriben
	map  <int, ofstream> FileMetric;
	map<int, string>::iterator It = Configuration.begin();
	for(; It != Configuration.end(); It++)
	{
		string file = "tmp/" +IdtoNameMetric(It->first )+ "_" +basename+".txt" ; 
		FileMetric[It->first].open( file.c_str()  );
	}
	for(int i = 0; i < AllFilesFitness.size(); i++)
	{	
		//Almacenar la información del archivo en memoria
		this->SetPareto = ReadFile( AllFilesSummary[i]);
		map<int, string>::iterator it = Configuration.begin();
		for(; it != Configuration.end(); it++)
		{
		   switch(it->first)
		   {
			case SPACING:
				FileMetric[SPACING] << Spacing() << endl;
			break;
			case SPREAD:
				FileMetric[SPREAD] << Spread() << endl;
			break;
			case HYPERVOLUME:
				if(it->second.empty())
				   FileMetric[HYPERVOLUME]  << HyperVolume() << endl;
			//	cout << HyperVolume() <<endl;
				else
				{
					//Obtener el punto de referencia....
					stringstream ss(it->second);
					vector<double> RefPoint;
					double data;
					while(ss>>data){RefPoint.push_back(data);}
					FileMetric[HYPERVOLUME]  << HyperVolume(RefPoint) << endl;
				}
			break;
			case HYPERVOLUMERATIO:
				if(it->second.empty())
				   FileMetric[HYPERVOLUMERATIO]  << HyperVolumeRatio() << endl;
				else
				{
					//Obtener el punto de referencia....
					stringstream ss(it->second);
					vector<double> RefPoint;
					double data;
					while(ss>>data){RefPoint.push_back(data);}
				   	FileMetric[HYPERVOLUMERATIO]  << HyperVolumeRatio(RefPoint) << endl;
				}
			break;
			case GENERATIONALDISTANCE:
				FileMetric[GENERATIONALDISTANCE]  << GenerationalDistance() << endl;
			break;
			case INVERTEDGENERATIONALDISTANCE:
				FileMetric[INVERTEDGENERATIONALDISTANCE]  << InvertedGenerationalDistance() << endl;
			break;
			case CLOSESTDISTANCE:
	//			Results[CLOSESTDISTANCE].push_back(ClosestDistance());
			break;
			case ALLSPACING:
				 RuningMeasureSpacing( AllFilesFitness[i] , FileMetric[SPACING] );
			break;
			case ALLSPREAD:	
				 RuningMeasureSpread( AllFilesFitness[i]  , FileMetric[ALLSPREAD]);
			break;
			case ALLHYPERVOLUME:
				 RuningMeasureHyperVolume( AllFilesFitness[i] ,FileMetric[ALLHYPERVOLUME] );
			break;
			case ALLHYPERVOLUMERATIO:
				 RuningMeasureHyperVolumeRatio( AllFilesFitness[i]  , FileMetric[ALLHYPERVOLUMERATIO]);
			break;
			case ALLGENERATIONALDISTANCE:
				 RuningMeasureGenerationalDistance( AllFilesFitness[i]  ,FileMetric[ALLGENERATIONALDISTANCE] );
			break;
			case ALLINVERTEDGENERATIONALDISTANCE:
				 RuningMeasureInvertedGenerationalDistance( AllFilesFitness[i]  ,FileMetric[ALLINVERTEDGENERATIONALDISTANCE] );
			break;
			case ALLCLOSESTDISTANCE:
				 RuningClosestDistance( AllFilesFitness[i]  ,FileMetric[ALLCLOSESTDISTANCE]  );
			break;
		   }
		}
	}



	//Para el caso de las métricas de tipo "ALL" se deben promediar
	//los valores en cada generación, esto se resuelve por medio de un mapa donde 
	//el índice indica la generación y su respectivo valor...
	It = Configuration.begin();
	for(; It != Configuration.end(); It++)
	{
		
		if(It->first == SPACING || It->first == SPREAD || It->first == HYPERVOLUME || It->first == HYPERVOLUMERATIO || It->first == GENERATIONALDISTANCE ||It->first == CLOSESTDISTANCE || INVERTEDGENERATIONALDISTANCE) continue;

		map<int, double> MeanGeneration;
		ifstream FileMetric;

		string file = IdtoNameMetric(It->first )+ "_" +basename+".txt" ; 
		FileMetric.open(  string("tmp/"+file).c_str());
		int Generation;
		double MetricValue;
		while( FileMetric >> Generation >> MetricValue   )
			MeanGeneration[Generation] += (MetricValue/ AllFilesSummary.size()); 
		FileMetric.close();

		ofstream SaveMetric;
		SaveMetric.open( string(PathResult+"/"+file).c_str()  );
		
		map<int, double>::iterator i = MeanGeneration.begin();
		for(; i != MeanGeneration.end(); i++)
			SaveMetric << i->first << " " << i->second << endl;
		SaveMetric.close();
	}	

	//En las métricas donde se realiza el análisis en la última ejecución
	//sólo es necesario copiar la información del fichero temporal
	
	
	It = Configuration.begin();
	for(; It != Configuration.end(); It++)
	{
		if(It->first == SPACING || It->first == SPREAD || It->first == HYPERVOLUME || It->first == HYPERVOLUMERATIO || It->first == GENERATIONALDISTANCE ||It->first == CLOSESTDISTANCE || INVERTEDGENERATIONALDISTANCE)
		{
			string file = IdtoNameMetric(It->first )+ "_" +basename+".txt" ; 
			system( string("cp tmp/"+file +" "+PathResult + "/"+file).c_str());
		}
	}

	//Cerrar todos los apuntadores a los ficheros...
	It = Configuration.begin();
	for(; It != Configuration.end(); It++)
		FileMetric[It->first].close();

}
string Measures::IdtoNameMetric(int Id)
{
	switch(Id)
	{
		case SPACING:
		   return "SPACING";
		case SPREAD:
		   return "SPREAD";
		case HYPERVOLUME:
		   return "HYPERVOLUME";
		case HYPERVOLUMERATIO:
		   return "HYPERVOLUMERATIO";
		case GENERATIONALDISTANCE:
		   return "GENERATIONALDISTANCE";
		case INVERTEDGENERATIONALDISTANCE:
		   return "INVERTEDGENERATIONALDISTANCE";
		case CLOSESTDISTANCE:
		   return "CLOSESTDISTANCE";
		case ALLSPACING:
		   return "ALLSPACING";
		case ALLSPREAD:
		   return "ALLSPREAD";
		case ALLHYPERVOLUME:
		   return "ALLHYPERVOLUME";
		case ALLHYPERVOLUMERATIO:
		   return"ALLHYPERVOLUMERATIO";
		case ALLGENERATIONALDISTANCE:
		  return "ALLGENERATIONALDISTANCE";
		case ALLINVERTEDGENERATIONALDISTANCE:
		  return "ALLINVERTEDGENERATIONALDISTANCE";
		case ALLCLOSESTDISTANCE:
		  return "ALLCLOSESTDISTANCE";
	 	default:
		   return "ERROR";
	}
}
/**
	SECCIÓN PARA IMPLEMENTAR LAS MÉTRICAS EN EL HISTORIAL DEL ALGORITMO
**/
void Measures::RuningClosestDistance(string FilenameVariables, ofstream &File)
{
	int TotalGenerations, CurrentGeneration, Dimension;

	ifstream IsFile;
	IsFile.open(FilenameVariables);
	/**
		Cargar la información en relación a la ejecución...
	**/	
	string trash;
	IsFile >> trash >>  TotalGenerations >> trash >> Dimension;
	int SizePool;
	while(IsFile >> trash >> SizePool >> trash >> CurrentGeneration)
	for(int i = 0; i < TotalGenerations; i++)
	{
		this->Variables.resize(SizePool,vector<double>(Dimension));
		for(int j =0; j  < SizePool; j++)
		{
			for(int m = 0; m < Dimension; m++)
			{
				IsFile >> this->Variables[j][m];	
			}	
		}	
		File << CurrentGeneration << " " <<ClosestDistance(this->Variables) <<endl;
	}
}
void Measures::RuningMeasureGenerationalDistance(string FileSetPareto,  ofstream &File)
{
	int TotalGenerations, CurrentGeneration, NumberObjectives;

	ifstream IsFile;
	IsFile.open(FileSetPareto);
	/**
		Cargar la información en relación a la ejecución...
	**/	
	string trash;
	IsFile >> trash >>  TotalGenerations >> trash >> NumberObjectives;
	for(int i = 0; i < TotalGenerations; i++)
	{
		int SizePool;
		IsFile >> trash >> SizePool >> trash >> CurrentGeneration ; 
		this->SetPareto.resize(SizePool,vector<double>(NumberObjectives));
		for(int j =0; j  < SizePool; j++)
		{
			for(int m = 0; m < NumberObjectives; m++)
			{
				IsFile >> this->SetPareto[j][m];	
			}	
		}	
		File << CurrentGeneration << " " << GenerationalDistance() << endl; 
	}
}
void Measures::RuningMeasureInvertedGenerationalDistance(string FileSetPareto,  ofstream &File)
{
	int TotalGenerations, CurrentGeneration, NumberObjectives;

	ifstream IsFile;
	IsFile.open(FileSetPareto);
	/**
		Cargar la información en relación a la ejecución...
	**/	
	string trash;
	IsFile >> trash >>  TotalGenerations >> trash >> NumberObjectives;
	for(int i = 0; i < TotalGenerations; i++)
	{
		int SizePool;
		IsFile >> trash >> SizePool >> trash >> CurrentGeneration ; 
		this->SetPareto.resize(SizePool,vector<double>(NumberObjectives));
		for(int j =0; j  < SizePool; j++)
		{
			for(int m = 0; m < NumberObjectives; m++)
			{
				IsFile >> this->SetPareto[j][m];	
			}	
		}	
		File << CurrentGeneration << " " << InvertedGenerationalDistance() << endl; 
	}
}
void Measures::RuningMeasureSpacing(string FileSetPareto,  ofstream &File)
{
	int TotalGenerations, CurrentGeneration, NumberObjectives;

	ifstream IsFile;
	IsFile.open(FileSetPareto);
	/**
		Cargar la información en relación a la ejecución...
	**/	
	string trash;
	IsFile >> trash >>  TotalGenerations >> trash >> NumberObjectives;
	
	for(int i = 0; i < TotalGenerations; i++)
	{
		vector<double> tmp;
		int SizePool;
		IsFile >> trash >> SizePool >> trash >> CurrentGeneration; 
		this->SetPareto.resize(SizePool,vector<double>(NumberObjectives));
		for(int j =0; j  < SizePool; j++)
		{
			for(int m = 0; m < NumberObjectives; m++)
			{
				IsFile >> this->SetPareto[j][m];	
			}	
		}	
		File << CurrentGeneration << " " << Spacing()<<endl;
	}
}
void Measures::RuningMeasureSpread(string FileSetPareto,  ofstream &File)
{
	int TotalGenerations, CurrentGeneration, NumberObjectives;

	ifstream IsFile;
	IsFile.open(FileSetPareto);
	/**
		Cargar la información en relación a la ejecución...
	**/	
	string trash;
	IsFile >> trash >>  TotalGenerations >> trash >> NumberObjectives;
	
	for(int i = 0; i < TotalGenerations; i++)
	{
		vector<double> tmp;
		int SizePool;
		IsFile >> trash >> SizePool >> trash >>CurrentGeneration ; 
		this->SetPareto.resize(SizePool,vector<double>(NumberObjectives));
		for(int j =0; j  < SizePool; j++)
		{
			for(int m = 0; m < NumberObjectives; m++)
			{
				IsFile >> this->SetPareto[j][m];	
			}	
		}	
		File << CurrentGeneration << " " << Spread();
	}
}
void Measures::RuningMeasureHyperVolume(string FileSetPareto,  ofstream &File)
{
	int TotalGenerations, CurrentGeneration, NumberObjectives;

	ifstream IsFile;
	IsFile.open(FileSetPareto);
	/**
		Cargar la información en relación a la ejecución...
	**/	
	string trash;
	IsFile >> trash >>  TotalGenerations >> trash >> NumberObjectives;
	
	for(int i = 0; i < TotalGenerations; i++)
	{
		vector<double> tmp;
		int SizePool;
		IsFile >> trash >> SizePool >> trash  >>CurrentGeneration; 
		this->SetPareto.resize(SizePool,vector<double>(NumberObjectives));
		for(int j =0; j  < SizePool; j++)
		{
			for(int m = 0; m < NumberObjectives; m++)
			{
				IsFile >> this->SetPareto[j][m];	
			}	
		}	
		File << CurrentGeneration << " " << HyperVolume() << endl;
	}
}
void Measures::RuningMeasureHyperVolumeRatio(string FileSetPareto,  ofstream &File)
{
	int TotalGenerations, CurrentGeneration, NumberObjectives;

	ifstream IsFile;
	IsFile.open(FileSetPareto);
	/**
		Cargar la información en relación a la ejecución...
	**/	
	string trash;
	IsFile >> trash >>  TotalGenerations >> trash >> NumberObjectives;
	
	for(int i = 0; i < TotalGenerations; i++)
	{
		vector<double> tmp;
		int SizePool;
		IsFile >> trash >> SizePool >> trash >>CurrentGeneration; 
		this->SetPareto.resize(SizePool,vector<double>(NumberObjectives));
		for(int j =0; j  < SizePool; j++)
		{
			for(int m = 0; m < NumberObjectives; m++)
			{
				IsFile >> this->SetPareto[j][m];	
			}	
		}	
		File << CurrentGeneration << " " << HyperVolumeRatio() << endl;
	}
}
/**
	FUNCIONES DE LAS MÉTRICAS 
**/
void Measures::PrintObjectiveSpace(string FileSetPareto, string FilenameSummary)
{
	int TotalGenerations, CurrentGeneration, NumberObjectives;
	ofstream  OsFile;
	OsFile.open(FilenameSummary);	

	ifstream IsFile;
	IsFile.open(FileSetPareto);
	/**
		Cargar la información en relación a la ejecución...
	**/	
	string trash;
	IsFile >> trash >>  TotalGenerations >> trash >> NumberObjectives;
	
	for(int i = 0; i < TotalGenerations; i++)
	{
		int SizePool;
		IsFile >> trash >> SizePool >> trash >> trash ; 
		this->SetPareto.resize(SizePool,vector<double>(NumberObjectives));
		for(int j =0; j  < SizePool; j++)
		{
			for(int m = 0; m < NumberObjectives; m++)
			{
				IsFile >> this->SetPareto[j][m];	
				if(i == TotalGenerations-2)
				{
					OsFile << this->SetPareto[j][m] << "\t";
				}
			}	
			if(i == TotalGenerations-2)
				OsFile << endl;
		}	
	}
}
vector<vector<double>> Measures::ReadFile(string FileName)
{
	ifstream OsFile;
	OsFile.open(FileName);
	
	int SizePool, NumberObjectives;

	OsFile >> SizePool >> NumberObjectives;
	vector<vector<double>> Set(SizePool);
	vector<double> CurrentGeneration(NumberObjectives);
	for(int i = 0; i < SizePool; i++)
	{
		for(int j = 0; j < NumberObjectives; j++)
		{
			OsFile >> CurrentGeneration[j]; 
		}
		Set[i] = CurrentGeneration;
	}
	return Set;
}

/***
	Se localizan las menores disancias del frente1 hacia el frente2
**/
vector<double> Measures::MinimumDistance(vector<vector<double>> &Front1, vector<vector<double>> &Front2)
{
    vector<double> MinDistance(Front1.size(), INFINITY);
    /**
        Encontrar los elementos que se encuentran a menor distancia.
    **/
    for(int i = 0; i < Front1.size(); i++)
    {

        for(int j = 0; j < Front2.size(); j++)
        {
       	        double DistanceWithPareto = EuclideanDistance(Front1[i], Front2[j] );
		MinDistance[i] = min( DistanceWithPareto, MinDistance[i]);
        }
    }
    return MinDistance;
}
double Measures::GenerationalDistance()
{

    /**
	Se normaliza el frente óptimo para obtener
	valores en el domino [0,1]...
    */
   vector<vector<double>> FrontNormalized = SetPareto ;
   vector<vector<double>> OptimalFrontNormalized = ParetoOptimalFront;
   vector<double> Min, Max;
   getExtremePoints(OptimalFrontNormalized, Min, Max);
   NormalizeFront( FrontNormalized, Min, Max);
   NormalizeFront(OptimalFrontNormalized, Min, Max);

    int p = 2;

    vector<double> MinDistance = MinimumDistance(FrontNormalized, OptimalFrontNormalized);
    double Suma=0;
    for(int i = 0; i < MinDistance.size(); i++)
    {
        Suma += pow(MinDistance[i],p);
    }
    return pow(Suma, 1.0/p)/this->SetPareto.size();
}
double Measures::InvertedGenerationalDistance()
{

    /**
	Se normaliza el frente óptimo para obtener
	valores en el domino [0,1]...
    */
   vector<vector<double>> FrontNormalized = SetPareto ;
   vector<vector<double>> ParetoFrontNormalized = ParetoOptimalFront;
   vector<double> Min, Max;
   getExtremePoints(ParetoFrontNormalized, Min, Max);
   NormalizeFront( FrontNormalized, Min, Max);
   NormalizeFront(ParetoFrontNormalized, Min, Max);

    int p = 2;

    vector<double> MinDistance = MinimumDistance(ParetoFrontNormalized, FrontNormalized);
    double Suma=0;
    for(int i = 0; i < MinDistance.size(); i++)
    {
        Suma += pow(MinDistance[i],p);
    }
    return pow(Suma, 1.0/p)/ParetoOptimalFront.size();
}
void Measures::getExtremePoints( vector<vector<double>> &Front ,vector<double> &Min, vector<double> &Max)
{
	double Objectives = Front[0].size();
	//Objtener los valores extremos de cada frente optimo
	 Min.resize(Objectives, INFINITY);
	 Max.resize(Objectives, -INFINITY);
	for(int i = 0; i < Front.size(); i++)
	{
		for(int m = 0; m< Objectives; m++)
		{
			Min[m] = min( Min[m], Front[i][m]);
			Max[m] = max( Max[m], Front[i][m]);
		}
	}
}
void Measures::NormalizeFront(vector<vector<double>> &Front, vector<double> &Min, vector<double> &Max )
{
	if(Front.empty())
	{
		cout<<"No pareto front" <<endl;
		exit(0);
	}
	double Objectives = Front[0].size();
	for(int i = 0; i < Front.size(); i++)
	{
		for(int m = 0; m < Objectives; m++)
		{
			Front[i][m] = (Front[i][m]- Min[m])/(Max[m] - Min[m]);
		}
	}
}
double Measures::EuclideanDistance(vector<double> SolutionA, vector<double> SolutionB )
{
    double Suma=0;
    for(int i = 0;i < SolutionA.size(); i++)
    {
        Suma += pow( SolutionA[i] - SolutionB[i] ,2);
    }
    return sqrt(Suma);
}

double Measures::DifferenceAbsolute(vector<double> SolutionA, vector<double> SolutionB )
{
    double Suma=0;
    for(int i = 0; i < SolutionA.size(); i++)
    {
        Suma += fabs( SolutionA[i] - SolutionB[i]);
    }
    return Suma;
}
vector<double> Measures::DistanceSolutions()
{
 /*   vector<double> Distances;
    for(int i =0 ; i < this->SetPareto-1; i++)
    {
        Distance.push_back( this->EuclideanDistance(this->SetPareto[i], this->SetPareto[i+1]) );
    }*/
}
vector<double> Measures::OwnMinimalDifference(vector<vector<double>> &Front)
{
    vector<double> MinDifference(Front.size(), INFINITY);

    for(int i = 0; i < Front.size(); i++)
    {
        for(int j = 0; j < Front.size(); j++)
        {
            if(i == j) continue;
            double DistanceBetweenPareto = EuclideanDistance(Front[i], Front[j] );
	    MinDifference[i] = min( DistanceBetweenPareto, MinDifference[i]);
        }
    }
    return MinDifference;
}
double Measures::Spacing()
{
//    vector<double> MinAbsoluteDifference = MinimalAbsoluteDifference(  );
//    double MeanDifferences = Mean(MinAbsoluteDifference);
//
//    double Suma = 0;
//    for(int i = 0; i < MinAbsoluteDifference.size(); i++)
//    Suma += pow( MinAbsoluteDifference[i]- MeanDifferences ,2);
//    return sqrt(Suma / MinAbsoluteDifference.size());
	return 0;
}
double Measures::Spread()
{
    vector<vector<double>> FrontNormalized = SetPareto ;
    vector<vector<double>> OptimalFrontNormalized = ParetoOptimalFront;
    int NObjectives = OptimalFrontNormalized[0].size();
    vector<double> MinOptimal, MaxOptimal;
    getExtremePoints(OptimalFrontNormalized, MinOptimal, MaxOptimal);
    
    NormalizeFront( FrontNormalized, MinOptimal, MaxOptimal);
    NormalizeFront(OptimalFrontNormalized, MinOptimal, MaxOptimal);

   vector<vector<double>> Extreme( NObjectives, vector<double> ( NObjectives,-INFINITY));
     double Termino1=0;
    for(int m = 0; m < NObjectives; m++)
	{
		for(int i = 0; i <  OptimalFrontNormalized.size(); i++)
		{
			if( Extreme[m][m] < OptimalFrontNormalized[i][m])
			{
				Extreme[m] = OptimalFrontNormalized[i];
			}	
		}
	} 
    vector<double> MinDistance = MinimumDistance(Extreme, FrontNormalized);
	for(int i = 0 ; i < MinDistance.size(); i++)
		Termino1 += MinDistance[i];
 
    vector<double> MinAbsoluteDifference = OwnMinimalDifference(FrontNormalized);
//    MinAbsoluteDifference.resize(MinAbsoluteDifference.size());
    double MeanDifferences = Mean(MinAbsoluteDifference);

    double Termino2 = 0;

    for(int i = 0; i < MinAbsoluteDifference.size(); i++)
        Termino2 += fabs( MinAbsoluteDifference[i]- MeanDifferences);

    /**
            Obtener los indices del frente de forma ordenada
    **/
    double Termino3 = MinAbsoluteDifference.size()*MeanDifferences ;
    double Numerador = Termino1 + Termino2;
    double Denominador = Termino1 + Termino3;
    return Numerador / Denominador;
}
double Measures::Mean(vector<double> &data)
{
    double Suma = 0 ;
    for(int i=0; i < data.size(); i++)
    Suma+=data[i];
    return Suma / data.size();
}
double Measures::Sum(vector<double> &data)
{
    double Suma = 0;
    for(int i = 0; i < data.size(); i++)
        Suma+= data[i];
    return Suma;
}
double Measures::HyperVolume(vector<double> &W)
{

    /**
        Ordenar el frente calculado en base a una función objetivo

    **/
    sort( this->SetPareto.begin(), this->SetPareto.end(), ObjetiveSort());

    //Extremo superior..
    double Volumen =  (W[0]-SetPareto[0][0])*(W[1]-SetPareto[0][1]);
    for(int i = 1; i < this->SetPareto.size(); i++)
    {
        Volumen +=  (W[0]-SetPareto[i][0])*(SetPareto[i-1][1]-SetPareto[i][1]);
    }
    return Volumen;

}
/**
	Se calcula la métrica del hipervolúmen sin un punto de referencia
	por medio del punto máximo.
**/
double Measures::HyperVolume()
{
	int Objectives = this->SetPareto[0].size();
	vector<double> MaxSet( Objectives, -INFINITY);

	//Obtener el vector extremo..
	for(int i = 0; i < this->SetPareto.size(); i++)
	{
		for(int m = 0 ; m < Objectives; m++)
			MaxSet[m] = max(MaxSet[m], SetPareto[i][m] );
	}
	return HyperVolume(MaxSet);
}
/**
	Se calcula la métrica del hipervolúmen normalizado sin el vector de referencia
	por medio del punto máximo.
**/
double Measures::HyperVolumeRatio()
{
	int Objectives = this->SetPareto[0].size();
	vector<double> MaxSet( Objectives, -INFINITY);

	//Obtener el vector extremo..
	for(int i = 0; i < this->SetPareto.size(); i++)
	{
		for(int m = 0 ; m < Objectives; m++)
			MaxSet[m] = max(MaxSet[m], SetPareto[i][m] );
	}
	
	return HyperVolumeRatio( MaxSet);
}
double Measures::HyperVolumeRatio(vector<double> &W)
{

    /**
        Ordenar el frente calculado en base a una función objetivo

    **/

    sort( this->SetPareto.begin(), this->SetPareto.end(), ObjetiveSort());

    ////Extremo superior..
    double HVPareto =  (W[0]-SetPareto[0][0])*(W[1]-SetPareto[0][1]);
    for(int i = 1; i < this->SetPareto.size(); i++)
    {
        HVPareto +=  (W[0]-SetPareto[i][0])*(SetPareto[i-1][1]-SetPareto[i][1]);
    }

    sort( this->ParetoOptimalFront.begin(), this->ParetoOptimalFront.end(), ObjetiveSort());
    double HVOptimal =  (W[0]-ParetoOptimalFront[0][0])*(W[1]-ParetoOptimalFront[0][1]);
    for(int i = 1; i < this->ParetoOptimalFront.size(); i++)
    {
        HVOptimal +=  (W[0]-ParetoOptimalFront[i][0])*(ParetoOptimalFront[i-1][1]-ParetoOptimalFront[i][1]);
    }
    return HVPareto/HVOptimal;

}
double Measures::HyperDistance(vector<double> &W)
{
    double SumDistantcesV = 0;
    for(int i = 0; i < this->SetPareto.size(); i++)
    {
        SumDistantcesV += EuclideanDistance(W, SetPareto[i]);
    }

    double SumDistantcesOptimal = 0;
    for(int i = 0; i < this->ParetoOptimalFront.size(); i++)
    {
        SumDistantcesOptimal += EuclideanDistance(W, ParetoOptimalFront[i]);
    }
    return (SumDistantcesV / this->ParetoOptimalFront.size())/(SumDistantcesOptimal/this->SetPareto.size());
}
double Measures::ClosestDistance(vector<vector<double>> &X)
{
	double Distance=0;
	vector<double> Weights(X.size(),INFINITY);
	for(int i = 0; i < X.size(); i++)
	{
		for(int j= 0; j< X.size(); j++)
		{
			if(i==j) continue;
			double D = EuclideanDistance(X[i], X[j]);
			Weights[i] = min(D, Weights[i]);
		}	
	}
	for(int i = 0; i < X.size(); i++)
		Distance+=Weights[i];	

	return Distance/X.size();
}
