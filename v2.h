// second set of directory info vars for previews
bool pvupdate = true;
char pvdir[4096][128];
int pvnum;
bool pv = false;

int pvdraw(int d_y,int d_x,int d_h,int d_w)
{
	int i = 0;
	int ii;
	while ( i < (d_h*2) && i < pvnum )
	{
		move((d_y-d_h+i),d_x);
		ii = 0;
		while ( ii < d_w && ii < strlen(pvdir[i]) )
		{
			printw("%c",pvdir[i][ii]);
			ii++;
		}
		i++;
	}
}
void pvread(const char *direc)
{
	pvnum = 0;
	// code for reading the directory entries
	struct dirent *de;
	DIR *dr = opendir(direc);
	while ((de = readdir(dr)) != NULL)
	{
		// hidden files
		if ( yourhidden == false || de->d_name[0] != '.' )
		{
			// copy all of the dirent stuff into my own var
			strcpy(pvdir[pvnum], de->d_name);
			// count the directory items there are
			pvnum++;
		}
	}
	// decrement by one, as the loop goes one over
	pvnum--; 
}
/*
// preview? OmO
if ( dirnum > 0 && pv == true )
{
	for ( i=0; i<maxrow; i++ )
	{
		mvprintw(i,((maxcol/2)-1),"|");
	}
	stat(dirdir[myline], &sb);
	switch (sb.st_mode & S_IFMT)
	{
		case S_IFDIR:
			// call previewdir if is directory
			pvread(dirdir[myline]);
			pvdraw( ((maxrow/4)*3)+1, (maxcol/2),(maxrow/4),(maxcol/2));
			break;
		default:
			break;
	}
}
*/
