/* functional stuff */

// move down after selecting a file
const bool yourmoveselect = true;

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
#define EDIT		'e'
#define	RENAME		'r'
#define	APPEND		'a'
#define	TOGGLEHIDE	'z'
#define	DEL		'D'
#define	SHELLCMD	'$'
#define	TOGGLENUMBER	'n'
#define	QUIT		'q'
#define	EXECSHELL	's'
#define	SELECT		' '

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
