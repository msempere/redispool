![alt logo](http://i.imgur.com/0yz70i0.png?1)
# RedisPool ![alt build](https://travis-ci.org/msempere/redispool.svg?branch=master)
Connection pool for Redis

### Dependencies
RedisPool requires hiredis only

### Install
Installing hiredis, on a Debian system:
```
sudo apt-get install libhiredis-dev
```

Or manually:
```
git clone https://github.com/redis/hiredis.git
cd hiredis & make & make install
```

Or using makefile from RedisPool:
```
make hiredis
```

Installing RedisPool:
```
git clone https://github.com/msempere/redispool.git
cd redispool & make & make install
```

### License
Distributed under MIT license. See `LICENSE` for more information.
