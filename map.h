#ifndef MAP_H_
#define MAP_H_

#include"common.h"

typedef struct{
    unsigned int x;
    unsigned int y;
} tspsNode;

typedef struct{
    int numNodes;
    tspsNode *nodes;
} tspsMap_t;

int parseMap(tspsMap_t *map);
int loadMap(char **file);

#endif /* MAP_H_ */
