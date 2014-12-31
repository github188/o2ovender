/********************************************************************
	created:		2005/08/21
	created:		21:8:2005   17:41
	filename: 		d:\work\newcvs\chub\impl\src\common\sdthreadpool.h
	file path:		d:\work\newcvs\chub\impl\src\common
	file base:		sdthreadpool
	file ext:		h
	author:			lifeng
	description:	a simple thread pool
*********************************************************************/

#ifndef SANDAI_C_THREADPOOL_H_200508211741
#define SANDAI_C_THREADPOOL_H_200508211741

#include <pthread.h>
#include <common/SDLogger.h>

class SDThreadPool
{
	const static int STACK_SIZE = (256 * 1024);

public:
	SDThreadPool(int pool_size);
	virtual ~SDThreadPool();

	bool startThreads();
	void waitThreadsTermination();

	inline int get_pool_size()
	{
		return m_pool_size;
	}

	inline int get_active_thread_count()
	{
		return m_active_count;
	}

protected:
	virtual void doIt() = 0;

	inline void setActive()
	{
		pthread_mutex_lock(&m_count_lock);
		m_active_count++;
		pthread_mutex_unlock(&m_count_lock);
	}

	inline void setInactive()
	{
		pthread_mutex_lock(&m_count_lock);
		m_active_count--;
		pthread_mutex_unlock(&m_count_lock);
	}

protected:
	DECL_LOGGER(logger);

	static void* threadProc(void* para);

	int m_pool_size;
	pthread_t *m_thread_ids;
	pthread_mutex_t m_count_lock;

	int m_active_count;
};

#endif

