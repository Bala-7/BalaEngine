// This example will show how to play sounds using irrKlang.
// It will play a looped background music and a sound every
// time the user presses a key.

// At the beginning, we need to include the irrKlang headers (irrKlang.h) and
// the iostream headers needed to print text to the console.

#include <stdio.h>
#include <irrKlang.h>

// include console I/O methods (conio.h for windows, our wrapper in linux)
#if defined(WIN32)
#include <conio.h>
#else
#include "../common/conio.h"
#endif

// Also, we tell the compiler to use the namespaces 'irrklang'.
// All classes and functions of irrKlang can be found in the namespace 'irrklang'.
// If you want to use a class of the engine,
// you'll have to type an irrklang:: before the name of the class.
// For example, to use the ISoundEngine, write: irrklang::ISoundEngine. To avoid having
// to put irrklang:: before of the name of every class, we tell the compiler that
// we use that namespaces here.

using namespace irrklang;

// To be able to use the irrKlang.dll file, we need to link with the irrKlang.lib.
// We could set this option in the project settings, but to make it easy we use
// a pragma comment:

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

// Now lets start with irrKlang 3D sound engine example 01, demonstrating simple 2D sound.
// Start up the sound engine using createIrrKlangDevice(). You can specify several
// options as parameters when invoking that function, but for this example, the default
// parameters are enough.
int main(int argc, const char** argv)
{
	// start the sound engine with default parameters
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
	{
		printf("Could not startup engine\n");
		return 0; // error starting up the engine
	}

	// To play a sound, we only to call play2D(). The second parameter
	// tells the engine to play it looped.

	// play some sound stream, looped
	engine->play2D("../../media/getout.ogg", true);

	// In a loop, wait until user presses 'q' to exit or another key to
	// play another sound.

	printf("\nHello World!\n");

	do
	{
		printf("Press any key to play some sound, press 'q' to quit.\n");

		// play a single sound
		engine->play2D("../../media/bell.wav");
	}
	while(getch() != 'q');

	// After we are finished, we have to delete the irrKlang Device created earlier
	// with createIrrKlangDevice(). Use ::drop() to do that. In irrKlang, you should
	// delete all objects you created with a method or function that starts with 'create'.
	// (an exception is the play2D()- or play3D()-method, see the documentation or the
	// next example for an explanation)
	// The object is deleted simply by calling ->drop().

	engine->drop(); // delete engine
	return 0;
}
