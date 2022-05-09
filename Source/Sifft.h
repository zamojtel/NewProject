


template<typename T> class Sifft
{
public:
	bool get_denormalize_amplitudes() const
	{
		return(m_denormalize_amplitudes);
	}


	bool get_denormalize_phases() const
	{
		return(m_denormalize_phases);
	}


	bool get_phase_refers_to_sine() const
	{
		return(m_phase_refers_to_sine);
	}


	size_t get_signal_size() const
	{
		return m_rifft.get_signal_size();
	}


	void perform(const T *a_amplitudes, const T *a_phases, T *a_output)
	{
		Complex<T> *rifft_input = m_rifft_input.get_array();
		T amplitude_scale = (m_denormalize_amplitudes ? T(0.5)*T(m_rifft.get_signal_size()) : T(1));
		T phase_scale = (m_denormalize_phases ? T(SYNTHTOOLS_2PI) : T(1));
		if(!m_phase_refers_to_sine)
		{
			for(size_t i = 0, e = get_signal_size()/2+1; i < e; ++i)
			{
				T amplitude = amplitude_scale * a_amplitudes[i];
				T phase = phase_scale * a_phases[i];
				rifft_input[i] = {amplitude * cos(phase), amplitude * sin(phase)};
			}
		}
		else
		{
			for(size_t i = 0, e = get_signal_size()/2+1; i < e; ++i)
			{
				T amplitude = amplitude_scale * a_amplitudes[i];
				T phase = phase_scale * a_phases[i];
				rifft_input[i] = {amplitude * sin(phase), amplitude * -cos(phase)};
			}
		}
		m_rifft.perform(rifft_input, a_output);
	}


	/*void perform(const Harmonic<T> *a_input, T *a_output)
	{
		Complex<T> *rifft_input = m_rifft_input.get_array();
		T amplitude_scale = (m_denormalize_amplitudes ? T(0.5)*T(m_rifft.get_signal_size()) : T(1));
		T phase_scale = (m_denormalize_phases ? T(SYNTHTOOLS_2PI) : T(1));
		if(!m_phase_refers_to_sine)
		{
			for(size_t i = 0, e = get_signal_size()/2+1; i < e; ++i)
			{
				const Harmonic<T> &h = a_input[i];
				T amplitude = amplitude_scale * h.amplitude;
				T phase = phase_scale * h.phase;
				rifft_input[i].set(amplitude * cos(phase), amplitude * sin(phase));
			}
		}
		else
		{
			for(size_t i = 0, e = get_signal_size()/2+1; i < e; ++i)
			{
				const Harmonic<T> &h = a_input[i];
				T amplitude = amplitude_scale * h.amplitude;
				T phase = phase_scale * h.phase;
				rifft_input[i].set(amplitude * -sin(phase), amplitude * cos(phase));
			}
		}
		m_rifft.perform(rifft_input, a_output);
	}*/


	/*void perform(const Spectrum<T> *a_spectrum, T *a_output)
	{
		perform(a_spectrum->get_amplitudes(), a_spectrum->get_phases(), a_output);
	}*/


	void set_denormalize_amplitudes(bool a_value)
	{
		m_denormalize_amplitudes = a_value;
	}


	void set_denormalize_phases(bool a_value)
	{
		m_denormalize_phases = a_value;
	}


	void set_phase_refers_to_sine(bool a_value)
	{
		m_phase_refers_to_sine = a_value;
	}


	void set_signal_size(size_t a_size)
	{
		m_rifft.set_signal_size(a_size);
		m_rifft_input.resize(a_size);
	}

	Sifft()
	:
		m_denormalize_amplitudes(true),
		m_denormalize_phases(true),
		m_phase_refers_to_sine(false)
	{
	}


	Sifft(size_t a_signal_size)
	:
		m_denormalize_amplitudes{true},
		m_denormalize_phases{true},
		m_phase_refers_to_sine{false}
	{
		set_signal_size(a_signal_size);
	}


private:
	bool m_denormalize_amplitudes;
	bool m_denormalize_phases;
	bool m_phase_refers_to_sine;
	Rifft<T> m_rifft;
	Ace::Vector<Complex<T>> m_rifft_input;
};
