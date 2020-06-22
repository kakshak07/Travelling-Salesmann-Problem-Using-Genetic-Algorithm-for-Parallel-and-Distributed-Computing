#ifndef COMMON_H_
#define COMMON_H_

#define TSPS_RC_SUCCESS 0
#define TSPS_RC_FAILURE 1

#define NUM_NODES 52
#define MPI_MIGRATION_TAG 100

// Default values
#define DFLT_POPSIZE 5000
#define DFLT_MUTRATE 0.1
#define DFLT_NUMGEN  10000
#define DFLT_NUMELIT 100
#define DFLT_MUTSIZE 2
#define DFLT_MIGRATE 500
#define DFLT_MIGSHAR 0.01

typedef struct{
    int populationSize;
    double mutationRate;
    int numGenerations;
    int numElitism;
    int mutationSize;
    int migrationRate;
    float migrationShare;
} tspsConfig_t;

void logg(const char * format, ...);
int readConfig(tspsConfig_t *config, int argc, char **argv);

#endif /* COMMON_H_ */
