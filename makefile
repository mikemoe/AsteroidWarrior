gameutil.o: gameutil.c gameutil.h
	gcc -I/usr/local/include -L/usr/local/lib -c gameutil.c -lallegro -lm\
    -lallegro_primitives
Astral\*Warrior: Astral\*Warrior.c gameutil.o gameutil.h
	gcc -I/usr/local/include -L/usr/local/lib Astral\*Warrior.c -lallegro -lallegro_dialog -lallegro_ttf\
    -lallegro_primitives -lallegro_font -lallegro_audio -lallegro_acodec -lm -o Astral\*Warrior
