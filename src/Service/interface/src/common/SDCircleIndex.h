#ifndef SD_CIRCLE_INDEX_H
#define SD_CIRCLE_INDEX_H

class SDCircleIndex
{
public:
    SDCircleIndex(uint32_t size=1) : m_index(0),m_size(size)
    {}
    void init(uint32_t size)
    {
         m_index = 0;
         m_size = size;
    }

    uint32_t get_index()
    {
        uint32_t index = m_index;
        ++m_index;
        if (m_index >= m_size) {
            m_index = 0;
        }

        return index;
    }

private:
    uint32_t m_index;
    uint32_t m_size;
};

#endif
