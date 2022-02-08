// This example will show how to use sound effects such as echo, reverb and distortion.
// irrKlang supports the effects Chorus, Compressor, Distortion, Echo, Flanger
// Gargle, 3DL2Reverb, ParamEq and WavesReverb.

using System;
using IrrKlang;

namespace CSharp._05._Effects
{
	class Class1
	{
		[STAThread]
		static void Main(string[] args)
		{
			// start the sound engine with default parameters
			ISoundEngine engine = new ISoundEngine();

			// we play a .xm file as music here. Note that the last parameter 
			// named 'enableSoundEffects' has been set to 'true' here. If this
			// is not done, sound effects cannot be used with this sound.
			// After this, we print some help text and start a loop which reads
			// user keyboard input.

			ISound music = engine.Play2D("../../media/MF-W-90.XM", false,
				false, StreamMode.AutoDetect, true);

			// Print some help text and start the display loop

			Console.Out.Write("\nSound effects example. Keys:\n");
			Console.Out.Write("\nESCAPE: quit\n");
			Console.Out.Write("w: enable/disable waves reverb\n");
			Console.Out.Write("d: enable/disable distortion\n");
			Console.Out.Write("e: enable/disable echo\n");
			Console.Out.Write("a: disable all effects\n");

			while(true) // endless loop until user exits
			{
				int key = _getch();

				// Handle user input: Every time the user presses a key in the console,
				// play a random sound or exit the application if he pressed ESCAPE.

				if (key == 27)
					break; // user pressed ESCAPE key
				else
				{
					ISoundEffectControl fx = null;
					if (music != null)
						fx = music.SoundEffectControl;

					if (fx == null)
					{
						// some sound devices do not support sound effects.
						Console.Out.Write("This device or sound does not support sound effects.\n");
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
							if (fx.IsDistortionSoundEffectEnabled)
								fx.DisableDistortionSoundEffect();
							else
								fx.EnableDistortionSoundEffect();
							break;

						case 'e':
							if (fx.IsEchoSoundEffectEnabled)
								fx.DisableEchoSoundEffect();
							else
								fx.EnableEchoSoundEffect();
							break;

						case 'w':
							if (fx.IsWavesReverbSoundEffectEnabled)
								fx.DisableWavesReverbSoundEffect();
							else
								fx.EnableWavesReverbSoundEffect();
							break;

						case 'a':
							fx.DisableAllEffects(); 
							break;
					}
				}
			}
		}

		// simple functions for reading keys from the console
		[System.Runtime.InteropServices.DllImport("msvcrt")]
		static extern int _kbhit();
		[System.Runtime.InteropServices.DllImport("msvcrt")]
		static extern int _getch();
	}
}
