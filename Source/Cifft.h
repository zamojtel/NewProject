
template<typename T> class Cifft
{
public:
	Cifft()
	{
	}


	Cifft(size_t a_signal_size)
	{
		set_signal_size(a_signal_size);
	}
	
	
	size_t get_signal_size() const
	{
		return m_cfft.get_signal_size();
	}
	

	void perform(const std::complex<T> *a_input, std::complex<T> *a_output)
	{
		size_t size = get_signal_size();
		std::complex<T> *reversed = m_reversed.data();
		for(size_t i = 0; i < size; ++i)
			reversed[i] = {a_input[i].imag(), a_input[i].real()};
		m_cfft.perform(reversed, a_output);
		for(size_t i = 0; i < size; ++i)
		{
			std::swap(reinterpret_cast<T(&)[2]>(a_output[i])[0], reinterpret_cast<T(&)[2]>(a_output[i])[1]);
			a_output[i] /= T(size);
		}
	}


	void set_signal_size(size_t a_size)
	{
		m_cfft.set_signal_size(a_size);
		m_reversed.resize(a_size);
	}



private:
	Cfft<T> m_cfft;
	std::vector<std::complex<T>> m_reversed;
};