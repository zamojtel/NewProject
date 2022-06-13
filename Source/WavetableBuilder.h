

template<class T, class U>  class WavetableBuilder {
public:
	//table of frequencies
	//frequencies are sorted increasingly
	//thanks to this function we can play without aliasing 
	void build(const WaveformModel<T>* model, int count, const T* base_frequencies, size_t wavetable_size, Wavetable<U>* result, T sample_rate) {
		//Spectrum
		// <float> sfft(1024);
	//sfft.perform(waveform.data(), amplitudes.data(), phases.data());
		std::vector<T> model_amplitudes(model->get_size() / 2 + 1);
		std::vector<T> model_phases(model->get_size() / 2 + 1);
		SpectrumFFT<T> sfft(model->get_size());
		sfft.perform(model->get_samples(), model_amplitudes.data(), model_phases.data());
		model_amplitudes.resize(wavetable_size / 2 + 1, 0);
		model_phases.resize(wavetable_size / 2 + 1, 0);
		std::vector<T> signal(wavetable_size);
		SpectrumInverseFFT<T> sifft(wavetable_size);
		//frequencies.size() == count
		//analizujemy model 
		for (int i = 0; i < count; i++) {
			T f = base_frequencies[i];
			//maksymalna liczba skladowych jakie sie mieszcza 
			int n_harmonics = floor((sample_rate / 2) / f);
			//wszystkie powyzej tego usuwamy
			//oddzielne przypadki dla 100hz i np 600hz 
			//pozbywamy sie efektu aliasingu 
			for (int j = n_harmonics + 1; j < model_amplitudes.size(); j++) {
				model_amplitudes[j] = 0;
				model_phases[j] = 0;
			}
			//void perform(const T * amplitudes, const T * phases, T * output)
			sifft.perform(model_amplitudes.data(), model_phases.data(), signal.data());
			result[i].samples.resize(wavetable_size+1);
			for (int k = 0; k < wavetable_size; k++) {
				result[i].samples[k] = signal[k];
			}
			//powtarzamy pierwsza
			result[i].samples[wavetable_size] = result[i].samples[0];
		}
	}
};