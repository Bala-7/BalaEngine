using System;
using IrrKlang;

namespace CSharp._01.HelloWorld
{
	class Class1
	{
		[STAThread]
		static void Main(string[] args)
		{
			// start the sound engine with default parameters
			ISoundEngine engine = new ISoundEngine();

			// To play a sound, we only to call play2D(). The second parameter
			// tells the engine to play it looped.

			engine.Play2D("../../media/getout.ogg", true);

			Console.Out.WriteLine("\nHello World");

			do
			{
				Console.Out.WriteLine("Press any key to play some sound, press 'q' to quit.");

				// play a single sound
				engine.Play2D("../../media/bell.wav");
			}
			while(_getch() != 'q');
		}
		
		// some simple function for reading keys from the console
		[System.Runtime.InteropServices.DllImport("msvcrt")]
		static extern int _getch();
	}
}
