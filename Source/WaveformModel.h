#pragma once
//zadaniem bedzie trzymanie probek ktore beda fala 
#include <vector>

template<class T>  class WaveformModel {
public:
	std::vector<T> m_samples;
	
	WaveformModel(size_t size) {
		m_samples.resize(size);
		for (size_t i = 0; i < m_samples.size(); i++) {
			m_samples[i] = 0;
		}
	}

	void set_size(size_t size, bool set_to_zero) {
		m_samples.resize(size);
		if (set_to_zero == true) {
			for (size_t i = 0; i < m_samples.size(); i++) {
				m_samples[i] = 0;
			}
		}
	}

	size_t get_size() const {
		return m_samples.size();
	}

	T* get_samples() {
		//adres tablicy gdzie wektor trzyma elementy 
		return m_samples.data();
	}

	const T* get_samples() const{
		return m_samples.data();
	}

};