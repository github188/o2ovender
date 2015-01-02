#ifndef SD_STAT_HANDLER_H
#define SD_STAT_HANDLER_H

#include <vector>
#include <map>
#include <common/SDThreadPool.h>
#include "SDStatQueue.h"

static const int STAT_ITEM_TYPE_COUNT = 1;
static const int STAT_ITEM_TYPE_FAIL_COUNT = 2;
static const int STAT_ITEM_TYPE_SUCC_COUNT = 3;
static const int STAT_ITEM_TYPE_TIME = 4;

namespace Stats
{
    static const int Accept = 1;
};

class SDStatItem
{
public:
	int m_stat_type;

	SDStatItem(int stat_type);
	void addStat(int item_attr, int value);
	std::string dump();
	std::string getStatName(int item_attr);

	bool friend operator< (const SDStatItem& s1, const SDStatItem& s2)
	{
		return s1.m_stat_type < s2.m_stat_type;
	}

protected:
private:
	//count
    int m_stat_count;
    int m_stat_succ_count;
    int m_stat_fail_count;

    //time(ms)
    int m_stat_time_count;
    unsigned int m_total_time;
    unsigned int m_avg_time;
    unsigned int m_max_time;
    unsigned int m_min_time;

    //size
    int m_stat_recv_size_count;
    int m_recv_size;
    int m_avg_recv_size;
    int m_stat_send_size_count;
    int m_send_size;
    int m_avg_send_size;

    //other
    int m_adder0;
    int m_adder1;
    int m_adder2;
    int m_stat0;
    int m_stat1;
    int m_stat2;
    int m_stat3;

	int m_avg0;
	int m_avg0_count;
	int m_avg0_total;
	int m_avg1;
	int m_avg1_count;
	int m_avg1_total;
	int m_avg2;
	int m_avg2_count;
	int m_avg2_total;
	int m_avg3;
	int m_avg3_count;
	int m_avg3_total;

	bool addrecvsize(int size);
    bool addsendsize(int size);
    bool addtime(unsigned int time);

	bool addavg(int& total, int& count, int& avg, int value);

};

class SDStatHandler : public SDThreadPool
{

public:
	virtual ~SDStatHandler();

	void init(int queue_size);
    void register_stat_name(int type, const std::string& name);

    static SDStatHandler* getInstance();
	static void close();

	bool putStat(const SDStat& stat);
	bool putStat(int type, int attr);
	bool putStat(int type, int attr, int value);

	//add total count
	bool putBatchStat(int type);
	//add succ or fail count
	bool putBatchStat(int type, bool is_succ);
	//add succ or fail count and time
	bool putBatchStat(int type, bool is_succ, int time);

	void dumpStat(char* file_name);

protected:
	virtual void doIt();

private:
	SDStatHandler();
	void processStat(const SDStat&);
	SDStatItem& get_stat_item(int stat_type);

	pthread_mutex_t m_lock;

	std::string m_start_time;
	SDStatQueue* m_stat_q;
	std::vector<SDStatItem> m_statitem_pool;
	static SDStatHandler* m_instance;

    std::map<int, std::string> m_stat_name_list;
};

#endif
