
// additional headers for simple console functions under linux,
// similar to conio.h in windows.
// used for irrKlang linux examples 

#ifndef __IRRKLANG_CONIO_H_INCLUDED__
#define __IRRKLANG_CONIO_H_INCLUDED__

#if !defined(_WIN32) && !defined(_WIN64) && !defined(__MACH__)

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

// sleeps 100 milliseconds
inline void sleepSomeTime() { usleep(100000); }

// returns if keyboard has been hit
inline int kbhit(void) 
{ 
	termios oldTerm, newTerm;
	int fd = 0; 
	
	tcgetattr(fd, &oldTerm); 
	newTerm = oldTerm;
	newTerm.c_lflag = newTerm.c_lflag & (!ICANON); 

	newTerm.c_cc[VMIN] = 0; 
	newTerm.c_cc[VTIME] = 1; 

	tcsetattr(fd, TCSANOW, &newTerm); 

	int c = getchar(); 

	tcsetattr(fd, TCSANOW, &oldTerm); 

	if (c != -1) 
		ungetc(c, stdin); 

	return ((c != -1) ? 1 : 0); 
}

// waits for the user to enter a character
inline int getch() 
{
	termios oldTerm, newTerm;

	tcgetattr( STDIN_FILENO, &oldTerm );
	newTerm = oldTerm;
	newTerm.c_lflag &= ~( ICANON | ECHO );

	tcsetattr( STDIN_FILENO, TCSANOW, &newTerm );

	int character = getchar();

	tcsetattr( STDIN_FILENO, TCSANOW, &oldTerm );

	return character;
}
//#endif // !defined(_WIN32) && !defined(_WIN64)
#elif defined(__MACH__)
// macOSX implementation

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/ioctl.h>

class RawTerm // keeps the terminal in raw mode as long as an instance of this class exists
{
	termios old;
	
public:
	RawTerm()
	{
		tcgetattr(STDIN_FILENO, &old);
		termios newt = old; 
 
		newt.c_iflag &= ~(IGNBRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON | BRKINT | PARMRK);
		newt.c_lflag &= ~(ICANON | ISIG | IEXTEN|ECHO | ECHONL);
		newt.c_cflag &= ~(CSIZE | PARENB);
		newt.c_cflag |= CS8;
		newt.c_oflag &= ~OPOST;
		
		tcsetattr(STDIN_FILENO, 0, &newt);
	}
	
	~RawTerm()
	{
		tcsetattr(STDIN_FILENO, 0, &old);
	}
};


inline int getch(void)
{
	RawTerm myRawterm;
	return getchar();
}


inline int kbhit(void)
{
	RawTerm myRawterm;
	
	int count = -1;
	ioctl(STDIN_FILENO, FIONREAD, &count);
	  
	return count > 0 ? count : 0;
}

// needed for irrklang
// sleeps 100 milliseconds
inline void sleepSomeTime() { usleep(100000); }

#endif

#endif // __IRRKLANG_CONIO_H_INCLUDED__

