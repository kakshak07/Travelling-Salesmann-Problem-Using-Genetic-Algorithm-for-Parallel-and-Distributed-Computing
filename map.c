#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "map.h"
#include "string.h"


int parseMap(tspsMap_t *map){

    char *file = NULL;
    char *weights = NULL;
    char *aux = NULL;
    int i, j;

    if(loadMap(&file) != TSPS_RC_SUCCESS)
        return TSPS_RC_FAILURE;

    weights = strstr(file, "NODE_COORD_SECTION");
    weights += 19;

    map->numNodes = NUM_NODES;
    map->nodes = (tspsNode*)malloc(sizeof(tspsNode)*map->numNodes);

    aux = weights;
    i = 0;
    while(aux != NULL){
        if(i >= map->numNodes)
            break;

        // skip the node id
        if(i==0)
            aux = strtok(weights, " ");
        else
            aux = strtok(NULL, " ");

        if(i!=atoi(aux)-1){
            printf("Error! i is [%d], should be [%d]\n", i, atoi(aux));
        }

        aux = strtok(NULL, " ");
        map->nodes[i].x = atoi(aux);

        aux = strtok(NULL, " \n");
        map->nodes[i].y = atoi(aux);

        if(*aux == '\n'){
            aux++;
        }

        i++;
    }

    free(file);
    return TSPS_RC_SUCCESS;
}

int loadMap(char **file){

    FILE * pFile;
    long lSize;
    char *buffer;
    size_t result;

    if((pFile = fopen("maps/berlin52.tsp", "rb")) == NULL &&
        (pFile = fopen("../maps/berlin52.tsp", "rb")) == NULL &&
        (pFile = fopen("berlin52.tsp", "rb")) == NULL){

        return TSPS_RC_FAILURE;
    }

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL)
        return TSPS_RC_FAILURE;

    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize)
        return TSPS_RC_FAILURE;

    // terminate
    fclose (pFile);
    *file = buffer;
    return TSPS_RC_SUCCESS;

}


