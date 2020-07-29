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
// selected files UI
char cutdisp[4096][128];
// storing prompt input
char promptbuffer[256];
// for reading directories
struct stat sb;

/* functions */
bool checkdir(const char *direc)
{
	DIR* dir = opendir(direc);
	if (dir) {
		return true;
	 	closedir(dir);
	} else {
		return false;
	}
}
int shellcd(void)
{
	char cmd[3] = "cd ";
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	strcat(cmd, cwd);
	system(cmd);
}
int calcdigits(int number)
{
	int digits = 0;
	do {
		++digits; 
		number /= 10;
	} while (number);
	return digits;
}
char myconfirm(const char *prompt)
{
	move(maxrow,0);
	clrtoeol();
	printw(prompt);
	ch = getch();
}
int myinit(void)
{
	// start curses mode
	initscr();
	/* colors
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	attron(COLOR_PAIR(0));
	fucking stupid colors I hate them */
	// don't echo & raw input
	noecho();
	//raw();
	cbreak();
	// hide cursor
	curs_set(0);

	return 0;
}
int myopenwith(const char *app, ...)
{
	char openbuff[128];
	sprintf(openbuff, "%s %s", app, dirdir[myline]);

	clear();
	endwin();

	system(openbuff);

	myinit();
	return 0;
}
int myopenprompt(void)
{
	char openbuff[128];
	char str[(maxcol-1)];
	echo();
	noraw();
	curs_set(1);
	mvprintw(maxrow,0,"open: ");
	mvgetstr(maxrow,6,str);

	sprintf(openbuff, "%s %s", str, dirdir[myline]);

	clear();
	endwin();

	system(openbuff);

	myinit();
	return 0;
}
int mycommand(void)
{
	char str[(maxcol-1)];
	echo();
	noraw();
	curs_set(1);
	mvprintw(maxrow,0,"$ ");
	mvgetstr(maxrow,2,str);
	endwin();
	system(str);
	myinit();
	return 0;
}
const char * myprompt(const char *ps1, int length)
{
	char * str;
	str = malloc(sizeof(char)*128);
	echo();
	noraw();
	curs_set(1);
	mvprintw(maxrow,0,ps1);
	mvgetstr(maxrow,length,str);
	echo();
	raw();
	curs_set(0);
	return str;
}
int mychangedir(const char *relativedir)
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	chdir(strcat(strcat(cwd, "/"), relativedir));
	return 0;
}
int mydraw(int d_w)
{
	int i = 0;
	int ii;
	int i2;
	clear();
	move((maxrow/2)-myline,0);
	if ( dirnum < 0 )
	{
		printw("[empty]");
		return 0;
	}
	while ( i != (dirnum + 1) )
	{
		// get the stats for the current file in the loop
		stat(dirdir[i], &sb);
		// make directories bold
		switch (sb.st_mode & S_IFMT)
		{
			case S_IFDIR:
				attron(DIRTHEME);
				break;
			default:
				attroff(DIRTHEME);
				break;
		}
		// don't print unless within terminal bounds
		if ( (myline-(maxrow/2)) <= (i) && (i-myline) <= (maxrow/2) )
		{
			if ( yournumbers == true ) {
				attron(NUMBERTHEME);
				printw("%d", i);
				attroff(NUMBERTHEME);
			}
			attron(SPACETHEME);
			printw("%s", youritemspace);
			attroff(SPACETHEME);
			ii = 0;
			while ( ii < (d_w-calcdigits(i)-strlen(youritemspace)) && ii < strlen(dirdir[i]) )
			{
				printw("%c", dirdir[i][ii]);
				ii++;
			}
			printw("\n");
		}
		i++;
	}

	// print the cursor indicator
	attron(CURSORTHEME);mvprintw((maxrow/2),0,yourchar);attroff(CURSORTHEME);

	// no numbers on cursor line
	move((maxrow/2),strlen(yourchar));
	for ( i2=0; i2 < calcdigits(myline); i2++ )
		printw(" ");

	// show a number of files in the corner.
	mvprintw(maxrow,maxcol-calcdigits((dirnum+1)),"%d", (dirnum+1));
	refresh();
}
int myselectfile(const char *myfile)
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	strcpy(cutbuffer[cutnumber], strcat(strcat(cwd,"/"),myfile) );
	strcpy(cutdisp[cutnumber], myfile);
	cutnumber++;
}
void myread(const char *direc)
{
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
			// count the directory items there are
			dirnum++;
		}
	}
	// decrement by one, as the loop goes one over
	dirnum--; 
}
/* main */
int main( int argc, char *argv[] )
{
	int i;
	int i2;
	// exit if bad arguments
	if ( argc > 2 )
	{
		printf("too many arguments\n");
		return 1;
	}
	if ( argc == 2 )
		chdir(argv[1]);

	// init curses stuff
	myinit();

	// get window dimensions
	getmaxyx(stdscr,maxrow,maxcol); maxrow--; maxcol--;

	// main loop
	while (true)
	{
		// update cwd files if needed
		if ( dirupdate == true )
			myread(".");

		/* get out of blank space
		   if entering a small directory */
		if (myline > dirnum)
			myline = dirnum;
		if (myline < 0)
			myline = 0;

		// new seperate draw loop
		// beginning of the DRAW ZONE
		mydraw(maxcol);

		// wait for input
		// the only code after this line should be input related
		ch = getch();

		// actions
		switch (ch)
		{
			case PGDOWN:
				if ( myline == dirnum )
				{
					if ( yourloop == false )
						break;
					else
						myline = 0;
				}
				else
					myline = myline+10;
				break;
			case PGUP:
				if ( myline == 0 )
				{
					if ( yourloop == false )
						break;
					else
						myline = dirnum;
				}
				else
					myline = myline-10;
				break;
			case DOWN:
				if ( myline == dirnum )
				{
					if ( yourloop == false )
						break;
					else
						myline = 0;
				}
				else
					myline++;
				break;
			case UP:
				if ( myline == 0 )
				{
					if ( yourloop == false )
						break;
					else
						myline = dirnum;
				}
				else
					myline--;
				break;
			case OPEN:
				// get stats for selected file
				stat(dirdir[myline], &sb);
				// open handler
				switch (sb.st_mode & S_IFMT)
				{
					case S_IFDIR:
						mychangedir(dirdir[myline]);
						if ( yourzero == true )
							myline = 0;
						dirupdate = true;
						break;
					default:
						myopenwith(youropener);
						break;
				}
				break;
			case BACK:
				chdir("./..");
				if ( yourzero == true )
					myline = 0;
				dirupdate = true;
				break;
			case MKDIR:
				strcpy(promptbuffer, myprompt("mkdir: ", 7));
				mkdir(promptbuffer, 0777);
				dirupdate = true;
				break;
			case RENAME:
				strcpy(promptbuffer, myprompt("rename: ", 8));
				rename(dirdir[myline], promptbuffer);
				dirupdate = true;
				break;
			case APPEND:
				strcpy(promptbuffer, dirdir[myline]);
				strcat(promptbuffer, myprompt("rename (append): ", 17));
				rename(dirdir[myline], promptbuffer);
				dirupdate = true;
				break;
			case DEL:
				myconfirm("really delete? (y,*)");
				if ( ch == 'y' )
					remove(dirdir[myline]);
				dirupdate = true;
				break;
			case SELECT:
				myselectfile(dirdir[myline]);
				break;
			case OPENWITH:
				myopenprompt();
				break;
			case TOGGLEHIDE:
				yourhidden = !yourhidden;
				dirupdate = true;
				break;
			case TOGGLENUMBER:
				yournumbers = !yournumbers;
				break;
			case SHELLCMD:
				mycommand();
				dirupdate = true;
				break;
			case EXECSHELL:
				endwin();
				system(getenv("SHELL"));
				myinit();
				dirupdate = true;
				break;
			case MYTEST:
				for ( i2=0; i2<cutnumber; i2++ )
					mvprintw(i2,0,"%s\n",cutdisp[i2]);
					mvprintw(i2,10,"%s\n",cutbuffer[i2]);
				mvprintw(0,maxcol-8,"%d",cutnumber);
				getch();
			// apparently resizing the terminal sends a character to ncurses
			// so that's cool
			case KEY_RESIZE:
				getmaxyx(stdscr,maxrow,maxcol); maxrow--; maxcol--;
				break;
			// quitting but cooler because it's with all of the other inputs now
			case QUIT:
				endwin();
				return 0;
		}
	}
}
