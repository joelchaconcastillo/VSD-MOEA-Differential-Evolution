/*==========================================================================
//  C++ Implementation of MOEA/D Based on Differential Evolution (DE) for Contest Multiobjective
//  Problems in CEC2009
//
//  Author: Hui Li
//
//  See the details of MOEA/D-DE and test problems in the following papers
//
//  1) H. Li and Q. Zhang, Comparison Between NSGA-II and MOEA/D on a Set of Multiobjective Optimization
//  Problems with Complicated Pareto Sets, Technical Report CES-476, Department of Computer Science,
//  University of Essex, 2007
//
//  2) H. Li and Q. Zhang, Multiobjective Optimization Problems with Complicated Pareto Sets, MOEA/D and NSGA-II,
//  IEEE Transaction on Evolutionary Computation, 2008, to appear.
//
//  If you have any questions about the codes, please contact
//  Dr. Hui Li       at   hzl@cs.nott.ac.uk   or
//  Dr. Qingfu Zhang at   qzhang@essex.ac.uk
//
//  Date: 14/NOV/2008
//
// ===========================================================================*/
#include "time.h"
#include "global.h"
#include "algorithm.h"
void InitializeBounds(int nvar, char * Instance)
{
        if( !strcmp("UF1", Instance) || !strcmp("UF2", Instance) || !strcmp("UF3", Instance) || !strcmp("UF4", Instance) || !strcmp("UF5", Instance) || !strcmp("UF6", Instance) || !strcmp("UF7", Instance) || !strcmp("UF8", Instance) || !strcmp("UF9", Instance) || !strcmp("UF10", Instance))
        {
                for(int i = 0 ;  i < nvar; i++)
                {
                   vlowBound[i]=0.0;
                   vuppBound[i]=1.0;//2.0*(i+1.0);
                }
        }

        if( !strcmp("WFG1", Instance) || !strcmp("WFG2", Instance) || !strcmp("WFG3", Instance) || !strcmp("WFG4", Instance) || !strcmp("WFG5", Instance) || !strcmp("WFG6", Instance) || !strcmp("WFG7", Instance) || !strcmp("WFG8", Instance) || !strcmp("WFG9", Instance))
        {
                for(int i = 0 ;  i < nvar; i++)
                {
                   vlowBound[i]=0.0;
                   vuppBound[i]=2.0*(i+1.0);
                }
        }
        if( !strcmp("DTLZ1", Instance) || !strcmp("DTLZ2", Instance) || !strcmp("DTLZ3", Instance) || !strcmp("DTLZ4", Instance) || !strcmp("DTLZ5", Instance) || !strcmp("DTLZ6", Instance) || !strcmp("DTLZ7", Instance) )
        {
                for(int i = 0 ;  i < nvar; i++)
                {
                   vlowBound[i]=0.0;
                   vuppBound[i]=1.0;
                }
        }
}

int main(int argc,char *argv[])
{


//moead_cpp WFG9 9 3 100 25000000 20 0.9 2 20 4
       int index = 1;
       int run = 1;
       strcpy(strpath, argv[index++]);
       strcpy(strTestInstance, argv[index++]);
       run= atoi(argv[index++]);
       nobj = atoi(argv[index++]);
       pops = atoi(argv[index++]);
       max_nfes= atoi(argv[index++]);
       niche =  atoi(argv[index++]);
       prob = atof(argv[index++]);
       limit = atof(argv[index++]);

       if(argc < index+2)//Two nvar and Di
       {
          nvar = atoi(argv[index++]);

       }
       else  //WFG instances..
       {
          param_l = atoi(argv[index++]);
          param_k = atoi(argv[index++]);
          nvar = param_l + param_k;
       }
       InitializeBounds(nvar, strTestInstance);


	CMOEAD MOEAD;
//	MOEAD.load_parameter();
	MOEAD.exec_emo(run);


	return 0;
}

