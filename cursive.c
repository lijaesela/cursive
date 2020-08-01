/* headers */

#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<curses.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

/* include the config */

#include"config.h"

/* variables */

// window dimensions and "ch" for input
int maxrow, maxcol, ch;

// position of user
int myline = 0;

// storing directory entries
bool dirupdate = true;
char dirdir[4096][128];
int dirnum;

// storing selected files
char cutbuffer[4096][1024];
int cutnumber = 0;
char cutdisp[4096][128];
char movecmdbuffer[4096];

// storing prompt input
char promptbuffer[256];

// for reading directories
struct stat sb;

/* functions */

int calcdigits(int number)
{
	// I copied this function from a forum lmao but it basically returns the number of digits of a base 10 number

	int digits = 0;

	do {
		++digits; 
		number /= 10;
	} while (number);

	return digits;
}
int myinit(void)
{
	// start curses mode
	initscr();

	// don't echo inputs
	noecho();

	// take inputs directly (but exit with Ctrl+c)
	cbreak();

	// hide terminal cursor
	curs_set(0);

	return 0;
}
char myconfirm(const char *prompt)
{
	// move to bottom left
	move(maxrow,0);

	// clear anything on that line
	clrtoeol();

	// print prompt
	printw(prompt);

	// set the global "ch" variable to get input
	ch = getch();
}
int myopenwith(const char *app, const char *file)
{
	char openbuff[128];

	// make a nice shell command with the sprintf function that I criminally underuse
	sprintf(openbuff, "%s %s", app, file);

	// end ncurses mode so the command can run in peace (and so it can be seen)
	endwin();

	// run the command
	system(openbuff);

	// start ncurses mode again
	myinit();

	return 0;
}
const char * myprompt(const char *ps1, int length)
{
	// allocate memory in the way that makes it work in functions for some reason
	char * str;
	str = malloc(sizeof(char)*maxcol);

	// set the stage at the bottom line for input
	echo();
	noraw();
	curs_set(1);
	clrtoeol();
	mvprintw(maxrow,0,ps1);

	// input
	mvgetstr(maxrow,length,str);

	// set everything back to normal
	echo();
	raw();
	curs_set(0);

	// return the input
	return str;
}
int mychangedir(const char *relativedir)
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	chdir(strcat(strcat(cwd, "/"), relativedir));
	return 0;
}
void mymv(void)
{
	return;
}
int myselectfile(const char *myfile)
{
	// get cwd
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	// get the absolute file path
	// concatenate CWD and filename together
	strcpy(cutbuffer[cutnumber], strcat(strcat(cwd,"/"),myfile) );

	// get the name alone as well, for UI purposes
	strcpy(cutdisp[cutnumber], myfile);

	// count the amount of selected items
	cutnumber++;
}
void myread(const char *direc)
{
	// start counting files at 0
	dirnum = 0;

	// code for reading the directory entries
	struct dirent *de;
	DIR *dr = opendir(direc);
	while ((de = readdir(dr)) != NULL)
	{
		// hidden files
		if ( yourhidden == false || de->d_name[0] != '.' )
		{
			// copy all of the dirent stuff into my own var
			strcpy(dirdir[dirnum], de->d_name);
			// count the directory items
			dirnum++;
		}
	}
	// decrement by one, as the loop goes one over
	dirnum--; 
}
int mydraw(int d_w)
{
	/*
	 * cursive draws files relatively to the cursor.
	 * This means the cursor is always in the middle of the screen.
	 * "Isn't clearing everything every single frame less efficient than just moving a cursor around?"
	 * "Yes."
	 * It looks really cool, alright?
	 */

	// multiple increments for epicly nested loops
	int i;
	int ii;

	// clear whatever was on the screen before
	clear();

	// don't bother with all of this if there isn't actually anything in the CWD
	if ( dirnum < 0 )
	{
		printw("[empty]");
		return 0;
	}

	// move to the right place to start the for loop
	move((maxrow/2)-myline,0);

	// the big boi loop, draws all files in CWD and their names
	for ( i=0; i<(dirnum + 1); i++ )
	{
		// get the stats for the current file in the loop for style and indication
		stat(dirdir[i], &sb);
		switch (sb.st_mode & S_IFMT)
		{
			// mark directories the configured way (bold by default)
			case S_IFDIR:
				attron(DIRTHEME);
				break;
			// unbold if not a directory, in case it is still set from previous line
			default:
				attroff(DIRTHEME);
				break;
		}

		// don't print unless within terminal bounds
		if ( (myline-(maxrow/2)) <= (i) && (i-myline) <= (maxrow/2) )
		{
			// print numbers if numbers are on
			if ( yournumbers == true )
			{
				attron(NUMBERTHEME);
				printw("%d", i);
				attroff(NUMBERTHEME);
			}

			// print item space
			attron(SPACETHEME);
			printw("%s", youritemspace);
			attroff(SPACETHEME);

			// extra space to make things less cramped when numbers are off
			if ( yournumbers == false )
			{
				printw(" ");
			}

			// print filenames with a loop on a char-by-char basis
			// this allows truncation
			for ( ii=0; ii < (d_w-calcdigits(i)-strlen(youritemspace)) && ii < strlen(dirdir[i]); ii++ )
			{
				printw("%c", dirdir[i][ii]);
			}

			// move to next line to repeat for next filename
			printw("\n");
		}
	}

	// print the cursor indicator
	attron(CURSORTHEME);mvprintw((maxrow/2),0,yourchar);attroff(CURSORTHEME);

	// no numbers on cursor line
	if ( yournumbers == true )
	{
		move((maxrow/2),strlen(yourchar));
		for ( i=0; i < calcdigits(myline); i++ )
			printw(" ");
	}

	// show a number of files in the bottom right corner
	mvprintw(maxrow,maxcol-calcdigits((dirnum+1)),"%d", (dirnum+1));

	// draw selected files starting from the top right of the window
	if ( cutnumber > 0 )
	{
		for ( i=0; i<cutnumber; i++ )
			mvprintw( i, maxcol-strlen(cutdisp[i]), "%s", cutdisp[i]);
	}

	// refresh the ncurses window to actually show everything
	refresh();
}

/* main */

int main( int argc, char *argv[] )
{
	// le increment has arrived
	int i;

	// exit if evoked incorrectly
	if ( argc > 2 )
	{
		printf("Usage: cursive [directory]");
		return 1;
	}
	// cd to [directory] argument, if given
	if ( argc == 2 )
		chdir(argv[1]);

	// init ncurses stuff
	myinit();

	// get window dimensions
	getmaxyx(stdscr,maxrow,maxcol); maxrow--; maxcol--;

	// main loop
	while (true)
	{
		// update cwd files if needed
		// signal to do this by setting this bool to "true" after any action that may change the directory
		if ( dirupdate == true )
			myread(".");

		// get out of blank space if entering a smaller directory
		if (myline < 0)
			myline = 0;
		else if (myline > dirnum)
			myline = dirnum;

		// draw the UI
		mydraw(maxcol);

		// wait for input
		// the only code after this line should be handling user input
		ch = getch();

		// actions
		switch (ch)
		{
			// movement
			case DOWN:
				myline++;
				break;
			case UP:
				myline--;
				break;
			case PGDOWN:
				myline = myline+(maxrow/2);
				break;
			case PGUP:
				myline = myline-(maxrow/2);
				break;

			// open file / cd to directory, ranger style
			case OPEN:
				// get stats for selected file
				stat(dirdir[myline], &sb);
				switch (sb.st_mode & S_IFMT)
				{
					// cd if direcrory
					case S_IFDIR:
						mychangedir(dirdir[myline]);
						if ( yourzero == true )
							myline = 0;
						break;
					// open with configured opener if anything else
					default:
						myopenwith(youropener, dirdir[myline]);
						break;
				}
				dirupdate = true;
				break;

			// go up a directory
			case BACK:
				chdir("./..");
				// go to the first item if configured to do so
				if ( yourzero == true )
					myline = 0;
				dirupdate = true;
				break;

			// make a directory with default GNU permissions
			case MKDIR:
				// put string returned from prompt in a buffer
				strcpy(promptbuffer, myprompt("mkdir: ", 7));
				// do the thing
				mkdir(promptbuffer, 0777);
				dirupdate = true;
				break;

			// rename file at cursor
			case RENAME:
				rename(dirdir[myline], myprompt("rename: ", 8));
				dirupdate = true;
				break;

			// rename but append given text to existing filename
			case APPEND:
				// add the filename to a buffer
				strcpy(promptbuffer, dirdir[myline]);
				// concatenate filename with string returned from prompt
				strcat(promptbuffer, myprompt("rename (append): ", 17));
				// change the old filename to what we now have in the buffer
				rename(dirdir[myline], promptbuffer);
				dirupdate = true;
				break;

			// deleting things
			case DEL:
				// check if there is a selection
				if ( cutnumber == 0 ) {
					// confirmation
					myconfirm("delete file at cursor? (y,*)");
					if ( ch == 'y' )
					{
						// get the type of file so it can be removed accordingly
						stat(dirdir[myline], &sb);
						switch (sb.st_mode & S_IFMT)
						{
							// remove directories with "rm" shell command
							case S_IFDIR:
								myopenwith("rm -rI", dirdir[myline]);
								break;
							// remove regular files
							case S_IFREG:
								remove(dirdir[myline]);
								break;
							// safe error for what I havent implemented yet
							default:
								myconfirm("can't delete this type (any key)");
								break;
						}
					}
				} else {
					// basically the same code as above but it runs in a for loop
					myconfirm("delete selected file(s)? (y,*)");
					if ( ch == 'y' )
					{
						for ( i=0; i<cutnumber; i++ )
						{
							stat(cutbuffer[i], &sb);
							switch (sb.st_mode & S_IFMT)
							{
								case S_IFDIR:
									myopenwith("rm -rI", cutbuffer[i]);
									break;
								case S_IFREG:
									remove(cutbuffer[i]);
									break;
								default:
									myconfirm("can't delete this type (any key)");
									break;
							}
						}
					}
					// set "cutnumber" to indicate that there is no longer a selection
					cutnumber = 0;
				}
				dirupdate = true;
				break;

			// select files for batch actions
			case SELECT:
				myselectfile(dirdir[myline]);
				if ( yourmoveselect == true )
					myline++;
				break;

			// prompt for what program to use to open a file
			case OPENWITH:
				sprintf(promptbuffer, "%s %s",
				myprompt("open with: ",11),
				dirdir[myline]);

				// end ncurses and run
				endwin();
				system(promptbuffer);
				myinit();

				dirupdate = true;
				break;

			// show/hide hidden files
			case TOGGLEHIDE:
				yourhidden = !yourhidden;
				dirupdate = true;
				break;
			case TOGGLENUMBER:
				yournumbers = !yournumbers;
				break;

			// run a single shell command
			case SHELLCMD:
				strcpy(promptbuffer,myprompt("$ ",2));

				// end ncurses and run
				endwin();
				system(promptbuffer);
				myinit();

				dirupdate = true;
				break;

			// start interactive shell
			case EXECSHELL:
				endwin();
				system(getenv("SHELL"));
				myinit();
				dirupdate = true;
				break;

			// open file in $EDITOR
			case EDIT:
				myopenwith(getenv("EDITOR"), dirdir[myline]);
				dirupdate = true;
				break;

			// move files by passing an mv command to the system
			case MOVE:
				strcpy(movecmdbuffer, "mv ");
				for ( i=0; i<cutnumber; i++ )
				{
					strcat(movecmdbuffer, cutbuffer[i]);
					strcat(movecmdbuffer, " ");
				}
				strcat(movecmdbuffer, "./");
				system(movecmdbuffer);
				cutnumber = 0;

				dirupdate = true;
				break;

			// handle window resize
			case KEY_RESIZE:
				getmaxyx(stdscr,maxrow,maxcol); maxrow--; maxcol--;
				break;

			// quitting
			case QUIT:
				endwin();
				return 0;
		}
	}
}
