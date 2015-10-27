#ifndef __REDIS_CONNECTION_POOL_H
#define __REDIS_CONNECTION_POOL_H

#include <hiredis/hiredis.h>
#include <pthread.h>

#define REDISPOOL_MAJOR 0
#define REDISPOOL_MINOR 1
#define REDISPOOL_SONAME 0.1

typedef struct redisConnectionPool {
    redisContext **connections;
    int allowedConnections;
    int usedConnections;
    pthread_mutex_t lock;
    const char *hostname;
    int port;
    struct timeval timeout;
} redisConnectionPool;

void redisFreeConnectionPool(redisConnectionPool *pool);

redisConnectionPool *redisCreateConnectionPool(int size, const char *hostname, int port, struct timeval timeout);

void redisPutConnectionInConnectionPool(redisContext *c, redisConnectionPool *pool);

redisContext *redisGetConnectionFromConnectionPool(redisConnectionPool *pool);

int redisConnectionsInConnectionPool(redisConnectionPool *pool);

int redisConnectionPoolDepth(redisConnectionPool *pool);

#endif
