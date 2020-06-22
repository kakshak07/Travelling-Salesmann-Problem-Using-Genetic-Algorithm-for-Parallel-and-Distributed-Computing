#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <assert.h>
#include "common.h"
#include "population.h"
#include "map.h"

//Each process generates its own sub-population using the pseudo random number generator
int generatePopulation(tspsPopulation_t *pop, tspsConfig_t *config){

	pop->numIndividuals = config->populationSize;
	pop->individuals = (tspsIndividual_t*)malloc(pop->numIndividuals * sizeof(tspsIndividual_t));

	int i;
	for(i=0; i<pop->numIndividuals; i++){
		generateRandomChromosome(&pop->individuals[i], i);
    	}

	return TSPS_RC_SUCCESS;
}

//Generates a new random chromossome
int generateRandomChromosome(tspsIndividual_t *ind, int index){
	int ii;
	for(ii=0; ii<NUM_NODES; ii++){
		ind->chrom[ii] = ii;   // city index starts from zero
	}

	struct timeval tv;
    	gettimeofday(&tv, NULL);
    	int usec = tv.tv_usec+index;
    	srand48(usec);

    	size_t i;
    	for (i = NUM_NODES - 1; i > 0; i--) {
        		size_t j = (unsigned int) (drand48()*(i+1));
        		int t = ind->chrom[j];
                ind->chrom[j] = ind->chrom[i];
        		ind->chrom[i] = t;
    	}

	return TSPS_RC_SUCCESS;
}

//This function calculates the fitness value of each chromosome the parsed map stored in the directory
double calculateFitnessChromosome(int *chromosome, tspsMap_t *map){
	int fitnessValue = 0.0;
	int i, firstCity, secondCity;
    int xd, yd;
	for (i=0; i<NUM_NODES-1; i++){
		firstCity = chromosome[i];
		secondCity = chromosome[i+1];
        xd = map->nodes[firstCity].x - map->nodes[secondCity].x;
        yd = map->nodes[firstCity].y - map->nodes[secondCity].y;
		fitnessValue = fitnessValue +  rint(sqrt(xd*xd + yd*yd));
	}
	return fitnessValue;
}

void swap (int *a, int *b){
    	int temp = *a;
    	*a = *b;
    	*b = temp;
}

int compare (const void *a, const void *b)
{

  tspsIndividual_t * popA = (tspsIndividual_t *)a;
  tspsIndividual_t * popB = (tspsIndividual_t *)b;

  return ( popA->fitness - popB->fitness );
}

//mutate the population
int mutatePopulation(tspsPopulation_t *pop, tspsConfig_t *config){

    tspsIndividual_t *ind = NULL;
    int alreadySwaped[NUM_NODES];
    int mutationRate = config->mutationRate * 100;
    int index1, index2;     //the index of the nodes to be swapped
    int aux;
    int i, j;

    for(i=config->numElitism; i<pop->numIndividuals; i++){
        if(rand()%100 > mutationRate)
            continue;

        memset(alreadySwaped, 0, sizeof(alreadySwaped));
        ind = &pop->individuals[i];

        //mutate!
        //swap mutationSize nodes in the chromosome
        for(j=0; j<config->mutationSize; j++){
            index1 = rand() % NUM_NODES;

            //if already swaped, jump to the next of the list
            while(alreadySwaped[index1] !=0){
                if(index1 + 1 < NUM_NODES)
                    index1++;
                else
                    index1 = 0;
            }
            alreadySwaped[index1] = 1;

            index2 = rand() % NUM_NODES;

            //if already swaped, jump to the next of the list
            while(alreadySwaped[index2] !=0){
                if(index2 + 1 < NUM_NODES)
                    index2++;
                else
                    index2 = 0;
            }
            alreadySwaped[index2] = 1;

            //swap the nodes
            aux = ind->chrom[index1];
            ind->chrom[index1] = ind->chrom[index2];
            ind->chrom[index2] = aux;
        }
    }

    return TSPS_RC_SUCCESS;

}

//sort the population by increasing fitness
int sortPopulation(tspsPopulation_t *pop){
    qsort(pop->individuals, pop->numIndividuals, sizeof(tspsIndividual_t), compare);
    return TSPS_RC_SUCCESS;
}

//This function is an implementation of PMX crossover operation
int crossoverPopulation(tspsPopulation_t *pop, tspsConfig_t *config){

	int i, j, k, l;
	for (i=0; i < config->populationSize; i++){
		pop->individuals[i].probability = pop->individuals[i].fitness/pop->totalFitness;
	}

	double rndNumber_one, rndNumber_two ;
	int cross_pone, cross_ptwo;

	double offset_one = 0.0;
	double offset_two = 0.0;

	int pick_one = 0, pick_two = 0;
	int count = config->numElitism;

	int temp;
	int max = NUM_NODES -1;
	int min = 1;
	for (count = 0 ; count<500; count= count+2){
		// selection of two parents based on sampling from the multinomial distribution
		rndNumber_one = rand() / (double) RAND_MAX;
		rndNumber_two = rand() / (double) RAND_MAX;
		//count = count+2;
		for (i = 0; i < config->populationSize; i++) {
			offset_one += pop->individuals[i].probability;
			if (rndNumber_one < offset_one) {
        			pick_one = i;
        			break;
    			}
		}
		for (i = 0; i < config->populationSize; i++) {
			offset_two += pop->individuals[i].probability;

			if (rndNumber_two < offset_two) {
        			pick_two = i;
        			break;
    			}
		}
		offset_one=0; offset_two=0;

		//pmx function
		//random selection of two delimiters - crossover points
		cross_pone = (max - min + 1)*(double)rand()/RAND_MAX + min;
		cross_ptwo = (max - min + 1)*(double)rand()/RAND_MAX + min;

		if(cross_ptwo<cross_pone){
			temp= cross_ptwo;
			cross_ptwo = cross_pone;
			cross_pone = temp;
		}
		int child_1[NUM_NODES] = {0};
		int child_2[NUM_NODES] = {0};

		for (i=0; i<NUM_NODES; i++){
			child_1[i] = pop->individuals[pick_one].chrom[i]; //parent(1) number 10
			child_2[i] = pop->individuals[pick_two].chrom[i]; //parent(2) number 11
		}
		int *vec_1 = (int*)malloc((cross_ptwo-cross_pone)*sizeof(int));
		int *vec_2 = (int*)malloc((cross_ptwo-cross_pone)*sizeof(int));
		int num=0;

		//swapping the segment of chromosome between the two delimiters
		for (i = cross_pone; i < cross_ptwo; i++ ){
			int tem = child_1[i];
			child_1[i] = child_2[i]; //
			child_2[i] = tem; //

			vec_1[num] = child_1[i];
			vec_2[num] = child_2[i];
			num++;
		}

		//copy rest of the segment from parents to children
		for(i = 0; i<cross_pone; i++){
			child_1[i] = pop->individuals[pick_one].chrom[i];
			child_2[i] = pop->individuals[pick_two].chrom[i];
		}
		for(i = cross_ptwo; i<NUM_NODES; i++){
			child_1[i] = pop->individuals[pick_one].chrom[i];
			child_2[i] = pop->individuals[pick_two].chrom[i];
		}
		int buf, flag=0;
		//solving the conflicts that arise due to above swapping procedure.
		for (i = 0; i < num; i++ ){
			if(flag==1){
				i=i-1;
				flag=0;
			}
			for (j = 0; j < num; j++ ){
				if((vec_1[i]==vec_2[j])&&(vec_1[i]!=vec_2[i] )){
					buf = vec_1[i];
					vec_1[i] = vec_1[j];
					vec_1[j] = buf;
					flag=1;
					continue;
				}
			}
		}
		for (i=0; i<num; i++){
			for(j = 0; j<cross_pone; j++){
				if(vec_1[i]==child_1[j]){
					child_1[j] = vec_2[i];
				}
				if(vec_2[i]==child_2[j]){
					child_2[j] = vec_1[i];
				}

			}
			for(j = cross_ptwo; j<NUM_NODES; j++){
				if(vec_1[i]==child_1[j]){
					child_1[j] = vec_2[i];
				}
				if(vec_2[i]==child_2[j]){
					child_2[j] = vec_1[i];
				}
			}
		}
		// replacing the worst candidates in the previous generations with the new children.
		for (i=0; i<NUM_NODES; i++){
			pop->individuals[config->populationSize-count-1].chrom[i] = child_1[i];
			pop->individuals[config->populationSize-count-2].chrom[i] = child_2[i];
		}
        	free(vec_1);
        	free(vec_2);
	}

    return 0;
}

// This function calculates the fitness value of each chromosome.
int calculateFitnessPopulation(tspsPopulation_t *pop, tspsMap_t *map){
	int i;
    int totalFitness = 0.0;

    for(i=0; i<pop->numIndividuals; i++){
        pop->individuals[i].fitness = calculateFitnessChromosome(&pop->individuals[i].chrom[0], map);
        totalFitness += pop->individuals[i].fitness;
    }

    pop->totalFitness = totalFitness;

    return TSPS_RC_SUCCESS;
}

// COMMUNICATION : using RED-BLACK communication
int migrateIndividuals(tspsPopulation_t *pop, int mpiId, int numProcs, tspsConfig_t *config){

    tspsIndividual_t *emigrant1 = NULL, *emigrant2 = NULL;
    tspsIndividual_t *imigrant1 = NULL, *imigrant2 = NULL;
    int i=0, j=-1;
    MPI_Status status;

    // define how many individuals are going to migrate
    int numMigrants = floor(pop->numIndividuals * config->migrationShare);

    //choose randomly for now the individual to migrate
    i = rand() % (pop->numIndividuals - numMigrants);
    emigrant1 = &pop->individuals[i];
    imigrant1 = (tspsIndividual_t *)malloc(sizeof(tspsIndividual_t) * numMigrants);

    // choose another emigrant if they are in the middle of the process chain
    if(mpiId > 0 && mpiId < numProcs-1){
        do{
            j = rand() % (pop->numIndividuals - numMigrants);
        } while(j == i);

        emigrant2 = &pop->individuals[j];
        imigrant2 = (tspsIndividual_t *)malloc(sizeof(tspsIndividual_t) * numMigrants);
    }

    /* red/black communication*/
    if (mpiId % 2 != 0){
        /* i'm red */

        if(mpiId < numProcs - 1){
            /* send emigrant to black at my right */
            //printf("+ Pr[%d]: Sending to [%d]... \n", mpiId, mpiId+1);
            MPI_Send(emigrant1, sizeof(tspsIndividual_t) * numMigrants, MPI_CHAR, mpiId+1, MPI_MIGRATION_TAG, MPI_COMM_WORLD);

            /* receive imigrant from black at my right */
            //printf("+ Pr[%d]: Receiving from [%d]... \n", mpiId, mpiId+1);
            MPI_Recv(imigrant1, sizeof(tspsIndividual_t) * numMigrants, MPI_CHAR, mpiId+1, MPI_MIGRATION_TAG, MPI_COMM_WORLD, &status );
        }

        if(mpiId > 0){
            /* send emigrant to black at my left */
            //printf("+ Pr[%d]: Sending to [%d]... \n", mpiId, mpiId-1);
            MPI_Send((mpiId < numProcs - 1? emigrant2 : emigrant1), sizeof(tspsIndividual_t) * numMigrants, MPI_CHAR, mpiId-1, MPI_MIGRATION_TAG, MPI_COMM_WORLD);

            /* receive imigrant from black at my left */
            //printf("+ Pr[%d]: Receiving from [%d]... \n", mpiId, mpiId-1);
            MPI_Recv((mpiId < numProcs - 1? imigrant2 : imigrant1), sizeof(tspsIndividual_t) * numMigrants, MPI_CHAR, mpiId-1, MPI_MIGRATION_TAG, MPI_COMM_WORLD, &status );
        }

    } else {
        /* i'm black */
        if(mpiId > 0){
            /* receive imigrant from red at my left */
            //printf("- Pr[%d]: Receiving from [%d]... \n", mpiId, mpiId-1);
            MPI_Recv(imigrant1, sizeof(tspsIndividual_t) * numMigrants, MPI_CHAR, mpiId-1, MPI_MIGRATION_TAG, MPI_COMM_WORLD, &status );


            /* send emigrant to red at my left */
            //printf("- Pr[%d]: Sending to [%d]... \n", mpiId, mpiId-1);
            MPI_Send(emigrant1, sizeof(tspsIndividual_t) * numMigrants, MPI_CHAR, mpiId-1, MPI_MIGRATION_TAG, MPI_COMM_WORLD);
        }

        if(mpiId < numProcs-1){
            /* receive imigrant from red at my right */
            //printf("- Pr[%d]: Receiving from [%d]... \n", mpiId, mpiId+1);
            MPI_Recv((mpiId > 0? imigrant2 : imigrant1), sizeof(tspsIndividual_t) * numMigrants, MPI_CHAR, mpiId+1, MPI_MIGRATION_TAG, MPI_COMM_WORLD, &status );

            /* send emigrant to red at my right */
            //printf("- Pr[%d]: Sending to [%d]... \n", mpiId, mpiId+1);
            MPI_Send((mpiId > 0? emigrant2 : emigrant1), sizeof(tspsIndividual_t) * numMigrants, MPI_CHAR, mpiId+1, MPI_MIGRATION_TAG, MPI_COMM_WORLD);
        }
    }

    // move imigrants to our current population
    if(numProcs > 1){
        memcpy(&pop->individuals[i], imigrant1, sizeof(tspsIndividual_t) * numMigrants);
    }

    if(mpiId > 0 && mpiId < numProcs-1){
        memcpy(&pop->individuals[j], imigrant2, sizeof(tspsIndividual_t) * numMigrants);
    }

    free(imigrant1);
    free(imigrant2);
}

int joinPopulations(tspsPopulation_t *pop, int mpiId, int mpiNumProcs){
    MPI_Status status;
    int i, j;
    int indPerPop = floor(pop->numIndividuals / mpiNumProcs);
    int indIdx = indPerPop;

    if(mpiId == 0){
        // get the top individuals from other populations
        for(i=1; i<mpiNumProcs; i++){
            for(j=0; j<indPerPop; j++){
                MPI_Recv(&pop->individuals[indIdx], sizeof(tspsIndividual_t), MPI_CHAR, i, MPI_MIGRATION_TAG, MPI_COMM_WORLD, &status);
                indIdx++;
            }
        }
    }else{
        for(j=0; j<indPerPop; j++){
            MPI_Send(&pop->individuals[j], sizeof(tspsIndividual_t), MPI_CHAR, 0, MPI_MIGRATION_TAG, MPI_COMM_WORLD);
            indIdx++;
        }
    }
}

void printIndividual(tspsIndividual_t *ind, const char *label){
    int i;
    logg("** %s\n", label);
    logg("-- fitness = [%d]\n", ind->fitness);
    logg("-- chromos = ");
    for(i=0; i<NUM_NODES; i++){
        logg("[%d]", ind->chrom[i]);
        if((i+1) % 10 == 0)
            logg("\n\t");
    }
    logg("\n--------------------------------------------------------------\n");
}
