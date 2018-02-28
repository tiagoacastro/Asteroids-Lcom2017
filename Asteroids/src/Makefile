COMPILER_TYPE = gnu

CC = gcc

PROG = Asteroids
SRCS = main.c graphics.c Asteroid.c spaceship.c game.c keyboard.c Bitmap.c mouse.c timer.c vbe.c video_gr.c MainMenuState.c collision.c HighscoreState.c rtc.c rtc_asm.S EndGameState.c NewHighscoreState.c kbd_asm.S
CFLAGS= -Wall -O3

DPADD += ${LIBDRIVER} ${LIBSYS}
LDADD += -llm -ldriver -lsys

LDFLAGS += -L.

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
