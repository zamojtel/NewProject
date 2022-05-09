
template<typename T> class Rfft
{
public:
	size_t get_signal_size() const
	{
		return m_cfft.get_signal_size();
	}


	void perform(const T *a_input, Complex<T> *a_output)
	{
		for(size_t i = 0, e = get_signal_size(); i < e; ++i)
			m_cfft_input[i] = {a_input[i], T(0)};
		m_cfft.perform(m_cfft_input, m_cfft_output);
		memcpy(a_output, m_cfft_output, (get_signal_size()/2+1) * sizeof(Complex<T>));
	}


	Rfft()
	{
	}


	Rfft(size_t a_signal_size)
	{
		set_signal_size(a_size);
	}


	void set_signal_size(size_t a_size)
	{
		m_cfft.set_signal_size(a_size);
		m_cfft_data.resize(a_size * 2);
		m_cfft_input = m_cfft_data.data();
		m_cfft_output = m_cfft_input + a_size;
	}


private:
	Cfft<T> m_cfft;
	std::vector<std::complex<T>> m_cfft_data;
	std::complex<T> *m_cfft_input = nullptr;
	std::complex<T> *m_cfft_output = nullptr;
};

