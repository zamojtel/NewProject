#ifndef COMPLEXINVERSEFFT_H
#define COMPLEXINVERSEFFT_H

template<typename T> class ComplexInverseFFT
{
public:
	ComplexInverseFFT()
	{
	}

	ComplexInverseFFT(size_t signal_size)
	{
		set_signal_size(signal_size);
	}
	
	size_t get_signal_size() const
	{
		return m_cfft.get_signal_size();
	}

	void perform(const std::complex<T> *input, std::complex<T> *output)
	{
		size_t size = get_signal_size();
		std::complex<T> *reversed = m_reversed.data();
		//zamiana czêœci urojenej z rzeczywist¹ w sygnale wejœciowym 
		for(size_t i = 0; i < size; ++i)
			reversed[i] = {input[i].imag(), input[i].real()};
		//przeprowadzamy dft
		m_cfft.perform(reversed, output);

		for(size_t i = 0; i < size; ++i)
		{
			//zamiana czêœæ urojon¹ i rzeczywist¹ w sygnale wyjœciowym 
			std::swap(reinterpret_cast<T(&)[2]>(output[i])[0], reinterpret_cast<T(&)[2]>(output[i])[1]);
			output[i] /= T(size); //skalowanie 1/N
		}
	}


	void set_signal_size(size_t a_size)
	{
		m_cfft.set_signal_size(a_size);
		m_reversed.resize(a_size);
	}



private:
	ComplexFFT<T> m_cfft;
	std::vector<std::complex<T>> m_reversed;
};

#endif 