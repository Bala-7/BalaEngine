// This example will show how to override file access with irrKlang.
// This is useful if you want to read sounds from other sources than
// just files, for example from custom internet streams or 
// an own encypted archive format.

using System;
using System.IO;
using IrrKlang;

namespace CSharp._04._OverrideFileAccess
{
	class Class1
	{
		// To start, we need to implement the class IFileFactory, which irrKlang uses
		// to open files. The interface consists only of one single method named 
		// openFile(String filename). In this method, we create return 
		// our own file access class and return it:

		class MyIrrKlangFileFactory : IrrKlang.IFileFactory
		{
			public System.IO.Stream openFile(String filename)
			{
				// we simply could return an opened FileStream here, but to demonstrate
				// overriding, we return our own filestream implementation
				return new MyFileStream(filename);
			}
		}

		// an own implementation of FileStream to overwrite read access to files 
		public class MyFileStream : System.IO.FileStream
		{
			public MyFileStream(String filename) : base(filename, FileMode.Open)
			{
			}		
	
			public override int Read(byte[] array, int offset, int count)
			{
				System.Console.Out.WriteLine("MyFileStream read bytes: " + count);
				return base.Read(array, offset, count);
			}

			public override long Seek(long offset, SeekOrigin origin)
			{
				System.Console.Out.WriteLine("MyFileStream seeked to: " + offset);
				return base.Seek(offset, origin);
			}
		};



		// The main work is done, the only thing missing is to start up the 
		// sound engine and tell it to use the created FileFactory for file access:

		// irrKlang 3D sound engine example 04, 
		// demonstrating how to override file access of irrKlang
		[STAThread]
		static void Main(string[] args)
		{
			// start the sound engine with default parameters
			ISoundEngine engine = new ISoundEngine();

			// create an instance of the file factory and let
			// irrKlang know about it. 

			MyIrrKlangFileFactory myfactory = new MyIrrKlangFileFactory();
			engine.AddFileFactory(myfactory);

			// that's it, play some sounds with our overriden
			// file access methods:

			// now play some sounds until user presses 'q'

			Console.Out.WriteLine("\nDemonstrating file access overriding.");
			Console.Out.WriteLine("Press any key to play some sound, press ESCAPE to quit.");

			_getch();

			engine.Play2D("../../media/getout.ogg", true);			

			do
			{
				// play some wave sound
				engine.Play2D("../../media/bell.wav");
			}
			while(_getch() != 27); // user pressed eskape key to cancel
		}
		
		// some simple function for reading keys from the console
		[System.Runtime.InteropServices.DllImport("msvcrt")]
		static extern int _getch();
	}
}
