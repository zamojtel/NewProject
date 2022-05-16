

template<typename T> class ComplexFFT
{
public:
	ComplexFFT()
	:
		m_signal_size{0}
	{
	}


	ComplexFFT(size_t signal_size)
	:
		m_signal_size{0}
	{
		set_signal_size(size);
	}


	size_t get_signal_size() const
	{
		return m_signal_size;
	}


	void perform(const std::complex<T> *input, std::complex<T> *output)
	{
		p_perform(m_signal_size, input, output, 1);
	}

	void set_signal_size(size_t value)
	{
		if(value != m_signal_size)
		{
			m_signal_size = value;
			m_twiddle_factors_vector.resize(m_signal_size);
			m_twiddle_factors = m_twiddle_factors_vector.data();
			for(unsigned int i = 0; i < m_signal_size/2; ++i)
			{
				double phase = double(i)/double(m_signal_size);
				m_twiddle_factors[i] = {T(cos(juce::MathConstants<float>::twoPi * phase)), T(-sin(juce::MathConstants<float>::twoPi * phase))};
			}
		}
	}


private:
	size_t m_signal_size;
	std::complex<T> *m_twiddle_factors;
	std::vector<std::complex<T>> m_twiddle_factors_vector;


	void p_perform(size_t signal_size, const std::complex<T> *input, std::complex<T> *output, size_t step)
	{
		if(signal_size == 1)
			output[0] = input[0];
		else
		{
			p_perform(signal_size/2, input, output, 2*step);
			p_perform(signal_size/2, input + step, output + signal_size/2, 2*step);
			for(size_t k = 0; k < signal_size/2; ++k)
			{
				std::complex<T> t = output[k];
				std::complex<T> f = m_twiddle_factors[k*step];
				output[k] = t + f * output[k + signal_size/2];
				output[k + signal_size/2] = t - f * output[k + signal_size/2];
			}
		}
	}
};

