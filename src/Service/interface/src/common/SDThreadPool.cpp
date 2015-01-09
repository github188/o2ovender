#include <exception>
#include "SDThreadPool.h"

using namespace std;

IMPL_LOGGER(SDThreadPool, logger)

SDThreadPool::SDThreadPool(int pool_size) : m_pool_size(pool_size)
{
	m_thread_ids = NULL;
	m_active_count = 0;
	pthread_mutex_init(&m_count_lock, NULL);
}

SDThreadPool::~SDThreadPool()
{
	if(m_thread_ids)
	{
		delete[] m_thread_ids;
		m_thread_ids = NULL;
	}
	pthread_mutex_destroy(&m_count_lock);
}

bool SDThreadPool::startThreads()
{
	if(m_pool_size > 0)
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr);

		//set stack size
		pthread_attr_setstacksize(&attr, STACK_SIZE);

		m_thread_ids = new pthread_t[m_pool_size];
		for(int i = 0; i < m_pool_size; i++)
		{
			if(pthread_create((m_thread_ids + i), &attr, threadProc, (void*)this))
			{
				return false;
			}
		}

		pthread_attr_destroy(&attr);
	}
	return true;
}

void SDThreadPool::waitThreadsTermination()
{
	if(m_thread_ids)
	{
		for(int i = 0; i < m_pool_size; i++)
		{
			pthread_join(m_thread_ids[i], NULL);
		}

		delete[] m_thread_ids;
		m_thread_ids = NULL;
	}
}

void* SDThreadPool::threadProc(void* para)
{
	SDThreadPool* me = (SDThreadPool*) para;
	me->doIt();
	return NULL;

}

