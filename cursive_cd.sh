# source this file or copy this function to your shell's rc file
# change "$cursive_tmp" to match the directory in config.h

cursive_cd ()
{
	$cursive_tmp="/tmp/cursive_cd"
	cursive
	if [ -f $cursive_tmp ]; then
		cd `cat $cursive_tmp` || echo "cursive_cd error: could not change directory"
	else
		echo "cursive_cd error: tmp file not found"
	fi
}
