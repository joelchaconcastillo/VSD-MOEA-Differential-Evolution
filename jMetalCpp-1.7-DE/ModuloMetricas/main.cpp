/**
	Autor: Joel Chacón Castillo
       
	Lista de argumentos:
		--i Ruta de origen (Donde se localizan los archivos a procesar, los prefijos deben ser: "Fenotype, Fitness, Summary (El summary debe llevar en la primer línea el tamaño de la población y el número de objetivos)". 
		--l Etiqueta que caracterizan a los archivos.
		--o Ruta donde se almacenaran los ficheros.
		--pf Frente de pareto óptimo. La primer línea debe llevar el tamaño de la población y el número de objetivos.
	
       El último argumento indica las métricas que se van a utilizar
       En el caso del hipervolumen el punto de referencia se indica separando las coordenadas por el caracter coma y todo junto.
	Ejemplo de ejecucion:
	./Ejecutable --i ../tmp/WFG1/ --l SBX --o ../Summary/ --pf OptimalFronts/WFG1.txt --HYPERVOLUMERATIO 3,5
**/
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include "Measures.hpp"
using namespace std;
string getVector(int Size, char *argv[], int Index)
{
	if(Index>=Size) return "";
	
	string RefPoint (argv[Index]);
	if(RefPoint[0] == '-' && RefPoint[1] == '-')
	return "";
	
	replace(RefPoint.begin(), RefPoint.end(), ',', ' ');	
	return RefPoint;
}
int main(int argc, char* argv[])
{
    if(argc<2)
    {
	cout << "Unknown Argument.."<<endl;
	exit(0);
    }
    string PathFiles, BaseName, PathResult, FP;
    map<int, string> Configuration;
    /**
	Se establece la configuracion..	
    **/
    for(int i = 1; i < argc ; i++)
    {
		string Terminal(argv[i]);
		if( Terminal == "--i")
			PathFiles = string(argv[++i]);
		else if(Terminal == "--l")
			BaseName = string(argv[++i]);
		else if(Terminal == "--o")
			PathResult = string(argv[++i]);
		else if(Terminal == "--pf")
			FP = string(argv[++i]);
		else if(Terminal == "--SPACING") 
			Configuration[SPACING] = "";
		else if(Terminal == "--SPREAD")
			Configuration[SPREAD] = "";
		else if(Terminal == "--HYPERVOLUME")
			Configuration[HYPERVOLUME] = getVector(argc, argv, ++i);
		else if(Terminal == "--HYPERVOLUMERATIO")
			Configuration[HYPERVOLUMERATIO] =getVector(argc, argv, ++i) ;
		else if(Terminal == "--GD")
			Configuration[GENERATIONALDISTANCE] = "";
		else if(Terminal == "--IGD")
			Configuration[INVERTEDGENERATIONALDISTANCE] = "";
		else if(Terminal == "--CLOSESTDISTANCE")
			Configuration[CLOSESTDISTANCE] = "";
		else if(Terminal == "--ALLSPACING")
			Configuration[ALLSPACING] = "";
		else if(Terminal == "--ALLSPREAD")
			Configuration[ALLSPREAD] = "";
		else if(Terminal == "--ALLHYPERVOLUME")
			Configuration[ALLHYPERVOLUME] = getVector(argc, argv, ++i);
		else if(Terminal == "--ALLHYPERVOLUMERATIO")
			Configuration[ALLHYPERVOLUMERATIO]=getVector(argc, argv, ++i);
		else if(Terminal == "--ALLGD")
			Configuration[ALLGENERATIONALDISTANCE]="";
		else if(Terminal == "--ALLIGD")
			Configuration[ALLINVERTEDGENERATIONALDISTANCE]="";
		else if(Terminal == "--ALLCLOSESTDISTANCE")
			Configuration[ALLCLOSESTDISTANCE] = "";
		else
		{
			cout << Terminal<<endl;
			cout << "Unknown Argument...";
			exit(0);
		}
    }
    Measures ObjMeasures;
    ObjMeasures.StartMetrics(Configuration, PathFiles, BaseName, PathResult, FP);


    return 0;
}
