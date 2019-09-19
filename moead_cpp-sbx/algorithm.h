#ifndef __EVOLUTION_H_
#define __EVOLUTION_H_

#include <iomanip>
#include "global.h"
#include "recomb.h"
#include "common.h"
#include "individual.h"

class CMOEAD
{
public:
	CMOEAD();
	virtual ~CMOEAD();

	void init_neighbourhood();               // calculate the neighbourhood of each subproblem
	void init_population();                  // initialize the population

	void load_parameter();

	void update_reference(CIndividual &ind);                 // update ideal point which is used in Tchebycheff or NBI method
	void update_problem(CIndividual &ind, int &id, int &type); // update current solutions in the neighbourhood

	void evol_population();                                      // DE-based recombination
	void mate_selection(vector<int> &list, int cid, int size, int type);  // select mating parents

	// execute MOEAD
	void exec_emo(int run);

	void save_front(char savefilename[4024]);       // save the pareto front into files
	void save_pos(char savefilename[4024]);

	void tour_selection(int depth, vector<int> &selected);
	void comp_utility();

    vector <CSubproblem> population;
	vector <double>      utility;

	void operator=(const CMOEAD &moea);

public:

	// algorithm parameters
  //  int		max_gen;       //  the maximal number of generations
//	int     pops;          //  the population size
//    int	    niche;         //  the neighborhood size
//	int     limit;         //  the maximal number of solutions replaced
//	double  prob;          //  the neighboring selection probability
//	double  rate;          //  the differential rate between solutions

//	int     nfes;          //  the number of function evluations

};

CMOEAD::CMOEAD()
{

}

CMOEAD::~CMOEAD()
{

}

void CMOEAD::init_population()
{
	idealpoint = vector<double>(nobj, 1.0e+30);
	utility    = vector<double>(pops, 1.0);

	char filename[1024];
	// Read weight vectors from a data file
	sprintf(filename,"%s/ParameterSetting/Weight/W%dD_%d.dat", strpath, nobj, pops);
	std::ifstream readf(filename);

    for(int i=0; i<pops; i++)
	{

		CSubproblem sub;

		// Randomize and evaluate solution
		sub.indiv.rnd_init();
		sub.indiv.obj_eval();

		sub.saved = sub.indiv;

		// Initialize the reference point
		update_reference(sub.indiv);

		// Load weight vectors
		for(int j=0; j<nobj; j++)
		{
		    readf>>sub.namda[j];
			//printf("%f ", sub.namda[j]);
		}
		//printf("\n"); getchar();

		// Save in the population
		population.push_back(sub);
		nfes++;
	}

	readf.close( );
}

void CMOEAD::operator=(const CMOEAD &alg)
{
	//population = alg.population;
}

void CMOEAD::init_neighbourhood()
{
    vector<double> dist   = vector<double>(pops, 0);
	vector<int>    indx   = vector<int>(pops, 0);

	for(int i=0; i<pops; i++)
	{
		// calculate the distances based on weight vectors
		for(int j=0; j<pops; j++)
		{
		    dist[j]    = dist_vector(population[i].namda,population[j].namda);
			indx[j]  = j;
		}

		// find 'niche' nearest neighboring subproblems
		minfastsort(dist,indx,population.size(),niche);

		// save the indexes of the nearest 'niche' neighboring weight vectors
		for(int k=0; k<niche; k++)
		{
			population[i].table.push_back(indx[k]);
		}

	}
    dist.clear();
	indx.clear();
}

void CMOEAD::tour_selection(int depth, vector<int> &selected)
{
	// selection based on utility
	vector<int> candidate;
	for(int k=0;    k<nobj; k++)    selected.push_back(k);   // select first m weights
	for(int n=nobj; n<pops; n++)    candidate.push_back(n);  // set of unselected weights

	while(selected.size()<int(pops/5.0))
	{
	    int best_idd = int(rnd_uni(&rnd_uni_init)*candidate.size()), i2;
		int best_sub = candidate[best_idd], s2;
		for(int i=1; i<depth; i++)
		{
		    i2  = int(rnd_uni(&rnd_uni_init)*candidate.size());
			s2  = candidate[i2];
			if(utility[s2]>utility[best_sub])
			{
				best_idd = i2;
			    best_sub = s2;
			}
		}
		selected.push_back(best_sub);
		candidate.erase(candidate.begin()+best_idd);
	}
}

void CMOEAD::comp_utility()
{
	double f1, f2, uti, delta;
    for(int n=0; n<pops; n++)
	{
		f1 = fitnessfunction(population[n].indiv.y_obj, population[n].namda);
		f2 = fitnessfunction(population[n].saved.y_obj, population[n].namda);
		delta = (f2 - f1)/f2;
		//delta = (f2 - f1);
        if(delta>0.001)
		{
			utility[n] = 1.0;
		}
		else
		{
            //uti        = 0.95*(1.0+delta/0.001)*utility[n];
			//utility[n] = uti<1.0?uti:1.0;
			utility[n] = (0.95 + 0.05*delta/0.001)*utility[n];
		}
        population[n].saved = population[n].indiv;
	}
}

void CMOEAD::update_problem(CIndividual &indiv, int &id, int &type)
{
	// indiv: child solution
	// id:   the id of current subproblem
	// type: update solutions in - neighborhood (1) or whole population (otherwise)
	int size, time = 0;
	if(type==1)	size = population[id].table.size();  // from neighborhood
	else        size = population.size();            // from whole population

	// a random order to update
	std::vector<int> perm(std::vector<int>(size, 0));
	for(int k=0; k<size; k++) perm[k] = k;

	//random_shuffle(perm.begin(), perm.end());
	// replaced by Aimin 2011.04.29
	permutation(perm);


    for(int i=0; i<size; i++)
	{
		// Pick a subproblem to update
		int k;
		if(type==1) k = population[id].table[perm[i]];
		else        k = perm[i];

		// calculate the values of objective function regarding the current subproblem
		double f1, f2;
		f1 = fitnessfunction(population[k].indiv.y_obj, population[k].namda);
		f2 = fitnessfunction(indiv.y_obj, population[k].namda);
		if(f2<f1)
		{
			population[k].indiv = indiv;
			time++;
		}
		// the maximal number of solutions updated is not allowed to exceed 'limit'
		if(time>=limit)
		{
			return;
		}
	}
	perm.clear();
}

void CMOEAD::update_reference(CIndividual &ind)
{
	//ind: child solution
	for(int n=0; n<nobj; n++)
	{
		if(ind.y_obj[n]<idealpoint[n])
		{
			idealpoint[n] = ind.y_obj[n];
		}
	}
}

void CMOEAD::mate_selection(vector<int> &list, int cid, int size, int type){
	// list : the set of the indexes of selected mating parents
	// cid  : the id of current subproblem
	// size : the number of selected mating parents
	// type : 1 - neighborhood; otherwise - whole population
	int ss   = population[cid].table.size(), id, parent;
    while(list.size()<size)
	{
		if(type==1){
		    id      = int(ss*rnd_uni(&rnd_uni_init));
			parent  = population[cid].table[id];
		}
		else
			parent  = int(population.size()*rnd_uni(&rnd_uni_init));

		// avoid the repeated selection
		bool flag = true;
		for(int i=0; i<list.size(); i++)
		{
			if(list[i]==parent) // parent is in the list
			{
				flag = false;
				break;
			}
		}

		if(flag) list.push_back(parent);
	}
}

void CMOEAD::evol_population()
{

	// random order of subproblems at each generation
	//vector<int> order(vector<int>(pops,0));
	//for(int i=0; i<pops; i++)  order[i] = i;
	//random_shuffle(order.begin(), order.end());

	vector<int> order;	this->tour_selection(10, order);

    for(int sub=0; sub<order.size(); sub++)
	{

		int c_sub = order[sub];    // random order

		int type;
        double rnd = rnd_uni(&rnd_uni_init);

		// mating selection based on probability
		if(rnd<prob)     type = 1;   // from neighborhood
		else             type = 2;   // from population

		// select the indexes of mating parents
		vector<int> plist;
		mate_selection(plist, c_sub, 2, type);  // neighborhood selection


		// produce a child solution
		CIndividual child;
		double rate2 = box_muller(0.5,0.1); //rate + 0.25*(rnd_uni(&rnd_uni_init) - 0.5);

	//	diff_evo_xoverB(population[c_sub].indiv,population[plist[0]].indiv,population[plist[1]].indiv, child, rate2);
		real_sbx_xoverB(population[plist[0]].indiv,population[plist[1]].indiv,child);

		plist.clear();

		// apply polynomial mutation
		realmutation(child, 1.0/nvar);

		// evaluate the child solution
		child.obj_eval();

		// update the reference points and other solutions in the neighborhood or the whole population
		update_reference(child);
		update_problem(child, c_sub, type);

		nfes++;
	}
}


void CMOEAD::exec_emo(int run)
{
    char filename1[5024];
    char filename2[5024];
    seed = run;
    seed = (seed + 23)%1377;
    rnd_uni_init = -(long)seed;

	// initialization
    nfes      = 0;
    sprintf(filename1,"%s/POS/POS_MOEAD_%s_RUN%d_seed_%d_nobj_%d_niche_%d.dat_bounded",strpath, strTestInstance,run, seed, nobj, niche);
    sprintf(filename2,"%s/POF/POF_MOEAD_%s_RUN%d_seed_%d_nobj_%d_niche_%d.dat_bounded",strpath, strTestInstance,run, seed, nobj, niche);

    init_population();
    init_neighbourhood();
        save_pos(filename1);
        save_front(filename2);

	int gen = 0;
	//save_front(filename);

//	max_gen = int(5.0*300000/pops);
    int current = nfes;
    int accumulator = 0, bef = nfes;
	while(nfes< max_nfes)
	{
		evol_population();
		accumulator += nfes - bef ;
                if(accumulator >= 0.5*(max_nfes)  )
                {
                   accumulator -= 0.5*(max_nfes);
                   save_pos(filename1);
                   save_front(filename2);
                }
                bef=nfes;

		gen++;

		if(gen%50==0)
		{
		    comp_utility();
		}
	}

//	sprintf(filename,"POFS/PS_%s_%d.dat",strTestInstance,run);
//	save_pos(filename);
//	sprintf(filename,"POF/PF_%s_%d.dat",strTestInstance, run);
//	save_front(filename);
        save_pos(filename1);
        save_front(filename2);

	population.clear();
	idealpoint.clear();
}

void CMOEAD::load_parameter()
{
	char filename[1024];

	sprintf(filename,"%s/ParameterSetting/%s.txt", strpath, strTestInstance);

	char temp[1024];
	std::ifstream readf(filename);
	readf.getline(temp, 1024);

//	readf>>pops;
//	readf>>max_gen;
//	readf>>niche;
//	readf>>limit;
//	readf>>prob;
//        pops=100;
        //max_gen = 250000;
//	niche = 10;
//	readf>>rate;

	readf.close();
}


void CMOEAD::save_front(char saveFilename[4024])
{
    std::fstream fout;
//	fout.open(saveFilename,std::ios::out);
	fout.open(saveFilename,fstream::app|fstream::out );
	fout<<setprecision(3) << scientific;
	for(int n=0; n<pops; n++)
	{
		for(int k=0;k<nobj;k++)
			fout<<population[n].indiv.y_obj[k]<<"  ";
		fout<<"\n";
	}
	fout.close();
}

void CMOEAD::save_pos(char saveFilename[4024])
{
    std::fstream fout;
//	fout.open(saveFilename,std::ios::out);
	fout.open(saveFilename, fstream::app|fstream::out);

	fout<<setprecision(3) << scientific;
	for(int n=0; n<pops; n++)
	{
		for(int k=0;k<nvar;k++)
			fout<<population[n].indiv.x_var[k]<<"  ";
		fout<<"\n";
	}
	fout.close();
}

#endif
