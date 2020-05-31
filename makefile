CC			= gcc
CFLAGS	= -Wall -O2 -g
LDFLAGS = -lSDLmain -lSDL -lSDL_image -lSDL_ttf -framework GLUT -framework OpenGL -framework Cocoa -lm

BINDIR	= bin/
SRCDIR	= src/
OBJDIR	= obj/

projet.out: $(OBJDIR)main.o $(OBJDIR)map.o $(OBJDIR)action.o $(OBJDIR)achat.o $(OBJDIR)collisions.o $(OBJDIR)ia.o
		g++ -o projet.out $(OBJDIR)main.o $(OBJDIR)map.o $(OBJDIR)action.o $(OBJDIR)achat.o $(OBJDIR)collisions.o $(OBJDIR)ia.o $(LDFLAGS)

main.o: $(OBJDIR)main.c $(SRCDIR)main.h $(SRCDIR)structures.h $(SRCDIR)map.h $(SRCDIR)action.h $(SRCDIR)collisions.h $(SRCDIR)ia.h
		g++ -c $(SRCDIR)main.c $(SRCDIR)main.h $(SRCDIR)structures.h $(SRCDIR)map.h $(SRCDIR)action.h $(SRCDIR)collisions.h $(SRCDIR)ia.h $(LDFLAGS)

map.o: $(OBJDIR)map.c $(SRCDIR)map.h $(SRCDIR)structures.h
	     g++ -c $(SRCDIR)map.c $(SRCDIR)structures.h $(SRCDIR)map.h  $(LDFLAGS)
	     
action.o: $(OBJDIR)action.c $(SRCDIR)action.h $(SRCDIR)structures.h $(SRCDIR)collisions.h $(SRCDIR)main.h
		g++ -c $(SRCDIR)action.c $(SRCDIR)action.h $(SRCDIR)structures.h $(SRCDIR)collisions.h $(SRCDIR)main.h $(LDFLAGS)
		
achat.o: $(OBJDIR)achat.c $(SRCDIR)achat.h $(SRCDIR)structures.h $(SRCDIR)map.h $(SRCDIR)collisions.h $(SRCDIR)main.h
		g++ -c $(SRCDIR)achat.c $(SRCDIR)achat.h $(SRCDIR)structures.h $(SRCDIR)map.h $(SRCDIR)collisions.h $(SRCDIR)main.h $(LDFLAGS)

collisions.o: $(OBJDIR)collisions.c $(SRCDIR)collisions.h $(SRCDIR)structures.h
		g++ -c $(SRCDIR)collisions.c $(SRCDIR)collisions.h $(SRCDIR)structures.h $(LDFLAGS)
		 
ia.o: $(OBJDIR)ia.c $(SRCDIR)ia.h $(SRCDIR)structures.h $(SRCDIR)collisions.h $(SRCDIR)action.h $(SRCDIR)main.h
		g++ -c $(SRCDIR)ia.c $(SRCDIR)ia.h $(SRCDIR)structures.h $(SRCDIR)collisions.h $(SRCDIR)action.h $(SRCDIR)main.h  $(LDFLAGS)

clean :
	rm -rf *~
	rm -rf $(SRCDIR)*/*~
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)*

$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
