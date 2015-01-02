#ifndef SD_ACCOUNT_INFO_H
#define SD_ACCOUNT_INFO_H

#include <string>
#include <common/SDLogger.h>
#include <client/dbclient.h>

class SDAccountInfo
{
public:
    std::string m_uid;
    std::string m_passwd;
};

class SDMongoDBAccountInfo
{
public:
    static int query(mongo::DBClientConnection* mongodb, SDAccountInfo& account_info);
    static int insert(mongo::DBClientConnection* mongodb, SDAccountInfo& account_info);

private:
    static const std::string NS;
    static const std::string FIELD_UID;
    static const std::string FIELD_PASSWD;

protected:
    DECL_LOGGER(logger);
};

#endif
