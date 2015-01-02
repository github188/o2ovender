#include <algorithm>
#include <string>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "SDStatHandler.h"
#include <common/SDStringUtility.h>

using namespace std;

static const char *LSPLITTER = "[";
static const char *RSPLITTER = "]";
static const char *SPLITTER = ",";

//////////////////////////////////////////////////////////////////////////
//  SDStatItem
//////////////////////////////////////////////////////////////////////////

SDStatItem::SDStatItem(int stat_type)
{
	m_stat_type = stat_type;

	m_stat_count = 0;
    m_stat_succ_count = 0;
    m_stat_fail_count = 0;

    m_stat_time_count = 0;
    m_total_time = 0;
    m_avg_time = 0;
    m_max_time = 0;
    m_min_time = (unsigned int)-1;
}

bool SDStatItem::addavg(int& total, int& count, int& avg, int value)
{
	count ++;
	total += value;
	avg = (int)(total / count);
	return true;
}

void SDStatItem::addStat(int item_attr, int value)
{
	switch(item_attr)
	{
	case STAT_ITEM_TYPE_COUNT:
		m_stat_count++;
		break;
	case STAT_ITEM_TYPE_FAIL_COUNT:
		m_stat_fail_count++;
		break;
	case STAT_ITEM_TYPE_SUCC_COUNT:
		m_stat_succ_count++;
		break;
	case STAT_ITEM_TYPE_TIME:
		addtime((unsigned int)value);
		break;
	
	default:
		break;

	}
}

string SDStatItem::getStatName(int stat_type)
{
	return SDStringUtility::format(stat_type);
}

string SDStatItem::dump()
{
	string stat;

	//count
	stat += SDStringUtility::format(m_stat_count > 0 ? m_stat_count : (m_stat_succ_count+m_stat_fail_count) );
	stat += SPLITTER;
	stat += SDStringUtility::format(m_stat_succ_count);
	stat += SPLITTER;
	stat += SDStringUtility::format(m_stat_fail_count);
	stat += "-";

	//time
	stat += SDStringUtility::format(m_total_time);
	stat += SPLITTER;
	stat += SDStringUtility::format(m_avg_time);
	stat += SPLITTER;
	stat += SDStringUtility::format(m_max_time);
	stat += SPLITTER;
	stat += SDStringUtility::format(m_min_time != (unsigned int)-1 ? m_min_time : 0);
//	stat += SPLITTER;

	return stat;
}

bool SDStatItem::addtime(unsigned int time)
{
	m_stat_time_count++;

	//time
	m_total_time += time;
	m_avg_time = (unsigned int)(m_total_time/m_stat_time_count);
	if(m_stat_time_count == 1)
	{
		m_max_time = time;
		m_min_time = time;
	}
	else
	{
		if(m_max_time < time)
			m_max_time = time;
		if(m_min_time > time)
			m_min_time = time;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
//  SDStatHandler
//////////////////////////////////////////////////////////////////////////

SDStatHandler* SDStatHandler::m_instance = new SDStatHandler();

SDStatHandler::SDStatHandler() : SDThreadPool(1)
{
	m_statitem_pool.clear();
	pthread_mutex_init(&m_lock, NULL);
    m_start_time = SDStringUtility::format_time(time(NULL));
}

SDStatHandler::~SDStatHandler()
{
	pthread_mutex_destroy(&m_lock);
}

SDStatHandler* SDStatHandler::getInstance()
{
	return m_instance;
}

void SDStatHandler::close()
{
	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void SDStatHandler::init(int queue_size)
{
	m_stat_q = new SDStatQueue(queue_size);
}
    
void SDStatHandler::register_stat_name(int type, const std::string& name)
{
    m_stat_name_list[type] = name;
}

bool SDStatHandler::putStat(const SDStat& stat)
{
	return m_stat_q->push_nonblock(stat);
}

bool SDStatHandler::putStat(int type, int attr, int value)
{
	return putStat(SDStat(type, attr, value));
}

bool SDStatHandler::putStat(int type, int attr)
{
	return putStat(SDStat(type, attr, 0));
}

bool SDStatHandler::putBatchStat(int type)
{
	return putStat(type, STAT_ITEM_TYPE_COUNT);
}

bool SDStatHandler::putBatchStat(int type, bool is_succ)
{
	if(is_succ)
		return putStat(type, STAT_ITEM_TYPE_SUCC_COUNT);
	else
		return putStat(type, STAT_ITEM_TYPE_FAIL_COUNT);
}

bool SDStatHandler::putBatchStat(int type, bool is_succ, int time)
{
	if(is_succ)
		return putStat(type, STAT_ITEM_TYPE_SUCC_COUNT)
			&& putStat(type, STAT_ITEM_TYPE_TIME, time);
	else
		return putStat(type, STAT_ITEM_TYPE_FAIL_COUNT)
			&& putStat(type, STAT_ITEM_TYPE_TIME, time);
}

void SDStatHandler::doIt()
{
	while(true)
	{
		SDStat stat;
		if(m_stat_q->pop(stat))
		{
			processStat(stat);
		}
	}
}

void SDStatHandler::processStat(const SDStat& stat)
{
	pthread_mutex_lock(&m_lock);
	get_stat_item(stat.m_stat_type).addStat(stat.m_stat_item_attr, stat.m_stat_value);
	pthread_mutex_unlock(&m_lock);
}

SDStatItem& SDStatHandler::get_stat_item(int stat_type)
{
	SDStatItem insertItem(stat_type);
	vector<SDStatItem>::iterator it_find = lower_bound(m_statitem_pool.begin(), m_statitem_pool.end(), insertItem);

	if(it_find != m_statitem_pool.end() && it_find->m_stat_type == stat_type)
	{
		return (*it_find);
	}
	else
	{
		return (*m_statitem_pool.insert(it_find, insertItem));
	}
}

void SDStatHandler::dumpStat(char* file_name)
{
	pthread_mutex_lock(&m_lock);

	string end_time = SDStringUtility::format_time(time(NULL));
	string output;

	//statistics time
	output += LSPLITTER;
	output += m_start_time;
	output += SPLITTER;
	output += end_time;
	output += RSPLITTER;

	//stat item
	int count = m_statitem_pool.size();
	for(int i = 0; i < count; i++)
	{
		output += LSPLITTER;
        string name;
        std::map<int, std::string>::iterator it = m_stat_name_list.find(m_statitem_pool[i].m_stat_type);
        if (it != m_stat_name_list.end()) {
            name = it->second;
        }
        else {
            name = SDStringUtility::format(m_statitem_pool[i].m_stat_type);
        }

        output += name;
        output += SPLITTER;
		output += m_statitem_pool[i].dump();
		output += RSPLITTER;
	}

	m_statitem_pool.clear();
	m_start_time = end_time;

	//write to file
	FILE *fp = fopen(file_name, "a+");
	if(fp)
	{
		fprintf(fp, output.c_str());
		fprintf(fp, "\n");
		fclose(fp);
	}

	pthread_mutex_unlock(&m_lock);
}
