
class WaveformFunctions {
public:
	template <class T > static float sine(T t) {
		return std::sin(t * juce::MathConstants<T>::twoPi);
	}

	template <class T > static float square(T t) {
		if (t < 0.5f)
			return -1;
		else
			return 1;
	}

	template <class T > static float triangle(T t) {
		if (t < 0.5f) {
			return 4 * t - 1;
		}
		else {
			return 3 - 4 * t;
		}
	}
	
	template <class T > static float saw(T t) {
		return 2 * t - 1;
	}

	//parametr p
	template <class T > static float pulse(T p,T t) {
		if (t <= p) {
			return 1;
		}
		else {
			return -1;
		}
	}

};