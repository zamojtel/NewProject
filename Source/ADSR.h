
//klasa do modyfikacji amplitudy jednego g³osu
template<class T> class ADSR_envelope {
public:
	//czas mierzony jako liczba próbek 
	T m_attack;
	//m_attack m_decay m_release to czasy sustain to poziom
	T m_decay; //w czasie decay spadamy do poziomu sustain np. 0.5
	T m_sustain;
	T m_release;
	T m_envelope;
	T m_release_scale;
	
	int m_current_stage_elapsed;
	enum class Stage {
		ATTACK, DECAY, SUSTAIN, RELEASE,FINISHED
	};

	Stage m_current_stage;
	void start() {
		m_current_stage_elapsed = 0;
		m_current_stage = Stage::ATTACK;
	}

	T next() {
		switch (m_current_stage)
		{
			case Stage::ATTACK:
				m_current_stage_elapsed++;
				m_envelope = (m_current_stage_elapsed / m_attack);
				//if(m_current_state_elapsed==m_attack
				if (m_envelope >= 1) {
					m_envelope = 1;
					m_current_stage = Stage::DECAY;
					m_current_stage_elapsed = 0;
				}
				break;
			case Stage::DECAY:
				m_current_stage_elapsed++;
				m_envelope = 1 - (m_current_stage_elapsed / m_decay);
					if (m_envelope<=m_sustain) {
						m_envelope = m_sustain;
						m_current_stage = Stage::SUSTAIN;
					}
				break;
			case Stage::SUSTAIN:
				m_envelope = m_sustain;
				break;
				//release schodzi do zera 
			case Stage::RELEASE:
				m_current_stage_elapsed++;
				//aktualizacja 
				m_envelope = (1 - (m_current_stage_elapsed /(m_release_scale*m_release))) * m_release_scale;
				if (m_envelope<=0) {
					m_envelope = 0;
					m_current_stage = Stage::FINISHED;
				}
				break;
			case Stage::FINISHED:
				m_envelope = 0;
				break;
			default:
				break;
		}
		return m_envelope;

	}
	//gdy u¿ytkownik puszcza klawisz 
	void release() {
		m_release_scale = m_envelope;
		m_current_stage = Stage::RELEASE;
		m_current_stage_elapsed = 0;
	}

	bool isFinished() {
		if (m_current_stage == Stage::FINISHED)
			return true;
		else
			return false;
	}

};