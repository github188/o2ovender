/********************************************************************
	created:		2005/08/26
	created:		26:8:2005   13:17
	filename: 		d:\work\newcvs\chub\impl\src\chub\sdstatdumper.h
	file path:		d:\work\newcvs\chub\impl\src\chub
	file base:		sdstatdumper
	file ext:		h
	author:			lifeng
	description:	statistics and status dumper
*********************************************************************/

#ifndef SANDAI_C_STATISTICSANDSTATUSDUMPER_H_200508261317
#define SANDAI_C_STATISTICSANDSTATUSDUMPER_H_200508261317

#include <common/SDThreadPool.h>
#include <common/SDLogger.h>
#include "SDStatHandler.h"
//#include "SDServerStatusMonitor.h"

class SDStatDumper : public SDThreadPool
{
public:
	static const int max_path = 256;
	void init(int dump_interval, const char* stat_file, const char* status_file);
    static SDStatDumper* getInstance();

protected:
	virtual void doIt();

private:
	DECL_LOGGER(logger);
	SDStatDumper();
	static SDStatDumper* m_instance;

	char m_stat_file[max_path];
	char m_status_file[max_path];

	int m_dump_interval;
	//SDServerStatusMonitor* m_status;
};

#endif
