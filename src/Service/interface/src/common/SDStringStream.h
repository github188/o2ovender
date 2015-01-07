#ifndef SD_STRING_STREAM_H_20071124
#define SD_STRING_STREAM_H_20071124

#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class SDStringStream;

class SDFakeString
{
private:
    const char* m_buffer;
    unsigned int m_buffer_size;

public:
    SDFakeString(const char* buffer, unsigned int buffer_size) : m_buffer(buffer), m_buffer_size(buffer_size) {}    
    friend class SDStringStream;
};

class SDStringStream
{
private:
	char* m_buffer;
	unsigned int m_buffer_size;
	unsigned int m_buffer_offset;

	bool m_need_delete;
	bool m_opt_status;
	bool m_is_little_endian;

public:
	static const bool DELETE = true;
	static const bool NODELETE = false;
	static const bool LITTLEENDIAN = true;
	static const bool BIGENDIAN = false;

public:
	SDStringStream(const char* user_buffer, unsigned int user_buffer_length, 
		bool need_delete=NODELETE, bool is_little_endian=LITTLEENDIAN)
	{
		m_buffer = (char*)user_buffer;
		m_buffer_size = user_buffer_length;
		m_buffer_offset = 0;

		m_need_delete = need_delete;
		m_opt_status = false;
		//m_is_little_endian = is_little_endian;

		int tmp = 1;
		bool little_endian = (*((char*)&tmp) == 1);

		if (little_endian) {
			if (is_little_endian == LITTLEENDIAN) {
				m_is_little_endian = LITTLEENDIAN;
			}
			else {
				m_is_little_endian = BIGENDIAN;
			}
		}
		else {
			if (is_little_endian == LITTLEENDIAN) {
				m_is_little_endian = BIGENDIAN;
			}
			else {
				m_is_little_endian = LITTLEENDIAN;
			}
		}
	}
	~SDStringStream()
	{
		if (m_need_delete) {
			delete[] m_buffer;
			m_buffer = 0;
		}
	}

	 bool operator! ()
	 {
	 	return m_opt_status;
	 }

	  unsigned int length()
	  {
		return m_buffer_offset;
	  }
	  const char* c_str()
	  {
		return m_buffer;
	  }
	  unsigned int max_size()
	  {
		return m_buffer_size;
	  }
	  
	  bool lseek(unsigned int offset)
	  {
	  	if (offset >= max_size()) {
			return false;
		}

  		m_buffer_offset = offset;
		return true;
	  }
	
	  unsigned int left_size()
	  {
	  	return (max_size() - length());
	  }

	  template<typename T>
	  SDStringStream& operator<< (const T& arg)
	  {
	  	if (this->operator!()) {
			return *this;
	  	}

		if (left_size() < sizeof(T)) {
			m_opt_status = true;
		}
		else {
			//m_opt_status = false;
			if (m_is_little_endian) {
				memcpy(m_buffer+m_buffer_offset, &arg, sizeof(T));
			}
			else {
				const char* ptr_from = (const char*)&arg + sizeof(T) - 1;
				char* ptr_to = m_buffer + m_buffer_offset;
				for (unsigned int i=0; i<sizeof(T); i++) {
					*ptr_to++ = *ptr_from--;
				}     
			}
			m_buffer_offset += sizeof(T);
		}
		return *this;
	  }
	  template<typename T>
	  SDStringStream& operator>> (T& arg)
	  {
	  	if (this->operator!()) {
			return *this;
	  	}

		if (left_size() < sizeof(T)) {
			m_opt_status = true;
		}
		else {
			//m_opt_status = false;
			if (m_is_little_endian) {
				memcpy(&arg, m_buffer+m_buffer_offset, sizeof(T));
			}
			else {
				const char* ptr_from = m_buffer + m_buffer_offset + sizeof(T) - 1;
				char* ptr_to = (char*)&arg;
				for (unsigned int i=0; i<sizeof(T); i++) {
					*ptr_to++ = *ptr_from--;
				} 
			}
			m_buffer_offset += sizeof(T);
		}

		return *this; 
	  }

	  SDStringStream& operator<< (const std::string& arg)
	  {
	  	if (this->operator!()) {
			return *this;
	  	}

		unsigned int str_length = arg.length();
		const char* str_buffer = arg.c_str();
		if (left_size() < sizeof(str_length) + str_length) {
			m_opt_status = true;
		}
		else {
			//m_opt_status = false;
			if (!!(this->operator<<(str_length))) {
				memcpy(m_buffer+m_buffer_offset, str_buffer, str_length);
				m_buffer_offset += str_length;
			}
		}

		return *this;
	  }
	  SDStringStream& operator>> (std::string& arg)
	  {
	  	if (this->operator!()) {
			return *this;
	  	}

		unsigned int str_length = 0;
		if (!!(this->operator>>(str_length))) {
			if (left_size() < str_length) {
				m_opt_status = true;
			}
			else {
				//m_opt_status = false;
				if (str_length > 0) {
					arg.resize(str_length);
					char* str_buffer = (char*)arg.c_str();
					memcpy(str_buffer, m_buffer+m_buffer_offset, str_length);
					m_buffer_offset += str_length;
				}
				else {
					arg.erase();
				}
			}
		}

		return *this;
	  }

      SDStringStream& operator<< (const SDFakeString& arg)
      {
          if (this->operator!()) {
              return *this;
          }

          unsigned int str_length = arg.m_buffer_size;
          const char* str_buffer = arg.m_buffer;
          if (left_size() < str_length) {
              m_opt_status = true;
          }
          else {
              //m_opt_status = false;         
              memcpy(m_buffer+m_buffer_offset, str_buffer, str_length);
              m_buffer_offset += str_length;
          }

          return *this;
      }
	  SDStringStream& operator>> (const SDFakeString& arg)
	  {
	  	if (this->operator!()) {
			return *this;
	  	}
        
        unsigned int str_length = arg.m_buffer_size;
        const char* str_buffer = arg.m_buffer;
        if (left_size() < str_length) {
            m_opt_status = true;
        }
        else {
            //m_opt_status = false;         
            memcpy(str_buffer, m_buffer+m_buffer_offset, str_length);
            m_buffer_offset += str_length;
        }

		return *this;
	  }
};

#endif // SD_STRING_STREAM_H_20071124

