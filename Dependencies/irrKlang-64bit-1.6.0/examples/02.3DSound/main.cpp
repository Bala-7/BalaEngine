// This example will show how to play sounds in 3D space using irrKlang.
// An mp3 file file be played in 3D space and moved around the user and a
// sound will be played at a random 3D position every time the user presses
// a key.

// For this example, we need some function to sleep for some seconds,
// so we include the platform specific sleep functions here. This is
// only need for demo purposes and has nothing to do with sound output.
// include console I/O methods (conio.h for windows, our wrapper in linux)
#if defined(WIN32)
#include <windows.h>
#include <conio.h>
inline void sleepSomeTime() { Sleep(100); }
#else
#include "../common/conio.h"
#endif

// Lets start: include the irrKlang headers and other input/output stuff
// needed to print and get user input from the console. And as exlained
// in the first tutorial, we use the namespace irr and audio and
// link to the irrKlang.dll file.
#include <stdio.h>
#include <irrKlang.h>
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll


// Now let's start with the irrKlang 3D sound engine example 02,
// demonstrating simple 3D sound. Simply startup the engine using
// using createIrrKlangDevice() with default options/parameters.
int main(int argc, const char** argv)
{
	// start the sound engine with default parameters
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		return 0; // error starting up the engine

	// Now play some sound stream as music in 3d space, looped.
	// We are setting the last parameter named 'track' to 'true' to
	// make irrKlang return a pointer to the played sound. (This is also returned
	// if the parameter 'startPaused' is set to true, by the way). Note that you
	// MUST call ->drop to the returned pointer if you don't need it any longer and
	// don't want to waste any memory. This is done in the end of the program.

	ISound* music = engine->play3D("../../media/ophelia.mp3",
	                               vec3df(0,0,0), true, false, true);

	// the following step isn't necessary, but to adjust the distance where
	// the 3D sound can be heard, we set some nicer minimum distance
	// (the default min distance is 1, for a small object). The minimum
	// distance simply is the distance in which the sound gets played
	// at maximum volume.

	if (music)
		music->setMinDistance(5.0f);

	// Print some help text and start the display loop

	printf("\nPlaying streamed sound in 3D.");
	printf("\nPress ESCAPE to quit, any other key to play sound at random position.\n\n");

	printf("+ = Listener position\n");
	printf("o = Playing sound\n");

	float posOnCircle = 0;
	const float radius = 5;

	while(true) // endless loop until user exits
	{
		// Each step we calculate the position of the 3D music.
		// For this example, we let the
		// music position rotate on a circle:

		posOnCircle += 0.04f;
		vec3df pos3d(radius * cosf(posOnCircle), 0,
					  radius * sinf(posOnCircle * 0.5f));

		// After we know the positions, we need to let irrKlang know about the
		// listener position (always position (0,0,0), facing forward in this example)
		// and let irrKlang know about our calculated 3D music position

		engine->setListenerPosition(vec3df(0,0,0), vec3df(0,0,1));

		if (music)
			music->setPosition(pos3d);

		// Now print the position of the sound in a nice way to the console
		// and also print the play position

		char stringForDisplay[] = "          +         ";
		int charpos = (int)((pos3d.X + radius) / radius * 10.0f);
		if (charpos >= 0 && charpos < 20)
			stringForDisplay[charpos] = 'o';
		int playPos = music ? music->getPlayPosition() : 0;

		printf("\rx:(%s)   3dpos: %.1f %.1f %.1f, playpos:%d:%.2d    ",
			stringForDisplay, pos3d.X, pos3d.Y, pos3d.Z,
			playPos/60000, (playPos%60000)/1000 );
	
		sleepSomeTime();

		// Handle user input: Every time the user presses a key in the console,
		// play a random sound or exit the application if he pressed ESCAPE.

		if (kbhit())
		{
			int key = getch();

			if (key == 27)
				break; // user pressed ESCAPE key
			else
			{
				// Play random sound at some random position.
				// Note that when calling play3D(), no pointer is returned because we didn't
				// specify the sound to start paused or to track it (as we did above
				// with the music), so we also don't need to call drop().

				vec3df pos(fmodf((float)rand(),radius*2)-radius, 0, 0);

				const char* filename;

				if (rand()%2)
					filename = "../../media/bell.wav";
				else
					filename = "../../media/explosion.wav";

				engine->play3D(filename, pos);

				printf("\nplaying %s at %.1f %.1f %.1f\n",
					filename, pos.X, pos.Y, pos.Z);
			}
		}
	}

	// don't forget to release the resources as explained above.

	if (music)
		music->drop(); // release music stream.

	engine->drop(); // delete engine
	return 0;
}
