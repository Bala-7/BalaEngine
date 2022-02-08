// This example will show how to use sound effects such as echo, reverb and distortion.
// irrKlang supports the effects Chorus, Compressor, Distortion, Echo, Flanger
// Gargle, 3DL2Reverb, ParamEq and WavesReverb.

// Lets start: include the irrKlang headers and other input/output stuff
// needed to print and get user input from the console. And as exlained
// in the first tutorial, we use the namespace irr and audio and
// link to the irrKlang.dll file.
#if defined(WIN32)
	#include <conio.h>
#else
	#include "../common/conio.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <irrKlang.h>
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll


// Now let's start with the irrKlang 3D sound engine example 05,
// demonstrating sound effects. Simply startup the engine using
// using createIrrKlangDevice() with default options/parameters.
int main(int argc, const char** argv)
{
	// start the sound engine with default parameters
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		return 0; // error starting up the engine

	// we play a .xm file as music here. Note that the last parameter 
	// named 'enableSoundEffects' has been set to 'true' here. If this
	// is not done, sound effects cannot be used with this sound.
	// After this, we print some help text and start a loop which reads
	// user keyboard input.
	
	const char* filename = "../../media/MF-W-90.XM";

	#ifdef __BIG_ENDIAN__
		filename = "../../media/ophelia.mp3"; // no xm playback on power pcs currently
	#endif
	
	ISound* music = engine->play2D(filename,
		true, false, true, ESM_AUTO_DETECT, true);

	// Print some help text and start the display loop

	printf("\nSound effects example. Keys:\n");
	printf("\nESCAPE: quit\n");
	printf("w: enable/disable waves reverb\n");
	printf("d: enable/disable distortion\n");
	printf("e: enable/disable echo\n");
	printf("a: disable all effects\n");

	while(true) // endless loop until user exits
	{
		int key = getch();

		if (key == 27)
			break; // user pressed ESCAPE key
		else
		{
			// user maybe pressed an effects key,
			// now enable or disable a sound effect.

			// We get a pointer to the ISoundEffectControl interface,
			// but this only exists if the sound driver supports sound effects
			// and if the sound was started setting the 'enableSoundeffects' flag
			// to 'true' as we did above. This pointer is only valid as long as
			// we don't call music->drop() and delete the music with this.

			ISoundEffectControl* fx = 0;
			if (music)
				fx = music->getSoundEffectControl();

			if (!fx)
			{
				// some sound devices do not support sound effects.
				printf("This device or sound does not support sound effects.\n");
				continue;
			}	

			// here we disable or enable the sound effects of the music depending
			// on what key the user pressed. Note that every enableXXXSoundEffect()
			// method also accepts a lot of parameters, so it is easily possible
			// to influence the details of the effect. If the sound effect is 
			// already active, it is also possible to simply call the 
			// enableXXXSoundEffect() method again to just change the effect parameters,
			// although we aren't doing this here.

			if (key < 'a') // make key lower
				key += 'a' - 'A';

			switch(key)
			{
			case 'd':
				if (fx->isDistortionSoundEffectEnabled())
					fx->disableDistortionSoundEffect();
				else
					fx->enableDistortionSoundEffect();
				break;

			case 'e':
				if (fx->isEchoSoundEffectEnabled())
					fx->disableEchoSoundEffect();
				else
					fx->enableEchoSoundEffect();
				break;

			case 'w':
				if (fx->isWavesReverbSoundEffectEnabled())
					fx->disableWavesReverbSoundEffect();
				else
					fx->enableWavesReverbSoundEffect();
				break;

			case 'a':
				fx->disableAllEffects(); 
				break;
			}
		}			
	}

	// don't forget to release the resources

	if (music)
		music->drop(); // release music stream.

	engine->drop(); // delete Engine

	return 0;
}
