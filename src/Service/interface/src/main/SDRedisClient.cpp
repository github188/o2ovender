#include "SDRedisClient.h"

using namespace std;

IMPL_LOGGER(SDRedisClient, logger);

SDRedisClient::SDRedisClient()
{
    m_c = NULL;
}

SDRedisClient::~SDRedisClient()
{
    close();
}

int SDRedisClient::connect(const std::string& host, int port)
{
    m_host = host;
    m_port = port;

    return reconnect();
}

int SDRedisClient::reconnect()
{
    m_c = redisConnect(m_host.c_str(), m_port);
    if (m_c->err != REDIS_OK) {
        LOG4CPLUS_ERROR(logger, "connect to redis " << m_host << ":" << m_port << " fail: " << m_c->errstr);
        close();
        return -1;
    }

    LOG4CPLUS_DEBUG(logger, "connect to redis " << m_host << ":" << m_port << " succ");
    return 0;
}

bool SDRedisClient::is_connected()
{
    return m_c != NULL;
}

int SDRedisClient::close()
{
    if (m_c) {
        redisFree(m_c);
        m_c = NULL;
    }
    return 0;
}
    
int SDRedisClient::select(int db)
{
    int res = -1;

    if (!is_connected()) {
        if (reconnect() == -1) {
            return -1;
        }
    }

    redisReply *reply = (redisReply *)redisCommand(m_c, "SELECT %d", db);
    if (!reply) {
        LOG4CPLUS_DEBUG(logger, "SELECT " << db << " fail: empty reply");
        close();
        return -1;
    }

    if (reply->type == REDIS_REPLY_STATUS && reply->str && reply->len && std::string(reply->str,reply->len) == "OK") {
        LOG4CPLUS_DEBUG(logger, "SELECT " << db << " succ");
        res = 0;
    }
    else {
        LOG4CPLUS_DEBUG(logger, "SELECT " << db << " fail: " << (reply->str && reply->len ? std::string(reply->str,reply->len) : ""));
    }

    freeReplyObject(reply);
    return res;
}

int SDRedisClient::exists(const std::string& key)
{
    int res = -1;
    
    if (!is_connected()) {
        if (reconnect() == -1) {
            return -1;
        }
    }

    redisReply *reply = (redisReply *)redisCommand(m_c, "EXISTS %b", (char *)key.c_str(), key.length());
    if (!reply) {
        LOG4CPLUS_DEBUG(logger, "EXISTS fail: empty reply");
        close();
        return -1;
    }

    if (reply->type == REDIS_REPLY_INTEGER) {
        LOG4CPLUS_DEBUG(logger, "EXISTS succ: " << reply->integer);
        res = reply->integer;
    }
    else {
        LOG4CPLUS_DEBUG(logger, "EXISTS fail: " << (reply->str && reply->len ? std::string(reply->str,reply->len) : ""));
    }

    freeReplyObject(reply);
    return res;
}

int SDRedisClient::set(const std::string& key, const std::string& value, int timeout)
{
    int res = -1;
    
    if (!is_connected()) {
        if (reconnect() == -1) {
            return -1;
        }
    }

    redisReply *reply = NULL;
    if (timeout > 0) {
        LOG4CPLUS_DEBUG(logger, "SET " << key << " " << value << " EX " << timeout);
        reply = (redisReply *)redisCommand(m_c, "SET %b %b EX %d", (char *)key.c_str(), key.length(), (char *)value.c_str(), value.length(), timeout);
    }
    else {
        reply = (redisReply *)redisCommand(m_c, "SET %b %b", (char *)key.c_str(), key.length(), (char *)value.c_str(), value.length());
    }
    if (!reply) {
        LOG4CPLUS_DEBUG(logger, "SET fail: empty reply");
        close();
        return -1;
    }

    if (reply->str && reply->len && std::string(reply->str,reply->len) == "OK") {
        LOG4CPLUS_DEBUG(logger, "SET succ");
        res = 0;
    }
    else {
        LOG4CPLUS_DEBUG(logger, "SET fail: " << (reply->str && reply->len ? std::string(reply->str,reply->len) : ""));
    }

    freeReplyObject(reply);
    return res;
}

int SDRedisClient::get(const std::string& key, std::string* value)
{
    int res = -1;
    
    if (!is_connected()) {
        if (reconnect() == -1) {
            return -1;
        }
    }

    redisReply *reply = NULL;
    reply = (redisReply *)redisCommand(m_c, "GET %b", (char *)key.c_str(), key.length());
    if (!reply) {
        LOG4CPLUS_DEBUG(logger, "GET fail: empty reply");
        close();
        return -1;
    }

    if (reply->type == REDIS_REPLY_STRING || reply->type == REDIS_REPLY_NIL) {
        LOG4CPLUS_DEBUG(logger, "GET succ");
        res = 0;
        if (reply->str && reply->len) {
            value->assign(reply->str,reply->len);
        }
        else {
            value->clear();
        }
    }
    else {
        LOG4CPLUS_DEBUG(logger, "GET fail: " << (reply->str && reply->len ? std::string(reply->str,reply->len) : ""));
    }

    freeReplyObject(reply);
    return res;
}
int SDRedisClient::del(const std::string& key)
{
    int res = -1;
    
    if (!is_connected()) {
        if (reconnect() == -1) {
            return -1;
        }
    }

    redisReply *reply = (redisReply *)redisCommand(m_c, "DEL %b", (char *)key.c_str(), key.length());
    if (!reply) {
        LOG4CPLUS_DEBUG(logger, "DEL fail: empty reply");
        close();
        return -1;
    }

    if (reply->type == REDIS_REPLY_INTEGER) {
        LOG4CPLUS_DEBUG(logger, "DEL succ: " << reply->integer);
        res = reply->integer;
    }
    else {
        LOG4CPLUS_DEBUG(logger, "DEL fail: " << (reply->str && reply->len ? std::string(reply->str,reply->len) : ""));
    }

    freeReplyObject(reply);
    return res;
}
