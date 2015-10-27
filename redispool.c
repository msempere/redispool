#include "redispool.h"
#include <stdlib.h>

static redisConnectionPool *redisConnectionPoolInit(
        int size,
        const char *hostname,
        int port,
        struct timeval timeout){
    redisConnectionPool *c;
    c = calloc(1, sizeof(redisConnectionPool));

    if (c == NULL)
        return NULL;

    c->hostname = hostname;
    c->port = port;
    c->timeout = timeout;
    c->connections = malloc(sizeof(redisContext*) * size);

    if (c->connections == NULL)
        return NULL;

    c->allowedConnections = size;
    c->usedConnections = 0;

    return c;
}

redisConnectionPool *redisCreateConnectionPool(
        int size,
        const char *hostname,
        int port,
        struct timeval timeout){
    return redisConnectionPoolInit(size, hostname, port, timeout);
}

void redisFreeConnectionPool(redisConnectionPool *pool){
    if(pool != NULL && pool->connections != NULL){
        if(pool->usedConnections > 0){
            int i = 0;
            for(; i<pool->usedConnections; i++){
                if(pool->connections[i] != NULL){
                    redisFree(pool->connections[i]);
                    pool->connections[i] = NULL;
                }
            }
        }
        free(pool->connections);
        pool->connections = NULL;
    }
}

redisContext *redisGetConnectionFromConnectionPool(redisConnectionPool *pool){
    if(pool != NULL && pool->connections != NULL ){
        if(pool->usedConnections > 0){
            redisContext *c;
            pthread_mutex_lock(&pool->lock);
            c = pool->connections[pool->usedConnections - 1];
            pool->connections[pool->usedConnections - 1] = NULL;
            pool->usedConnections--;
            pthread_mutex_unlock(&pool->lock);
            return c;
        }
        else{
            return redisConnectWithTimeout(pool->hostname, pool->port, pool->timeout);
        }
    }
    else {
        return NULL;
    }
}

void redisPutConnectionInConnectionPool(redisContext *c, redisConnectionPool *pool){
    if(c != NULL && pool != NULL && !c->err && pool->connections != NULL
            && pool->usedConnections < pool->allowedConnections){
        pthread_mutex_lock(&pool->lock);
        pool->connections[pool->usedConnections] = c;
        pool->usedConnections++;
        pthread_mutex_unlock(&pool->lock);
    }
}

int redisConnectionsInConnectionPool(redisConnectionPool *pool){
    return pool->usedConnections;
}

int redisConnectionPoolDepth(redisConnectionPool *pool){
    return pool->allowedConnections;
}
