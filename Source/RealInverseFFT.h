

template<typename T> class RealInverseFFT
{
public:
	size_t get_signal_size() const
	{
		return m_cifft.get_signal_size();
	}


	void perform(const std::complex<T> *input, T *output)
	{
		size_t size = get_signal_size();
		memcpy(m_cifft_input, input, (size/2+1) * sizeof(std::complex<T>));
		for(size_t i = size/2+1; i < size; ++i)
			m_cifft_input[i] = std::conj(m_cifft_input[size-i]); //{m_cifft_input[size-i].real(), -m_cifft_input[size-i].imag()};
		m_cifft.perform(m_cifft_input, m_cifft_output);
		for(size_t i = 0; i < size; ++i)
			output[i] = m_cifft_output[i].real();
	}


	void perform(const T *magnitudes, const T *phases, T *output)
	{
		size_t size = get_signal_size();
		size_t spectrum_size = size/2+1;
		for(size_t i = 0; i < spectrum_size; ++i)
			m_cifft_input[i] = {magnitudes[i] * cos(phases[i]), magnitudes[i] * sin(phases[i])};
		for(size_t i = spectrum_size; i < size; ++i)
			m_cifft_input[i] = std::conj(m_cifft_input[size-i]); //{m_cifft_input[size-i].r, -m_cifft_input[size-i].i};
		m_cifft.perform(m_cifft_input, m_cifft_output);
		for(size_t i = 0; i < size; ++i)
			output[i] = m_cifft_output[i].real();
	}


	RealInverseFFT()
	{
	}


	RealInverseFFT(size_t signal_size)
	{
		set_signal_size(signal_size);
	}


	void set_signal_size(size_t size)
	{
		m_cifft.set_signal_size(size);
		m_cifft_data.resize(size * 2);
		m_cifft_input = m_cifft_data.data();
		m_cifft_output = m_cifft_input + size;
	}


private:
	ComplexInverseFFT<T> m_cifft;
	std::vector<std::complex<T>> m_cifft_data;
	std::complex<T> *m_cifft_input = nullptr;
	std::complex<T> *m_cifft_output = nullptr;
};


