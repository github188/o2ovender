#include "SDRedisServer.h"

using namespace std;

IMPL_LOGGER(SDRedisServer, logger);

SDRedisServer::SDRedisServer()
{
    m_c = NULL;
}

SDRedisServer::~SDRedisServer()
{
    close();
}

int SDRedisServer::connect(const std::string& host, int port)
{
    m_host = host;
    m_port = port;

    return reconnect();
}

int SDRedisServer::reconnect()
{
    m_c = redisConnect(m_host.c_str(), m_port);
    if (m_c->err != REDIS_OK) {
        close();
        return -1;
    }

    return 0;
}

bool SDRedisServer::is_connected()
{
    return m_c != NULL;
}

int SDRedisServer::close()
{
    if (m_c) {
        redisFree(m_c);
        m_c = NULL;
    }
    return 0;
}
    
int SDRedisServer::select(int db)
{
    int res = -1;

    if (!is_connected()) {
        if (reconnect() == -1) {
            return -1;
        }
    }

    redisReply *reply = (redisReply *)redisCommand(m_c, "SELECT %d", db);
    if (!reply) {
        close();
        return -1;
    }

    if (reply->type == REDIS_REPLY_STATUS && reply->str && reply->len && std::string(reply->str,reply->len) == "OK") {
        res = 0;
    }

    freeReplyObject(reply);
    return res;
}

int SDRedisServer::exists(const std::string& key)
{
    int res = -1;
    
    if (!is_connected()) {
        if (reconnect() == -1) {
            return -1;
        }
    }

    redisReply *reply = (redisReply *)redisCommand(m_c, "EXISTS %b", (char *)key.c_str(), key.length());
    if (!reply) {
        close();
        return -1;
    }

    if (reply->type == REDIS_REPLY_INTEGER) {
        res = reply->integer;
    }

    freeReplyObject(reply);
    return res;
}

int SDRedisServer::set(const std::string& key, const std::string& value, int timeout)
{
    int res = -1;
    
    if (!is_connected()) {
        if (reconnect() == -1) {
            return -1;
        }
    }

    redisReply *reply = NULL;
    if (timeout > 0) {
        reply = (redisReply *)redisCommand(m_c, "SET %b %b EX %d", (char *)key.c_str(), key.length(), (char *)value.c_str(), value.length(), timeout);
    }
    else {
        reply = (redisReply *)redisCommand(m_c, "SET %b %b", (char *)key.c_str(), key.length(), (char *)value.c_str(), value.length());
    }
    if (!reply) {
        close();
        return -1;
    }

    if (reply->str && reply->len && std::string(reply->str,reply->len) == "OK") {
        res = 0;
    }

    freeReplyObject(reply);
    return res;
}

int SDRedisServer::del(const std::string& key)
{
    int res = -1;
    
    if (!is_connected()) {
        if (reconnect() == -1) {
            return -1;
        }
    }

    redisReply *reply = (redisReply *)redisCommand(m_c, "DEL %b", (char *)key.c_str(), key.length());
    if (!reply) {
        close();
        return -1;
    }

    if (reply->type == REDIS_REPLY_INTEGER) {
        res = reply->integer;
    }

    freeReplyObject(reply);
    return res;
}
