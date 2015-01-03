#ifndef SD_MONGO_UTILITY_H
#define SD_MONGO_UTILITY_H

#include <string>
#include <vector>
#include <common/SDLogger.h>
#include <client/dbclient.h>

class SDMongoUtility
{
public:
    static int connect(mongo::DBClientConnection* mongodb, const std::string& host, int port);
    
    static int insert(mongo::DBClientConnection* mongodb, const std::string& ns, const mongo::BSONObj& obj);
    static int findone(mongo::DBClientConnection* mongodb, const std::string& ns, const mongo::BSONObj& obj, mongo::BSONObj* robj);
    static int findall(mongo::DBClientConnection* mongodb, const std::string& ns, const mongo::BSONObj& obj, std::vector<mongo::BSONObj>* robj);

private:
    DECL_LOGGER(logger);
};

#endif
