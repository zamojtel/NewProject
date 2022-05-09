


template<typename T> class Sfft
{
public:
	bool get_normalize_amplitudes() const
	{
		return(m_normalize_amplitudes);
	}


	bool get_normalize_phases() const
	{
		return(m_normalize_phases);
	}


	bool get_phase_refers_to_sine() const
	{
		return(m_phase_refers_to_sine);
	}


	size_t get_signal_size() const
	{
		return m_rfft.get_signal_size();
	}


	//void perform(const T *a_input, Harmonic<T> *a_output)
	//{
	//	Complex<T> *rfft_output = m_rfft_output.get_array();
	//	m_rfft.perform(a_input, rfft_output);
	//	T dc_scale = (m_normalize_amplitudes ? T(1)/T(m_rfft.get_size()) : T(1));
	//	T amplitude_scale = (m_normalize_amplitudes ? T(2)/T(m_rfft.get_size()) : T(1));
	//	T phase_scale = (m_normalize_phases ? T(1)/T(SYNTHTOOLS_2PI) : T(1));
	//	if(!m_phase_refers_to_sine)
	//	{
	//		a_output[0].set(rfft_output[0].get_magnitude() * dc_scale, rfft_output[0].get_phase() * phase_scale);
	//		for(size_t i = 1, e = get_signal_size()/2+1; i < e; ++i)
	//			a_output[i].set(rfft_output[i].get_magnitude() * amplitude_scale, rfft_output[i].get_phase() * phase_scale);
	//	}
	//	else
	//	{
	//		T dc_amplitude = rfft_output[0].get_magnitude() * dc_scale;
	//		T dc_phase = (rfft_output[0].x != T(0) || rfft_output[0].y != T(0) ? atan2(rfft_output[0].x, -rfft_output[0].y) : T(0));
	//		a_output[0].set(dc_amplitude, dc_phase * phase_scale);
	//		for(size_t i = 1, e = get_signal_size()/2+1; i < e; ++i)
	//		{
	//			T amplitude = rfft_output[i].get_magnitude() * amplitude_scale;
	//			T phase = (rfft_output[i].x != T(0) || rfft_output[i].y != T(0) ? atan2(rfft_output[i].x, -rfft_output[i].y) : T(0));
	//			a_output[i].set(amplitude, phase * phase_scale);
	//		}
	//	}
	//}


	void perform(const T *a_input, T *a_amplitudes, T *a_phases)
	{
		Complex<T> *rfft_output = m_rfft_output.get_array();
		m_rfft.perform(a_input, rfft_output);
		T dc_scale = (m_normalize_amplitudes ? T(1)/T(m_rfft.get_signal_size()) : T(1));
		T amplitude_scale = (m_normalize_amplitudes ? T(2)/T(m_rfft.get_signal_size()) : T(1));
		T phase_scale = (m_normalize_phases ? T(1)/T(SYNTHTOOLS_2PI) : T(1));

		a_amplitudes[0] = rfft_output[0].abs() * dc_scale;
		for(size_t i = 1, e = get_signal_size()/2+1; i < e; ++i)
			a_amplitudes[i] = rfft_output[i].abs() * amplitude_scale;

		if(!m_phase_refers_to_sine)
		{
			for(size_t i = 0, e = get_signal_size()/2+1; i < e; ++i)
				a_phases[i] = rfft_output[i].arg() * phase_scale;
		}
		else
		{
			for(uint i = 0, e = get_signal_size()/2+1; i < e; ++i)
			{
				T phase = (rfft_output[i].r != T(0) || rfft_output[i].i != T(0) ? atan2(rfft_output[i].r, -rfft_output[i].i) : T(0));
				a_phases[i] = phase * phase_scale;
			}
		}
	}


	//void perform(const T *a_input, Spectrum<T> *a_output)
	//{
	//	a_output->set_signal_size(get_signal_size());
	//	perform(a_input, a_output->get_amplitudes(), a_output->get_phases());
	//}


	void set_normalize_amplitudes(bool a_value)
	{
		m_normalize_amplitudes = a_value;
	}

	
	void set_normalize_phases(bool a_value)
	{
		m_normalize_phases = a_value;
	}


	void set_phase_refers_to_sine(bool a_value)
	{
		m_phase_refers_to_sine = a_value;
	}


	void set_signal_size(size_t a_size)
	{
		m_rfft.set_signal_size(a_size);
		m_rfft_output.resize(a_size);
	}


	Sfft()
	:
		m_normalize_amplitudes{true},
		m_normalize_phases{true},
		m_phase_refers_to_sine{false}
	{
	}


	Sfft(size_t a_signal_size)
	:
		m_normalize_amplitudes{true},
		m_normalize_phases{true},
		m_phase_refers_to_sine{false}
	{
		set_signal_size(a_signal_size);
	}


private:
	bool m_normalize_amplitudes;
	bool m_normalize_phases;
	bool m_phase_refers_to_sine;
	Rfft<T> m_rfft;
	Ace::Vector<Complex<T>> m_rfft_output;
};
