// This example will show how to record and play back audio. Additionally,
// the example shows how to write recorded audio out into a .WAV file.
// Audio recording is currently only supported on windows, so this
// example won't work on Linux or MacOS for now.

// Lets start: include the irrKlang headers and other input/output stuff
// needed to print and get user input from the console. And as exlained
// in the first tutorial, we use the namespace irr and audio and
// link to the irrKlang.dll file.
#include <stdio.h>
#include <irrKlang.h>

// include console I/O methods (conio.h for windows, our wrapper in linux)
#if defined(WIN32)
#include <conio.h>
#else
#include "../common/conio.h"
#endif
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll


using namespace irrklang;

void writeWaveFile(const char* filename, SAudioStreamFormat format, void* data);

// The following will simply start up the irrklang engine, create an audio recorder, record
// some audio when the user presses a key, and save that data to a wave file. Additionally,
// the data is added into the sound engine and played back as well.
int main(int argc, const char** argv)
{
	ISoundEngine* engine = createIrrKlangDevice();
	IAudioRecorder* recorder = createIrrKlangAudioRecorder(engine);

	if (!engine || !recorder)
	{
		printf("Could not create audio engine or audio recoder\n");
		return 1;
	}

	printf("\nPress any key to start recording audio...\n");
	getch();

	// record some audio

	recorder->startRecordingBufferedAudio();

	printf("\nRECORDING. Press any key to stop...\n");
	getch();

	recorder->stopRecordingAudio();

	printf("\nRecording done, recorded %dms of audio.\n", 
		recorder->getAudioFormat().FrameCount * 1000 / recorder->getAudioFormat().SampleRate );
	printf("Press any key to play back recorded audio...\n");
	getch();
	
	// write the recorded audio as wave file
	writeWaveFile("recorded.wav", recorder->getAudioFormat(), recorder->getRecordedAudioData());

	// play the recorded audio
	recorder->addSoundSourceFromRecordedAudio("myRecordedVoice");
	engine->play2D("myRecordedVoice", true);

	// wait until user presses a key
	printf("\nPress any key to quit...");
	getch();

	recorder->drop();
	engine->drop(); // delete engine

	return 0;
}


// writes the recorded audio data into a .WAV file
void writeWaveFile(const char* filename, SAudioStreamFormat format, void* data)
{	
	if (!data)
	{
		printf("Could not save recorded data to %s, nothing recorded\n", filename);
		return;
	}

	FILE* file = fopen(filename, "wb");

	if (file)
	{
		// write wave header 
		unsigned short formatType =	1;
		unsigned short numChannels = format.ChannelCount;
		unsigned long  sampleRate =	format.SampleRate;
		unsigned short bitsPerChannel = format.getSampleSize() * 8;
		unsigned short bytesPerSample = format.getFrameSize() ;
		unsigned long  bytesPerSecond = format.getBytesPerSecond();
		unsigned long  dataLen = format.getSampleDataSize();
			
		const int fmtChunkLen = 16;
		const int waveHeaderLen = 4 + 8 + fmtChunkLen + 8;

		unsigned long totalLen = waveHeaderLen + dataLen;

		fwrite("RIFF", 4, 1, file);
		fwrite(&totalLen, 4, 1, file);
		fwrite("WAVE", 4, 1, file);
		fwrite("fmt ", 4, 1, file);
		fwrite(&fmtChunkLen, 4, 1, file);
		fwrite(&formatType, 2, 1, file);
		fwrite(&numChannels, 2, 1, file);
		fwrite(&sampleRate, 4, 1, file);
		fwrite(&bytesPerSecond, 4, 1, file);
		fwrite(&bytesPerSample, 2, 1, file);
		fwrite(&bitsPerChannel, 2, 1, file);

		// write data

		fwrite("data", 4, 1, file);
		fwrite(&dataLen, 4, 1, file);
		fwrite(data, dataLen, 1, file);

		// finish

		printf("Saved audio as %s\n", filename);
		fclose(file);
	}
	else
		printf("Could not open %s to write audio data\n", filename);
}