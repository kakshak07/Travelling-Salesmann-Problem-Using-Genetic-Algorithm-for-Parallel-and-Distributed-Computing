#include "common.h"
#include "stdio.h"
#include <stdlib.h>
#include <stdarg.h>

extern int mpiId;

void logg(const char * format, ...){
#ifdef DEBUG
    if(mpiId != 0)
        return;

    va_list(args);
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
#endif
}

int readConfig(tspsConfig_t *config, int argc, char **argv){
    int populationSize = DFLT_POPSIZE;
    double mutationRate = DFLT_MUTRATE;
    int numGenerations = DFLT_NUMGEN;
    int numElitism = DFLT_NUMELIT;
    int mutationSize = DFLT_MUTSIZE;
    int migrationRate = DFLT_MIGRATE;
    float migrationShare = DFLT_MIGSHAR;

    if(argc > 1){
        if(argc != 8){
            logg("Invalid number of args\n Usage: ./tspgen <population size> <mutation rate> <number generations> <elitism number> <mutation size> <migration rate> <migration share>\n");
            return TSPS_RC_FAILURE;
        }

        populationSize = atoi(argv[1]);
        if(populationSize <= 0){
            logg("Invalid population size (> 0)!\n Usage: ./tspgen <population size> <mutation rate> <number generations> <elitism number> <mutation size> <migration rate> <migration share>\n");
            return TSPS_RC_FAILURE;
        }

        mutationRate = atof(argv[2]);
        if(mutationRate < 0 || mutationRate > 1){
            logg("Invalid mutation rate (0..1)!\n Usage: ./tspgen <population size> <mutation rate> <number generations> <elitism number> <mutation size> <migration rate> <migration share>\n");
            return TSPS_RC_FAILURE;
        }

        numGenerations = atoi(argv[3]);
        if(numGenerations < 100){
            logg("Invalid number of generations (>= 100)!\n Usage: ./tspgen <population size> <mutation rate> <number generations> <elitism number> <mutation size> <migration rate> <migration share>\n");
            return TSPS_RC_FAILURE;
        }

        numElitism = atoi(argv[4]);
        if(numElitism < 0 || numElitism >= populationSize){
            logg("Invalid elitism number (population size < elitism <= 0)!\n Usage: ./tspgen <population size> <mutation rate> <number generations> <elitism number> <mutation size> <migration rate> <migration share>\n");
            return TSPS_RC_FAILURE;
        }

        mutationSize = atoi(argv[5]);
        if(mutationSize < 1 || mutationSize > NUM_NODES/2){
            logg("Invalid mutation size (%d < mutation size < 0)!\n Usage: ./tspgen <population size> <mutation rate> <number generations> <elitism number> <mutation size> <migration rate> <migration share>\n", NUM_NODES/2);
            return TSPS_RC_FAILURE;
        }

        migrationRate = atoi(argv[6]);
        if(migrationRate >= numGenerations){
            logg("Invalid migration rate size (migration rate < num generations)!\n Usage: ./tspgen <population size> <mutation rate> <number generations> <elitism number> <mutation size> <migration rate> <migration share>\n", NUM_NODES/2);
            return TSPS_RC_FAILURE;
        }

        migrationShare = atof(argv[7]);
        if(migrationShare > 0.5 || migrationShare < 0.0){
            logg("Invalid migration share (0 < migration share <= 0.5)!\n Usage: ./tspgen <population size> <mutation rate> <number generations> <elitism number> <mutation size> <migration rate> <migration share>\n", NUM_NODES/2);
            return TSPS_RC_FAILURE;
        }
    }

    config->populationSize = populationSize;
    config->mutationRate = mutationRate;
    config->numGenerations = numGenerations;
    config->numElitism = numElitism;
    config->mutationSize = mutationSize;
    config->migrationRate = migrationRate;
    config->migrationShare = migrationShare;

    logg("* Config:\n");
    logg("-- Population Size = [%d]\n", config->populationSize);
    logg("-- Mutation Rate = [%f]\n", config->mutationRate);
    logg("-- Num of Generations = [%d]\n", config->numGenerations);
    logg("-- Num of Elitism = [%d]\n", config->numElitism);
    logg("-- Mutation Size = [%d]\n", config->mutationSize);
    logg("-- Migration Rate = [%d]\n", config->migrationRate);
    logg("-- Migration Share = [%f]\n", config->migrationShare);

    return TSPS_RC_SUCCESS;

}
