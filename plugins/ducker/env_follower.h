// Envelope follower based on Citizen Chunks' example
#ifndef __ENV_FOLLOWER_H__
#define __ENV_FOLLOWER_H__


	//-------------------------------------------------------------
	// DC offset (to prevent denormal)
	//-------------------------------------------------------------

	// USE:
	// 1. init envelope state to DC_OFFSET before processing
	// 2. add to input before envelope runtime function
	static const double DC_OFFSET = 1.0E-25;

	//-------------------------------------------------------------
	// envelope detector
	//-------------------------------------------------------------
	class EnvelopeDetector
	{
	public:
		EnvelopeDetector(
			  double ms = 1.0
			, double sampleRate = 44100.0
			);
		virtual ~EnvelopeDetector() {}

		// time constant
		virtual void   setTc( double ms );
		virtual double getTc( void ) const { return ms_; }

		// sample rate
		virtual void   setSampleRate( double sampleRate );
		virtual double getSampleRate( void ) const { return sampleRate_; }

		// runtime function
		inline void run( double in, double &state ) {
			state = in + coef_ * ( state - in );
		}

	protected:
			
		double sampleRate_;		// sample rate
		double ms_;				// time constant in ms
		double coef_;			// runtime coefficient
		virtual void setCoef( void );	// coef calculation

	};	// end SimpleComp class

	//-------------------------------------------------------------
	// attack/release envelope
	//-------------------------------------------------------------
	class AttRelEnvelope
	{
	public:
		AttRelEnvelope(
			  double att_ms = 10.0
			, double rel_ms = 100.0
			, double sampleRate = 44100.0
			);
		virtual ~AttRelEnvelope() {}

		// attack time constant
		virtual void   setAttack( double ms );
		virtual double getAttack( void ) const { return att_.getTc(); }

		// release time constant
		virtual void   setRelease( double ms );
		virtual double getRelease( void ) const { return rel_.getTc(); }

		// sample rate dependencies
		virtual void   setSampleRate( double sampleRate );
		virtual double getSampleRate( void ) const { return att_.getSampleRate(); }

		// runtime function
		inline void run( double in, double &state ) {

			/* assumes that:
			* positive delta = attack
			* negative delta = release
			* good for linear & log values
			*/

			if ( in > state )
				att_.run( in, state );	// attack
			else
				rel_.run( in, state );	// release
		}

	private:
			
		EnvelopeDetector att_;
		EnvelopeDetector rel_;
		
	};


#endif	
