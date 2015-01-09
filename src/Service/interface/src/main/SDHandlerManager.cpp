#include "SDHandlerManager.h"

#include <common/SDStringUtility.h>
#include <common/SDSocketFactory.h>
#include <common/SDNetFramework.h>

#include <mobile/SDMobileSocket.h>
#include <mobile/SDMobileRequestFactory.h>
#include <mobile/SDMobileLoginRequest.h>
#include <mobile/SDMobileRegisterRequest.h>
#include <mobile/SDMobileCodeRequest.h>
#include <mobile/SDMobileCommodityRequest.h>

#include <avm/SDAVMSocket.h>
#include <avm/SDAVMRequestFactory.h>
#include <avm/SDAVMSignInRequest.h>

#include "SDEchoSocket.h"
#include "SDProxySocket.h"

using namespace std;

IMPL_LOGGER(SDHandlerManager, logger);

SDHandlerManager::SDHandlerManager(const std::string& conf_file)
{
    m_conf_file = conf_file;
}

bool SDHandlerManager::init()
{
    return (read_conf()
            && init_queue()
            && init_thread()
           );
}

void SDHandlerManager::start()
{
    SDNetFramework::startThreads();
    m_work_handler->startThreads();
    m_avm_work_handler->startThreads();
}

void SDHandlerManager::wait()
{
    SDNetFramework::waitThreadsTermination();
    m_work_handler->waitThreadsTermination();
    m_avm_work_handler->waitThreadsTermination();
}

bool SDHandlerManager::init_queue()
{
    SDMobileRequestFactory::register_object(o2ovender::request_TYPE_LOGIN, new SDMobileLoginRequest());
    SDMobileRequestFactory::register_object(o2ovender::request_TYPE_REGISTER, new SDMobileRegisterRequest());
    SDMobileRequestFactory::register_object(o2ovender::request_TYPE_GET_IDENTIFYING_CODE, new SDMobileCodeRequest());
    SDMobileRequestFactory::register_object(o2ovender::request_TYPE_COMMODITY_LIST, new SDMobileCommodityRequest());
    
    SDAVMRequestFactory::register_object(SDAVMRequest::SIGN_IN, new SDAVMSignInRequest());

    return true;
}

bool SDHandlerManager::init_thread()
{
    SDNetFramework::doConfigure("../conf/netframework.conf");

    mongo::Status status = mongo::client::initialize();
    if ( !status.isOK() ) {
        LOG4CPLUS_FATAL(logger, "failed to initialize the client driver: " << status.toString() );
        return false;
    }
    
    int handler_size = m_config.getInt("work.handler.count", 1);
    m_work_handler = boost::shared_ptr<SDWorkHandler>(new SDWorkHandler(handler_size));
    if (!m_work_handler->init(m_config)) {
        return false;
    }
    
    handler_size = m_config.getInt("work.handler.count", 1);
    m_avm_work_handler = boost::shared_ptr<SDAVMWorkHandler>(new SDAVMWorkHandler(handler_size));
    if (!m_avm_work_handler->init(m_config)) {
        return false;
    }
    
    std::vector<std::string>& listen_ipv4 = SDNetFramework::m_listen_ipv4;
    for (std::vector<std::string>::iterator it = listen_ipv4.begin(); it != listen_ipv4.end(); ++it) {
        if (it->find(":80") != string::npos && (it->find(":80")+3) == it->length()) {
            SDMobileSocket* socket = new SDMobileSocket();
            socket->init(m_work_handler);
            SDSocketFactory::register_socket(*it, socket);       
        }
        else if (it->find(":8000") != string::npos && (it->find(":8000")+5) == it->length()) {
            SDAVMSocket* socket = new SDAVMSocket();
            socket->init(m_avm_work_handler);
            SDSocketFactory::register_socket(*it, socket);       
        }
    }
    
    return true;
}

bool SDHandlerManager::read_conf()
{
    if (!m_config.load(m_conf_file.c_str()))
    {
        LOG4CPLUS_ERROR(logger, "can not open config file " << m_conf_file);
        return false;
    }

    return true;
}
