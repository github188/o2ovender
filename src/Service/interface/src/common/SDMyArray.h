#ifndef SANDAI_CPP_MYARRAY_H_200610261426
#define SANDAI_CPP_MYARRAY_H_200610261426

template <unsigned int stack_size>
class MyArray
{
public:
	MyArray()
	{
		m_stack_length = 0;
		m_max_stack_size = stack_size;
	}
	~MyArray()
	{
	}

	void set(char* buffer, unsigned int buffer_length)
	{
		if (buffer_length <= m_max_stack_size )		
			m_stack_length = buffer_length;
		else
			m_stack_length = m_max_stack_size;
		
		memcpy(m_stack, buffer, m_stack_length);
	}	
	char operator[](int index) const
	{
		return m_stack[index];
	}	
	std::string to_string() const 
	{
		return std::string(m_stack, m_stack_length);
	}	
	const char* c_str() const
	{
		return m_stack;		
	}
	const unsigned int length() const
	{
		return m_stack_length;
	}

	const unsigned size() const
	{
		return m_max_stack_size;
	}

private:
	unsigned int m_max_stack_size;
	unsigned int m_stack_length;
	char m_stack[stack_size];
};

#endif // SANDAI_CPP_MYARRAY_H_200610261426

