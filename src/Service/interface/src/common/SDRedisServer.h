#ifndef SD_REDIS_SERVER_H
#define SD_REDIS_SERVER_H

#include <string>
#include <common/SDLogger.h>
#include <hiredis.h>

class SDRedisServer
{
public:
    SDRedisServer();
    ~SDRedisServer();

    int connect(const std::string& host, int port);
    bool is_connected();
    int close();
    
    int select(int db);
    int exists(const std::string& key);
    int set(const std::string& key, const std::string& value, int timeout=0);
    int del(const std::string& key);

private:
    DECL_LOGGER(logger);

    int reconnect();

    redisContext* m_c;
    std::string m_host;
    int m_port;
};

#endif
