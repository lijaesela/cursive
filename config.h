/* functional stuff */

// loop back when hitting the top or bottom of a directory
const bool yourloop = true;

// go to the first item upon entering a directory
const bool yourzero = false;

// file opener
const char *youropener = "xdg-open";

// hide hidden files at startup
bool yourhidden = true;

// print numbers at startup
bool yournumbers = true;

/* vim keys */

// moving
#define	DOWN		'j'
#define	UP		'k'
#define	PGDOWN		'J'
#define	PGUP		'K'
#define	OPEN		'l'
#define	BACK		'h'
// actions
#define	OPENWITH	'o'
#define	MKDIR		'm'
#define	RENAME		'r'
#define	APPEND		'a'
#define	TOGGLEHIDE	'z'
#define	DEL		'D'
#define	SHELLCMD	'$'
#define	TOGGLENUMBER	'n'
#define	QUIT		'q'
#define	EXECSHELL	's'
// WIP actions
#define	SELECT		' '
#define	MYTEST		't'

/* theme */

// ncurses attributes for certain kinds of files
// examples: A_BOLD, A_DIM, A_STANDOUT, A_NORMAL
#define DIRTHEME	A_BOLD
#define CURSORTHEME	A_BOLD
#define NUMBERTHEME	A_DIM
#define SPACETHEME	A_DIM		// for "youritemspace"

// marker for the selected file, can be more than one character
const char *yourchar = ">";

// printed before all files, can be more than one character
// although it should probably be as long as "yourchar"
const char *youritemspace = " ";
