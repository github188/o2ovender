/********************************************************************
	created:		2005/08/15
	created:		15:8:2005   12:32
	filename: 		d:\work\newcvs\chub\impl\src\common\sdqueue.h
	file path:		d:\work\newcvs\chub\impl\src\common
	file base:		sdqueue
	file ext:		h
	author:			lifeng
	description:	this is a fixed size queue; class T had better be a pointer
*********************************************************************/

#ifndef SANDAI_C_QUEUE_H_200508151110
#define SANDAI_C_QUEUE_H_200508151110

#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <common/SDException.h>

class SDIQueueInfo
{
public:
	virtual ~SDIQueueInfo() { };

	virtual int getCapacity() = 0;
	virtual int getSize() = 0;
	virtual int getMaxSize(bool resetMax = true) = 0;
};

template <class T> class SDQueue : public SDIQueueInfo{

	T*  m_buffer;
    T m_null_object;

	int m_capacity;
	int m_start;
	int m_size;
	int m_max_size;

	pthread_mutex_t m_lock;
	pthread_cond_t	m_cond_not_full;
	pthread_cond_t	m_cond_not_empty;

public:

	SDQueue(int capacity)
	{
		m_buffer = NULL;
		m_start = 0;
		m_size = 0;
		m_max_size = 0;
		m_capacity = 0;

		pthread_mutex_init(&m_lock, NULL);
		if(pthread_cond_init(&m_cond_not_full, NULL))
			throw SDInitException("Init thread_cond_full error: "+ errno);
		if(pthread_cond_init(&m_cond_not_empty, NULL))
			throw SDInitException("Init thread_cond_empty error: "+ errno);

        init(capacity);
	}

	virtual ~SDQueue()
	{
		pthread_mutex_destroy(&m_lock);
		pthread_cond_destroy(&m_cond_not_full);
		pthread_cond_destroy(&m_cond_not_empty);

        uninit();
	}

	void init(int capacity)
	{
		try
		{
			m_buffer = allocBuffer(capacity);
			m_capacity = capacity;
		}
		catch(std::exception&){}
	}

	void uninit()
	{
		freeBuffer(m_buffer);
	}

	bool pop(T& retVal, int waitMs)
	{
		if(m_buffer == NULL)
		{
			return false;
		}

		bool ret_val = false;
		pthread_mutex_lock(&m_lock);

		try
		{
			if(m_size == 0)
			{//empty
				if(waitMs == 0)
				{//wait infinitely
					while(m_size == 0)
						pthread_cond_wait(&m_cond_not_empty, &m_lock);
				}
				else
				{//wait for a given timeout
					struct timespec timespot;

					if(waitMs > 0)
					{
						getAbsTimeout(waitMs, timespot);
					}
					else
					{//need not call gettimeofday
						timespot.tv_sec = 0;
						timespot.tv_nsec = 0;
					}
					pthread_cond_timedwait(&m_cond_not_empty, &m_lock, &timespot);
				}
			}

			if(m_size > 0)
			{
				retVal = m_buffer[m_start];
                m_buffer[m_start] = m_null_object;  /* free the shared object */
				ret_val = true;

				if(++m_start >= m_capacity)
					m_start -= m_capacity;

				m_size --;
				pthread_cond_signal(&m_cond_not_full);
			}
		}
		catch (std::exception& ) {
			//exception
			ret_val = false;
		}

		pthread_mutex_unlock(&m_lock);
		return ret_val;
	}

	bool push(const T& element, int waitMs)
	{
		if(m_buffer == NULL)
		{
			return false;
		}

		bool retVal = false;
		pthread_mutex_lock(&m_lock);

		try	
		{
			if(m_size >= m_capacity)
			{//full
				if(waitMs == 0)
				{//wait infinitely
					while(m_size >= m_capacity)
						pthread_cond_wait(&m_cond_not_full, &m_lock);
				}
				else
				{//wait for a given timeout
					struct timespec timespot;

					if(waitMs > 0)
					{
						getAbsTimeout(waitMs, timespot);
					}
					else
					{//need not call gettimeofday
						timespot.tv_sec = 0;
						timespot.tv_nsec = 0;
					}
					pthread_cond_timedwait(&m_cond_not_full, &m_lock, &timespot);
				}
			}

			if(m_size < m_capacity)
			{
				int newpos = m_start + m_size;
				if(newpos >= m_capacity)
					newpos -= m_capacity;

				m_buffer[newpos] = element;
				m_size ++;
				if(m_max_size < m_size)
					m_max_size = m_size;
				pthread_cond_signal(&m_cond_not_empty);
				retVal = true;
			}
		}
		catch (std::exception&) {
			//exception
			retVal = false;
		}

		pthread_mutex_unlock(&m_lock);
		return retVal;
	}

	bool pop(T& retVal)
	{
		return pop(retVal, 0);
	}
	bool pop_nonblock(T& retVal)
	{
		return pop(retVal, -1);
	}

	bool push(T element)
	{
		return push(element, 0);
	}
	bool push_nonblock(T element)
	{
		return push(element, -1);
	}

	int getCapacity()
	{
		return m_capacity;
	}

	int getSize()
	{
		return m_size;
	}

	int getMaxSize(bool resetMax = true)
	{
		int maxSize = m_max_size;

		if(resetMax)
		{
			pthread_mutex_lock(&m_lock);	
			m_max_size = m_size;
			pthread_mutex_unlock(&m_lock);		
		}

		return maxSize;		
	}

protected:
	virtual T* allocBuffer(int capacity)
    {
        return new T[capacity];
    }
	virtual void freeBuffer(T* pBuffer)
    {
        delete[] pBuffer;
    }

private:
	void getAbsTimeout(int timeout, struct timespec& abstimeout)
	{
		struct timeval now;
		gettimeofday(&now, NULL);
		timeout += (now.tv_usec / 1000);

		abstimeout.tv_sec = now.tv_sec + timeout / 1000;
		abstimeout.tv_nsec = (timeout % 1000) * 1000 * 1000;
	}

};

#endif

