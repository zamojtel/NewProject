
template<typename T> class RealFFT
{
public:
	size_t get_signal_size() const
	{
		return m_cfft.get_signal_size();
	}


	void perform(const T *input, std::complex<T> *output)
	{
		for(size_t i = 0, e = get_signal_size(); i < e; ++i)
			m_cfft_input[i] = {input[i], T(0)};
		m_cfft.perform(m_cfft_input, m_cfft_output);
		memcpy(output, m_cfft_output, (get_signal_size()/2+1) * sizeof(std::complex<T>));
	}


	RealFFT()
	{
	}


	RealFFT(size_t a_signal_size)
	{
		set_signal_size(a_size);
	}


	void set_signal_size(size_t size)
	{
		m_cfft.set_signal_size(size);
		m_cfft_data.resize(size * 2);
		m_cfft_input = m_cfft_data.data();
		m_cfft_output = m_cfft_input + size;
	}


private:
	ComplexFFT<T> m_cfft;
	std::vector<std::complex<T>> m_cfft_data;
	std::complex<T> *m_cfft_input = nullptr;
	std::complex<T> *m_cfft_output = nullptr;
};

