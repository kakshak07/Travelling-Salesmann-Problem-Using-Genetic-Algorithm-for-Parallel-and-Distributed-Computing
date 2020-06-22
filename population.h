#ifndef POPULATION_H_
#define POPULATION_H_

#include "common.h"
#include "map.h"

typedef struct{
    int chrom[NUM_NODES];
    int fitness;
    double probability;
    int index;

}tspsIndividual_t;

typedef struct{
    tspsIndividual_t *individuals;
    int numIndividuals;
    double totalFitness;
}tspsPopulation_t;

int generatePopulation(tspsPopulation_t *pop, tspsConfig_t *config);
int generateRandomChromosome(tspsIndividual_t *ind, int index);
int generateRandomChromosome2(tspsIndividual_t *ind);
void swap(int *a, int *b);
double calculateFitnessChromosome(int *chromosome, tspsMap_t *map);
int generateNewPopulation(tspsPopulation_t *pop, tspsConfig_t *config);
int compare (const void *a, const void *b);
int sortPopulation(tspsPopulation_t *pop);
int crossoverPopulation(tspsPopulation_t *pop,  tspsConfig_t *config);
int mutatePopulation(tspsPopulation_t *pop,  tspsConfig_t *config);
int calculateFitnessPopulation(tspsPopulation_t *pop, tspsMap_t *map);
int migrateIndividuals(tspsPopulation_t *pop, int mpiId, int numProcs, tspsConfig_t *config);
int joinPopulations(tspsPopulation_t *pop, int mpiId, int mpiNumProcs);
void printIndividual(tspsIndividual_t *ind, const char *label);
#endif /* POPULATION_H_ */

