O =		jcomapi.o jdapimin.o jdapistd.o jdatasrc.o jdcoefct.o jdcolor.o \
		jddctmgr.o jdhuff.o jdinput.o jdmainct.o jdmarker.o jdmaster.o \
		jdmerge.o jdphuff.o jdpostct.o jdsample.o jdtrans.o jerror.o \
		jidctflt.o jidctfst.o jidctint.o jidctred.o jmemmgr.o jmemnobs.o \
		jquant1.o jquant2.o jutils.o

CC		=	gcc

# pour Debian / Ubuntu
CFLAGS	=	-O3 -fPIC

# pour les autres Linux / MacOSX / Windows
#CFLAGS	=	-O3

%.o : %.c
	$(CC) $(CFLAGS) -c $<

libjpeg.a:	$(O)
	ar cr $@ $(O)
	ranlib $@

clean:
	rm -f $(OBJ) $(O)
