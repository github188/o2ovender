#include <common/SDStringUtility.h>
#include <string.h>
#include <stdio.h>
//#include "base.h"
#include "SDStatDumper.h"

IMPL_LOGGER(SDStatDumper, logger)

SDStatDumper* SDStatDumper::m_instance = new SDStatDumper();
SDStatDumper::SDStatDumper() : SDThreadPool(1)
{}
SDStatDumper* SDStatDumper::getInstance()
{
	return m_instance;
}

void SDStatDumper::init(int dump_interval, const char *stat_file, const char *status_file)
{
    m_dump_interval = dump_interval;

	strcpy(m_stat_file, stat_file);
	strcpy(m_status_file, status_file);
}

void SDStatDumper::doIt()
{
    char file_name[max_path];

    while (true)
    {
        sleep(m_dump_interval);
        LOG4CPLUS_INFO(logger, "ready to dump stat and status dump interval=" << m_dump_interval);

        strcpy(file_name, m_stat_file);
        strcat(file_name, SDStringUtility::get_local_datestring().c_str());
        SDStatHandler::getInstance()->dumpStat(file_name);
/*
        if (m_status)
        {
            strcpy(file_name, m_status_file);
            strcat(file_name, SDStringUtility::get_local_datestring().c_str());
            //m_status->dump(file_name);
        }
        else
		{
			LOG4CPLUS_ERROR(logger, "bad status pointer");
		}
*/
	}
}
