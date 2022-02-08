using System;
using IrrKlang;

namespace CSharp._02._3DSound
{
	class Class1
	{
		[STAThread]
		static void Main(string[] args)
		{
			// start the sound engine with default parameters
			ISoundEngine engine = new ISoundEngine();

			// Now play some sound stream as music in 3d space, looped.
			// We play it at position (0,0,0) in 3d space

			ISound music = engine.Play3D("../../media/ophelia.mp3",
										 0,0,0, true);

			// the following step isn't necessary, but to adjust the distance where
			// the 3D sound can be heard, we set some nicer minimum distance
			// (the default min distance is 1, for a small object). The minimum
			// distance simply is the distance in which the sound gets played
			// at maximum volume.

			if (music != null)
				music.MinDistance = 5.0f;

			// Print some help text and start the display loop

			Console.Out.Write("\nPlaying streamed sound in 3D.");
			Console.Out.Write("\nPress ESCAPE to quit, any other key to play sound at random position.\n\n");

			Console.Out.Write("+ = Listener position\n");
			Console.Out.Write("o = Playing sound\n");

			Random rand = new Random(); // we need random 3d positions
			const float radius = 5;
			float posOnCircle = 0;

			while(true) // endless loop until user exits
			{
				// Each step we calculate the position of the 3D music.
				// For this example, we let the
				// music position rotate on a circle:

				posOnCircle += 0.04f;
				Vector3D pos3d = new Vector3D(radius * (float)Math.Cos(posOnCircle), 0,
											  radius * (float)Math.Sin(posOnCircle * 0.5f));

				// After we know the positions, we need to let irrKlang know about the
				// listener position (always position (0,0,0), facing forward in this example)
				// and let irrKlang know about our calculated 3D music position

				engine.SetListenerPosition(0,0,0, 0,0,1);

				if (music != null)
					music.Position = pos3d;

				// Now print the position of the sound in a nice way to the console
				// and also print the play position

				string stringForDisplay = "          +         ";
				int charpos = (int)((pos3d.X + radius) / radius * 10.0f);
				if (charpos >= 0 && charpos < 20)
				{
					stringForDisplay = stringForDisplay.Remove(charpos, 1);
					stringForDisplay = stringForDisplay.Insert(charpos, "o");					
				}

				uint playPos = 0;
				if (music != null)
					playPos = music.PlayPosition;

				string output = String.Format("\rx:({0})   3dpos: {1:f} {2:f} {3:f}, playpos:{4}:{5:00}    ",
					stringForDisplay, pos3d.X, pos3d.Y, pos3d.Z,
					playPos/60000, (playPos%60000)/1000);

				Console.Write(output);

				System.Threading.Thread.Sleep(100);

				// Handle user input: Every time the user presses a key in the console,
				// play a random sound or exit the application if he pressed ESCAPE.

				if (_kbhit()!=0)
				{
					int key = _getch();

					if (key == 27)
						break; // user pressed ESCAPE key
					else
					{
						// Play random sound at some random position.

						Vector3D pos = new Vector3D(((float)rand.NextDouble() % radius*2.0f) - radius, 0, 0);

						string filename;

						if (rand.Next()%2 != 0)
							filename = "../../media/bell.wav";
						else
							filename = "../../media/explosion.wav";

						engine.Play3D(filename, pos.X, pos.Y, pos.Z);

						Console.Write("\nplaying {0} at {1:f} {2:f} {3:f}\n",
							filename, pos.X, pos.Y, pos.Z);
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
