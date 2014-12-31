#include "SDBuffer.h"

#include <string.h>

using namespace std;

SDBuffer::SDBuffer()
{
    m_data = NULL;
    m_len = 0;
    m_size = 0;
}

SDBuffer::SDBuffer(const char* buf, int size)
{
    m_data = new char[size];
	memcpy(m_data, buf, size);
    m_len = 0;
    m_size = size;
}

SDBuffer::~SDBuffer()
{
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
