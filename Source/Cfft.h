
template<typename T> class Cfft
{
public:
	Cfft()
	:
		m_signal_size{0}
	{
	}


	Cfft(size_t a_signal_size)
	:
		m_signal_size{0}
	{
		set_signal_size(a_size);
	}


	size_t get_signal_size() const
	{
		return m_signal_size;
	}


	void perform(const std::complex<T> *a_input, std::complex<T> *a_output)
	{
		p_perform(m_signal_size, a_input, a_output, 1);
	}


	void set_signal_size(size_t a_value)
	{
		if(a_value != m_signal_size)
		{
			m_signal_size = a_value;
			m_twiddle_factors_vector.resize(m_signal_size);
			m_twiddle_factors = m_twiddle_factors_vector.data();
			for(uint i = 0; i < m_signal_size/2; ++i)
			{
				double phase = double(i)/double(m_signal_size);
				m_twiddle_factors[i] = {T(cos(double(SYNTHTOOLS_2PI) * phase)), T(-sin(double(SYNTHTOOLS_2PI) * phase))};
			}
		}
	}


private:
	size_t m_signal_size;
	std::complex<T> *m_twiddle_factors;
	std::vector<std::complex<T>> m_twiddle_factors_vector;


	void p_perform(size_t a_signal_size, const std::complex<T> *a_input, std::complex<T> *a_output, size_t a_step)
	{
		if(a_signal_size == 1)
			a_output[0] = a_input[0];
		else
		{
			p_perform(a_signal_size/2, a_input, a_output, 2*a_step);
			p_perform(a_signal_size/2, a_input + a_step, a_output + a_signal_size/2, 2*a_step);
			for(size_t k = 0; k < a_signal_size/2; ++k)
			{
				std::complex<T> t = a_output[k];
				std::complex<T> f = m_twiddle_factors[k*a_step];
				a_output[k] = t + f * a_output[k + a_signal_size/2];
				a_output[k + a_signal_size/2] = t - f * a_output[k + a_signal_size/2];
			}
		}
	}
};

