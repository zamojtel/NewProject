
template<typename T> class Rifft
{
public:
	size_t get_signal_size() const
	{
		return m_cifft.get_signal_size();
	}


	void perform(const Complex<T> *a_input, T *a_output)
	{
		size_t size = get_signal_size();
		memcpy(m_cifft_input, a_input, (size/2+1) * sizeof(Complex<T>));
		for(size_t i = size/2+1; i < size; ++i)
			m_cifft_input[i] = {m_cifft_input[size-i].r, -m_cifft_input[size-i].i};
		m_cifft.perform(m_cifft_input, m_cifft_output);
		for(size_t i = 0; i < size; ++i)
			a_output[i] = m_cifft_output[i].r;
	}


	void perform(const T *a_magnitudes, const T *a_phases, T *a_output)
	{
		size_t size = get_signal_size();
		size_t spectrum_size = size/2+1;
		for(size_t i = 0; i < spectrum_size; ++i)
			m_cifft_input[i] = {a_magnitudes[i] * cos(a_phases[i]), a_magnitudes[i] * sin(a_phases[i])};
		for(size_t i = spectrum_size; i < size; ++i)
			m_cifft_input[i] = {m_cifft_input[size-i].r, -m_cifft_input[size-i].i};
		m_cifft.perform(m_cifft_input, m_cifft_output);
		for(size_t i = 0; i < size; ++i)
			a_output[i] = m_cifft_output[i].r;
	}


	Rifft()
	{
	}


	Rifft(size_t a_signal_size)
	{
		set_signal_size(a_signal_size);
	}


	void set_signal_size(size_t a_size)
	{
		m_cifft.set_signal_size(a_size);
		m_cifft_data.resize(a_size * 2);
		m_cifft_input = m_cifft_data.get_array();
		m_cifft_output = m_cifft_input + a_size;
	}


private:
	Cifft<T> m_cifft;
	Ace::Vector<Complex<T>> m_cifft_data;
	Complex<T> *m_cifft_input = nullptr;
	Complex<T> *m_cifft_output = nullptr;
};

