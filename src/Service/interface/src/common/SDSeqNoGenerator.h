/********************************************************************
	created:		2005/08/24
	created:		24:8:2005   15:43
	filename: 		d:\work\newcvs\chub\impl\src\common\sdseqnogenerator.h
	file path:		d:\work\newcvs\chub\impl\src\common
	file base:		sdseqnogenerator
	file ext:		h
	author:			lifeng
	description:	sequence number generator
*********************************************************************/

#ifndef SANDAI_C_SEQUENCENUMBERGENERATOR_H_200508241543
#define SANDAI_C_SEQUENCENUMBERGENERATOR_H_200508241543

#include <pthread.h>

class SDSeqNoGenerator
{
public:
	static SDSeqNoGenerator* getInstance();
	static void close();
	unsigned int getSequenceNo();
	~SDSeqNoGenerator();

protected:
private:
	SDSeqNoGenerator();
	static SDSeqNoGenerator* m_instance;
	unsigned int m_count;
	pthread_mutex_t m_lock;
	void increase();
};

class SDSeqNoGenerator2
{
public:
	static SDSeqNoGenerator2* getInstance();
	static void close();
	unsigned int getSequenceNo();
	~SDSeqNoGenerator2();

protected:
private:
	SDSeqNoGenerator2();
	static SDSeqNoGenerator2* m_instance;
	unsigned int m_count;
	pthread_mutex_t m_lock;
	void increase();
};

#endif

