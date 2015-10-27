#include <redispool/redispool.h>
#include <hiredis/hiredis.h>

int main(void){

    const char *hostname = "127.0.0.1";
    int port = 6379;
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds

    redisConnectionPool *pool;
    pool = redisCreateConnectionPool(5, hostname, port, timeout);
    printf("Connections: %d\n", redisConnectionsInConnectionPool(pool));

    redisContext *connection;

    connection = redisConnectWithTimeout(hostname, port, timeout);
    if (connection == NULL || connection->err) {
        if (connection) {
            printf("Connection error: %s\n", connection->errstr);
            redisFree(connection);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
    }

    redisPutConnectionInConnectionPool(connection, pool);
    printf("Connections: %d\n", redisConnectionsInConnectionPool(pool));

    connection = redisGetConnectionFromConnectionPool(pool);
    printf("Connections: %d\n", redisConnectionsInConnectionPool(pool));

    if(connection != NULL){
        redisReply *reply;
        reply = redisCommand(connection,"PING");
        printf("PING: %s\n", reply->str);
        freeReplyObject(reply);
    }
    redisFreeConnectionPool(pool);
    return 0;
}
