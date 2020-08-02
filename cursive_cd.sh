# source this file or copy this function to your shell's rc file
# change "$cursive_tmp" to match the directory in config.h

cursive_cd ()
{
	$cursive_tmp="/tmp/cursive_cd"
	cursive
	[ -f $cursive_tmp ] && cd `cat $cursive_tmp` || echo "cursive tmp file not found, cannot cd."
}
