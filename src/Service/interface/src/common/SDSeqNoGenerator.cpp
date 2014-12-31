#include "SDSeqNoGenerator.h"

SDSeqNoGenerator* SDSeqNoGenerator::m_instance = new SDSeqNoGenerator();

SDSeqNoGenerator::SDSeqNoGenerator()
{
	m_count = 0;
	pthread_mutex_init(&m_lock, NULL);
}

SDSeqNoGenerator::~SDSeqNoGenerator()
{
	pthread_mutex_destroy(&m_lock);
}

SDSeqNoGenerator* SDSeqNoGenerator::getInstance()
{
	return m_instance;
}

void SDSeqNoGenerator::close()
{
	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

unsigned int SDSeqNoGenerator::getSequenceNo()
{
	unsigned int ret_no = 0;
	pthread_mutex_lock(&m_lock);
	ret_no = m_count++;
	pthread_mutex_unlock(&m_lock);
	return ret_no;
}

///////////////////////////////////////////////////////////////////////

SDSeqNoGenerator2* SDSeqNoGenerator2::m_instance = new SDSeqNoGenerator2();

SDSeqNoGenerator2::SDSeqNoGenerator2()
{
	m_count = 0;
	pthread_mutex_init(&m_lock, NULL);
}

SDSeqNoGenerator2::~SDSeqNoGenerator2()
{
	pthread_mutex_destroy(&m_lock);
}

SDSeqNoGenerator2* SDSeqNoGenerator2::getInstance()
{
	return m_instance;
}

void SDSeqNoGenerator2::close()
{
	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

unsigned int SDSeqNoGenerator2::getSequenceNo()
{
	unsigned int ret_no = 0;
	pthread_mutex_lock(&m_lock);
	ret_no = m_count++;
	pthread_mutex_unlock(&m_lock);
	return ret_no;
}


