#ifndef SD_BUFFER_H
#define SD_BUFFER_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "SDQueue.h"

class SDBuffer;
typedef boost::shared_ptr<SDBuffer> SDSharedBuffer;
typedef SDQueue<SDSharedBuffer> SDBufferQueue;

class SDBuffer
{
public:
    SDBuffer();
    SDBuffer(const char* buf, int size);
    ~SDBuffer();
    
    void alloc_buf(int size);
    void free_buf();

    char* data()          {return m_data;}
    int length()          {return m_len;}
    int size()            {return m_size;}
    int offset()          {return m_size-m_len;}
    void lseek(int offset) {if(offset) {m_len += offset;} else {m_len=0;}}

    char* m_data;
    int m_len;
    int m_size;

    std::string m_ipv4;
};

#endif
