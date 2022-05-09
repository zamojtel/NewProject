#pragma once
#include <cmath>
#include <JuceHeader.h>

/// <summary>
/// aktualnie jest to generator fali o niskiej czestotliwosci
/// </summary>
/// <typeparam name="T"></typeparam>
template<class T>  class LowFrequencyOscillator {
public:
	//current function
	float (*m_func) (float);
	Waveform m_waveform;
	T m_sample_rate;
	T m_hertz;
	//faza idziemy po sinusie 
	T m_phase;

	LowFrequencyOscillator() {
		setWaveform(Waveform::SINE);
	}

	void start() {
		this->m_phase = 0;
		//setHertz(2);
		//set_sample_rate(44100);
	}
	void set_hertz(T value) {
		this->m_hertz = value;
	}

	void set_sample_rate(T value) {
		this->m_sample_rate = value;
	}
	//generuje nastepna probke 
	T next() {
		//zwiekszanie fazy 
		m_phase+=m_hertz/m_sample_rate;
		if (m_phase >= 1) {
			m_phase -= 1;
		}
		return m_func(m_phase);
	}

	void setWaveform(Waveform wave) {
		m_waveform = wave;
		switch (m_waveform) {
		case Waveform::SINE:
			m_func = sine;
			break;
		case Waveform::SQUARE:
			m_func = square;
			break;
		case Waveform::TRIANGLE:
			m_func = triangle;
			break;
		}
	
	}

	T getHertz() const {
		return m_hertz;
	}

	//t z przedzialu <0,1)
	static float sine(float t) {
		return std::sin(t * juce::MathConstants<float>::twoPi);
	}

	static float square(float t) {
		if (t < 0.5f)
			return -1;
		else
			return 1;
	}

	static float triangle(float t) {
		if (t < 0.5f) {
			return 4 * t - 1;
		}
		else {
			return 3 - 4 * t;
		}
	}
};
