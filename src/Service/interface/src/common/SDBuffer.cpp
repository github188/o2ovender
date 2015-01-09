#include "SDBuffer.h"

#include <string.h>

using namespace std;

IMPL_LOGGER(SDBuffer,logger);

SDBuffer::SDBuffer()
{
    LOG4CPLUS_TRACE(logger, "SDBuffer>>>");
    m_data = NULL;
    m_len = 0;
    m_size = 0;
}

SDBuffer::SDBuffer(const char* buf, int size)
{
    LOG4CPLUS_TRACE(logger, "SDBuffer>>>");
    m_data = new char[size];
	memcpy(m_data, buf, size);
    m_len = 0;
    m_size = size;
}

SDBuffer::SDBuffer(int size)
{
    LOG4CPLUS_TRACE(logger, "SDBuffer>>>");
    m_data = new char[size];
	memset(m_data, 0, size);
    m_len = 0;
    m_size = size;
}

SDBuffer::~SDBuffer()
{
    LOG4CPLUS_TRACE(logger, "SDBuffer<<<");
    free_buf();
}
    
void SDBuffer::alloc_buf(int size)
{
    free_buf();

    m_data = new char[size];
    m_size = size;
    m_len = 0;
}

void SDBuffer::free_buf()
{
    if (m_data) {
        delete[] m_data;
    }
    m_data = NULL;
    m_len = 0;
    m_size = 0;
}
