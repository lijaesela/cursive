# cursive
cursive is an ncurses file browser for GNU/Linux written in pure C.  
This program is DEPRECATED. It's also really weird. Use at your own risk.  

### Building, Running, and Installing
(in any directory)  
```shell
git clone https://github.com/lijaesela/cursive
```  
```shell
cd cursive
```  
```shell
make
```  
```shell
./cursive
```  
to install (root privileges needed):  
```shell
make install
```
It is easily possible to install without root privileges by simply editing the variables in the makefile.
```makefile
INSTALLDIR = /usr/local/bin
MANDIR = /usr/local/share/man
```
This configuration will not require root privileges.
```makefile
INSTALLDIR = /home/jimbo/.local/bin
MANDIR = /home/jimbo/local/share/man
```

### Documentation
Man page :3  
```shell
man ./cursive.1
```  
or, if installed:  
```shell
man cursive
```
