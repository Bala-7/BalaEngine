// This example will show how to override file access with irrKlang.
// This is useful if you want to read sounds from other sources than
// just files, for example from custom internet streams or 
// an own encypted archive format.

// lets start: include irrKlang headers and other input/output stuff
// needed to print and get user input from the console.
#if defined(WIN32)
#include <conio.h>
#else
#include "../common/conio.h"
#endif

#include <string.h>
#include <stdio.h>
#include <irrKlang.h>
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

// To start, we need to implement the class IFileFactory, which irrKlang uses
// to open files. The interface consists only of one single method named 
// createFileReader(const ik_c8* filename). In this method, we create return 
// our own file access class and return it:

// a class implementing the IFileFactory interface to override irrklang file access
class CMyFileFactory : public IFileFactory
{
public:

	//! Opens a file for read access. Simply return 0 if file not found.
	virtual IFileReader* createFileReader(const ik_c8* filename)
	{
		printf("MyFileFactory: open file %s\n", filename);

		FILE* file = fopen(filename, "rb");
		if (!file)
			return 0;

		return new CMyReadFile(file, filename);
	}

protected:

	// To write our own file access methods returned in the method above,
	// we only need to implement the IFileReader interface, which has some
	// standard methods like read(), seek(), getPos() etc. In this example
	// we simply use fopen, fread, fseek etc and print to the console 
	// when we are reading or seeking:

	// an own implementation of IReadFile to overwrite read access to files 
	class CMyReadFile : public IFileReader
	{
	public:

		// constructor, store size of file and filename
		CMyReadFile(FILE* openedFile, const ik_c8* filename)
		{
			File = openedFile;
			strcpy(Filename, filename);

			// get file size
			fseek(File, 0, SEEK_END);
			FileSize = ftell(File);
			fseek(File, 0, SEEK_SET);
		}

		~CMyReadFile()
		{
			fclose(File);
		}

		//! reads data, returns how much was read
		ik_s32 read(void* buffer, ik_u32 sizeToRead)
		{
			printf("CMyReadFile: read %d bytes\n", sizeToRead);
			return (ik_s32)fread(buffer, 1, sizeToRead, File);
		}

		//! changes position in file, returns true if successful
		bool seek(ik_s32 finalPos, bool relativeMovement)
		{
			printf("CMyReadFile: seek to position %d\n", finalPos);
			return fseek(File, finalPos, relativeMovement ? SEEK_CUR : SEEK_SET) == 0;
		}

		//! returns size of file
		ik_s32 getSize()
		{
			return FileSize;
		}

		//! returns where in the file we are.
		ik_s32 getPos()
		{
			return ftell(File);
		}

		//! returns name of file
		const ik_c8* getFileName()
		{
			return Filename;
		}

		FILE* File;
		char Filename[1024];
		ik_s32 FileSize;

	}; // end class CMyReadFile

}; // end class CMyFileFactory



// The main work is done, the only thing missing is to start up the 
// sound engine and tell it to use the created FileFactory for file access:

// irrKlang 3D sound engine example 04, 
// demonstrating how to override file access of irrKlang
int main(int argc, const char** argv)
{
	// start the sound engine with default parameters
	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		return 0; // error starting up the engine

	// create an instance of the file factory and let
	// irrKlang know about it. irrKlang will drop() the
	// factory itself if it doesn't need it any longer.

	CMyFileFactory* factory = new CMyFileFactory();
	engine->addFileFactory(factory);
	factory->drop(); // we don't need it anymore, delete it

	// that's it, play some sounds with our overriden
	// file access methods:

	printf("\nDemonstrating file access overriding.\n");
	printf("Press any key to start playing sounds, then press escape to cancel\n");

	getch();
	
	engine->play2D("../../media/getout.ogg", true);

	while(true) // endless loop until user exits
	{
		// play some wave sound
		engine->play2D("../../media/explosion.wav");
		
		if (getch() == 27)
			break; // user pressed ESCAPE key, cancel
	}

	engine->drop(); // delete engine
	return 0;
} 
