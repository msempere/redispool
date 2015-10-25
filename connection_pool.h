#ifndef __REDIS_CONNECTION_POOL_H
#define __REDIS_CONNECTION_POOL_H

#include <hiredis/hiredis.h>

typedef struct redisConnectionPool {
    redisContext **connections;
    int allowedConnections;
    int usedConnections;

} redisConnectionPool;

void redisFreeConnectionPool(redisConnectionPool *pool);

redisConnectionPool *redisCreateConnectionPool(int size);

void redisPutConnectionInConnectionPool(redisContext *c, redisConnectionPool *pool);

redisContext *redisGetConnectionFromConnectionPool(redisConnectionPool *pool);

int redisConnectionsInConnectionPool(redisConnectionPool *pool);

int redisConnectionPoolDepth(redisConnectionPool *pool);

#endif
