
#ifndef SPECTRUMINVERSEFFT_H
#define SPECTRUMINVERSEFFT_H
template<typename T> class SpectrumInverseFFT
{
public:

	size_t get_signal_size() const
	{
		return m_rifft.get_signal_size();
	}

	void perform(const T *amplitudes, const T *phases, T *output)
	{
		std::complex<T> *rifft_input = m_rifft_input.data();
		T amplitude_scale = T(0.5)*T(m_rifft.get_signal_size());

		for(size_t i = 0, e = get_signal_size()/2+1; i < e; ++i){
			T amplitude = amplitude_scale * amplitudes[i];
			T phase =phases[i];
			rifft_input[i] = {amplitude * cos(phase), amplitude * sin(phase)};
		}
		
		m_rifft.perform(rifft_input, output);
	}



	void set_signal_size(size_t size)
	{
		m_rifft.set_signal_size(size);
		m_rifft_input.resize(size);
	}

	SpectrumInverseFFT()
	{
	}


	SpectrumInverseFFT(size_t a_signal_size)
	{
		set_signal_size(a_signal_size);
	}


private:
	RealInverseFFT<T> m_rifft;
	std::vector<std::complex<T>> m_rifft_input;
};

#endif