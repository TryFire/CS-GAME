#ifndef SOUND_H
#define SOUND_H

#ifdef _WIN32
#	include <conio.h>
#	include <windows.h>
#endif

#include "fmod.hpp"
#include "fmod_errors.h"

class Sound {
private:
	static FMOD::System*	_system;
	static FMOD::Channel*	_channel;
	static int		_nsounds;	// compte d'occurrences.

	FMOD::Sound*	_sound;
	void init (void);
public:
	Sound (const char*);				// charge un fichier de sons.
	~Sound ();					// libère les échantillons.
	void play (float volume =1., float pan =0.);	// le fait jouer.
};
#endif
