#ifndef SD_MONGO_CLIENT_H
#define SD_MONGO_CLIENT_H

#include <string>
#include <vector>
#include <common/SDLogger.h>
#include <client/dbclient.h>

class SDMongoClient
{
public:
    SDMongoClient();

    int connect(const std::string& host, int port);
    
    int insert(const std::string& ns, const mongo::BSONObj& obj);
    int findone(const std::string& ns, const mongo::BSONObj& obj, mongo::BSONObj* robj);
    int findall(const std::string& ns, const mongo::BSONObj& obj, std::vector<mongo::BSONObj>* robj);

private:
    DECL_LOGGER(logger);
    mongo::DBClientConnection m_mongodb;
};

#endif
