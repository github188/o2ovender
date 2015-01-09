/********************************************************************
	created:		2005/08/25
	created:		25:8:2005   17:01
	filename: 		d:\work\newcvs\chub\impl\src\chub\sdstatqueue.h
	file path:		d:\work\newcvs\chub\impl\src\chub
	file base:		sdstatqueue
	file ext:		h
	author:			lifeng
	description:	statistics item queue
*********************************************************************/

#ifndef SANDAI_C_STATISTICSITEM_H_200508251701
#define SANDAI_C_STATISTICSITEM_H_200508251701

#include <common/SDQueue.h>

class SDStat
{
public:
	SDStat()
	{
		m_stat_type = 0;
		m_stat_item_attr = 0;
		m_stat_value = 0;
	}
	SDStat(int type, int attr, int value)
	{
		m_stat_type = type;
		m_stat_item_attr = attr;
		m_stat_value = value;
	}

	int m_stat_type;
	int m_stat_item_attr;
	int m_stat_value;
};

typedef SDQueue<SDStat> SDStatQueue;

#endif
