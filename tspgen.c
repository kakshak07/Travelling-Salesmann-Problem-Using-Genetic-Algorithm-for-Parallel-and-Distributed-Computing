#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "mpi.h"
#include "common.h"
#include "map.h"
#include "population.h"

int mpiId = 0;

int main(int argc, char **argv){
	tspsMap_t map;
    tspsConfig_t config;
    tspsPopulation_t population;

    unsigned long int numGenerations = 0;
    int mpiNumProcs = 0;
    double start, end;

    //starting MPI directives
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&mpiNumProcs);
    MPI_Comm_rank(MPI_COMM_WORLD,&mpiId);

    logg("* Starting tspgen...\n");

    // parse the command line args
    if(readConfig(&config, argc, argv) != TSPS_RC_SUCCESS){
        return TSPS_RC_FAILURE;
    }

    // parse the map
    logg("* Parsing map...\n");
    if(parseMap(&map) != TSPS_RC_SUCCESS){
        logg("Error! Unable to read map 'maps/brazil58.tsp'!\n");
        return TSPS_RC_FAILURE;
    }

    // initialize random seed:
    srand ( time(NULL)*mpiId );

    logg("* Generating population...\n");
    if(generatePopulation(&population, &config) != TSPS_RC_SUCCESS){
        logg("Error! Failed to create a new random population!");
        return TSPS_RC_FAILURE;
    }

    // start a timer (mpi_barrier + mpi_wtime)
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    logg("* Initializing reproduction loop...\n");
    while(1){

        numGenerations++;

        calculateFitnessPopulation(&population, &map);

        if(numGenerations % 500 == 0){
            logg("- Generation %d...\n", numGenerations);
            printIndividual(&population.individuals[0], "Current Top Individual");
        }

        sortPopulation(&population);

        if(config.numGenerations > 0 && numGenerations == config.numGenerations){
            logg("* Max number of generations [%d] reached!\n", config.numGenerations);
            break;
        }

        crossoverPopulation(&population,  &config);

        mutatePopulation(&population, &config);

        // migrate population at every n generation
        if(numGenerations % config.migrationRate == 0){
            migrateIndividuals(&population, mpiId, mpiNumProcs, &config);
        }
    }

    // join all the populations
    joinPopulations(&population, mpiId, mpiNumProcs);
    sortPopulation(&population);

    // get the best inidividual and print it
    printIndividual(&population.individuals[0], "Top Individual");
    printIndividual(&population.individuals[config.populationSize-1], "Worst (of the top ones) Individual");

    // stop the timer
    MPI_Barrier(MPI_COMM_WORLD); /* IMPORTANT */
    end = MPI_Wtime();

    if(mpiId == 0) { /* use time on master node */
        printf("* Runtime = %f\n", end-start);
    }

    logg("* tspgen finished!\n");
    free(population.individuals);
    free(map.nodes);
    MPI_Finalize();

    return TSPS_RC_SUCCESS;
}



