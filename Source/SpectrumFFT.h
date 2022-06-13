
#ifndef SPECTRUMFFT_H
#define SPECTRUMFFT_H

template<typename T> class SpectrumFFT
{
public:

	size_t get_signal_size() const
	{
		return m_rfft.get_signal_size();
	}

	// spectrum wyznacza amplitudy i przesuniêcia fazowe sk³adowych
	void perform(const T *input, T *amplitudes, T *phases)
	{
		std::complex<T>* rfft_output = m_rfft_output.data();
		m_rfft.perform(input, rfft_output);
		T dc_scale = T(1) / T(m_rfft.get_signal_size());
		//2 stad ze sygnal symetryczny
		T amplitude_scale = T(2) / T(m_rfft.get_signal_size()); //dzielimy przez rozmiar sygnalu 
	 

		//skladowa stala
		amplitudes[0] = std::abs(rfft_output[0]) * dc_scale;
		for(size_t i = 1, e = get_signal_size()/2+1; i < e; ++i)
			amplitudes[i] = std::abs(rfft_output[i]) * amplitude_scale;

		for (size_t i = 0, e = get_signal_size() / 2 + 1; i < e; ++i)
			phases[i] = std::arg(rfft_output[i]);
		
	}


	void set_signal_size(size_t size)
	{
		m_rfft.set_signal_size(size);
		m_rfft_output.resize(size);
	}


	SpectrumFFT(size_t signal_size)
	{
		set_signal_size(signal_size);
	}


private:
	RealFFT<T> m_rfft;
	std::vector<std::complex<T>> m_rfft_output;
};

#endif