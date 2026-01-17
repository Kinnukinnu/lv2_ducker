// ENVELOPE FOLLOWER BASED ON Citizen Chunks Example.

#include "env_follower.h"
#include <algorithm>	// for min(), max()
#include <cassert>		// for assert()
#include <cmath>


EnvelopeDetector::EnvelopeDetector( double ms, double sampleRate )
{
    assert( sampleRate > 0.0 );
    assert( ms > 0.0 );
    sampleRate_ = sampleRate;
    ms_ = ms;
    setCoef();
}

//-------------------------------------------------------------
void EnvelopeDetector::setTc( double ms )
{
    assert( ms > 0.0 );
    ms_ = ms;
    setCoef();
}

//-------------------------------------------------------------
void EnvelopeDetector::setSampleRate( double sampleRate )
{
    assert( sampleRate > 0.0 );
    sampleRate_ = sampleRate;
    setCoef();
}

//-------------------------------------------------------------
void EnvelopeDetector::setCoef( void )
{
    coef_ = exp( -1000.0 / ( ms_ * sampleRate_ ) );
}

//-------------------------------------------------------------
// attack/release envelope
//-------------------------------------------------------------
AttRelEnvelope::AttRelEnvelope( double att_ms, double rel_ms, double sampleRate )
    : att_( att_ms, sampleRate )
    , rel_( rel_ms, sampleRate )
{
}

//-------------------------------------------------------------
void AttRelEnvelope::setAttack( double ms )
{
    att_.setTc( ms );
}

//-------------------------------------------------------------
void AttRelEnvelope::setRelease( double ms )
{
    rel_.setTc( ms );
}

//-------------------------------------------------------------
void AttRelEnvelope::setSampleRate( double sampleRate )
{
    att_.setSampleRate( sampleRate );
    rel_.setSampleRate( sampleRate );
}


