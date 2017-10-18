#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "gameutil.h"


int ScreenHeight = 800;
int ScreenWidth = 800;
int mapheight = 1500;
int mapwidth = 1500;
const int FPS = 60;

int score = 0;
int lives = 3;
int level = 1;


int main(void)
{
    // seed for random tile map
    srand((unsigned int)time(NULL));


    node *blastlist = NULL;
    node2 *asteroidlist = NULL;


    ALLEGRO_DISPLAY *display;

    al_init_native_dialog_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    if(!al_init())
    {
        al_show_native_message_box(NULL, NULL, NULL, "Could not initialize allegro 5", NULL, ALLEGRO_MESSAGEBOX_QUESTION);
        exit(1);
    }

    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    display = al_create_display(ScreenWidth, ScreenHeight);
    ScreenWidth = al_get_display_width(display);
    ScreenHeight = al_get_display_height(display);

    al_hide_mouse_cursor(display);

    if(!display)
    {
        al_show_native_message_box(NULL, NULL, NULL, "Could not initialize allegro 5", NULL, ALLEGRO_MESSAGEBOX_QUESTION);
    }

    al_set_window_position(display, 0, 0);
    al_set_window_title(display, "ASTRAL*WARRIOR");

    ALLEGRO_FONT *font = al_load_font("assets/fonts/pirulen.ttf", 10, 0);

    ALLEGRO_FONT *font2 = al_load_font("assets/fonts/bodoni.ttf", 20, 0);

    al_init_primitives_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();

    al_reserve_samples(4);

    ALLEGRO_SAMPLE *explosionsound = al_load_sample("assets/sounds/explosion.wav");
    ALLEGRO_SAMPLE *lazersound = al_load_sample("assets/sounds/lazer.wav");
    ALLEGRO_SAMPLE *blastsound = al_load_sample("assets/sounds/blast.wav");

    ALLEGRO_SAMPLE *musicsound = al_load_sample("assets/music/ken.wav");
    ALLEGRO_SAMPLE *musicsound1 = al_load_sample("assets/music/guile.wav");
    ALLEGRO_SAMPLE *musicsound2 = al_load_sample("assets/music/battles.wav");

    ALLEGRO_SAMPLE_INSTANCE *musicinstance = al_create_sample_instance(musicsound);
    al_set_sample_instance_playmode(musicinstance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(musicinstance, al_get_default_mixer());

    ALLEGRO_SAMPLE_INSTANCE *musicinstance1 = al_create_sample_instance(musicsound1);
    al_set_sample_instance_playmode(musicinstance1, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(musicinstance1, al_get_default_mixer());

    ALLEGRO_SAMPLE_INSTANCE *musicinstance2 = al_create_sample_instance(musicsound2);
    al_set_sample_instance_playmode(musicinstance2, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(musicinstance2, al_get_default_mixer());

    ALLEGRO_COLOR mycolor = al_map_rgb(20, 150, 255);
    ALLEGRO_COLOR mycolor2 = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR blastcolor = al_map_rgb(255, 30, 0);
    ALLEGRO_COLOR asteroidcolor = al_map_rgb(255, 25, 255);

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);

    bool done = false;
    bool draw = true;
    int shipx = mapwidth / 2 - 8;
    int shipy = mapheight / 2 - 10;
    int shipcos = 0;
    int shipsin = 0;
    float asteroidspeed = 5;
    float lazerspeed = 15;
    float shipspeed = 0;
    float rotate = 0;
    direction dir = CTRL;

    ALLEGRO_BITMAP *bitmap = al_create_bitmap(16, 20);
    al_set_target_bitmap(bitmap);
    drawship(8, 11, &mycolor, 1.0);

    ALLEGRO_BITMAP *bitmap2 = al_create_bitmap(16, 20);
    al_set_target_bitmap(bitmap2);
    drawship2(8, 11, &mycolor2, 1.0);


    ALLEGRO_BITMAP *blast = al_create_bitmap(2, 6);
    al_set_target_bitmap(blast);
    drawblast(0, 0, &blastcolor);


    ALLEGRO_BITMAP *asteroid = al_create_bitmap(61, 61);
    al_set_target_bitmap(asteroid);
    drawasteroid(30, 30, &asteroidcolor, 1.0);


    ALLEGRO_COLOR asteroidcolor2 = al_map_rgb(0, 255, 50);
    ALLEGRO_BITMAP *asteroid2 = al_create_bitmap(61, 61);
    al_set_target_bitmap(asteroid2);
    drawasteroid(30, 30, &asteroidcolor2, 1.0);


    ALLEGRO_BITMAP *world = al_create_bitmap(mapwidth, mapheight);


    al_set_target_bitmap(al_get_backbuffer(display));

    bool death = false;
    int waitframes = 0;

    char scorestring[20] = "Score 0000000";

	ALLEGRO_BITMAP *line = al_create_bitmap(1, 1);
	al_set_target_bitmap(line);
	al_draw_filled_rectangle(0, 0, 1, 1, al_map_rgb(255, 255, 0));

	al_set_target_bitmap(al_get_backbuffer(display));

	// title screen

    al_draw_elliptical_arc(ScreenWidth / 2, ScreenHeight / 2, ScreenWidth / 2, ScreenHeight / 2, 0, ALLEGRO_PI, al_map_rgb(100, 200, 255), 1.0);
    al_draw_elliptical_arc(ScreenWidth / 2, ScreenHeight / 2, ScreenWidth / 2, ScreenHeight / 2, 0, -ALLEGRO_PI, al_map_rgb(100, 200, 255), 1.0);


    ALLEGRO_COLOR textcolor = al_map_rgba(20, 100, 255, 255);

    drawship(ScreenWidth / 2 - 45, ScreenHeight / 2 - 30, &mycolor, 1.0);
    drawship2(ScreenWidth / 2 + 45, ScreenHeight / 2 - 30, &mycolor2, 1.0);

    al_draw_text(font, al_map_rgb(190,40,255), ScreenWidth / 2, ScreenHeight / 2, ALLEGRO_ALIGN_CENTRE, "ASTRAL*WARRIOR");
    al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2 + 30, ALLEGRO_ALIGN_CENTRE, "Press any key to continue");
	draweffect(175, ScreenWidth / 2, ScreenHeight / 2, 15, 5, line);
    drawasteroid(ScreenWidth/2, ScreenHeight/2 - 30, &asteroidcolor2, 1.0);
    al_flip_display();


    ALLEGRO_EVENT key;
    ALLEGRO_EVENT_QUEUE *tempqueue = al_create_event_queue();
    al_register_event_source(tempqueue, al_get_keyboard_event_source());
    al_register_event_source(tempqueue, al_get_display_event_source(display));
    do
    {
        al_wait_for_event(tempqueue, &key);
    }while(key.type != ALLEGRO_EVENT_KEY_DOWN && key.type != ALLEGRO_EVENT_DISPLAY_CLOSE);


    if(key.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        goto finished;
    else if(key.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        if(key.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            goto finished;
    }




	// sources for assets

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(font2, textcolor, ScreenWidth / 2, 50, ALLEGRO_ALIGN_CENTRE, "MUSIC BY BULBY");

    al_draw_text(font2, textcolor, ScreenWidth / 2, 75, ALLEGRO_ALIGN_CENTRE, "channel: \
youtube.com/channel/UCz6zvgkf6eKpgqlUZQstOtQ");

    al_draw_text(font2, textcolor, ScreenWidth / 2, 100, ALLEGRO_ALIGN_CENTRE, "1st level: \
youtube.com/watch?v=4eg1uYQhcoM");

    al_draw_text(font2, textcolor, ScreenWidth / 2, 125, ALLEGRO_ALIGN_CENTRE, "2nd level: \
youtube.com/watch?v=kucS8dNTHuM");

    al_draw_text(font2, textcolor, ScreenWidth / 2, 150, ALLEGRO_ALIGN_CENTRE, "3rd level: \
youtube.com/watch?v=04_jviOqc3Y");

    al_draw_text(font2, textcolor, ScreenWidth / 2, 175, ALLEGRO_ALIGN_CENTRE, "SOUNDS");
    al_draw_text(font2, textcolor, ScreenWidth / 2, 200, ALLEGRO_ALIGN_CENTRE, "freesound.org");

    al_draw_text(font2, textcolor, ScreenWidth / 2, 225, ALLEGRO_ALIGN_CENTRE, "FONTS");
    al_draw_text(font2, textcolor, ScreenWidth / 2, 250, ALLEGRO_ALIGN_CENTRE, "1001freefonts.com");

    al_draw_text(font2, textcolor, ScreenWidth / 2, 275, ALLEGRO_ALIGN_CENTRE, "GAME LIBRARY");
    al_draw_text(font2, textcolor, ScreenWidth / 2, 300, ALLEGRO_ALIGN_CENTRE, "Allegro 5");
    al_draw_text(font2, textcolor, ScreenWidth / 2, 325, ALLEGRO_ALIGN_CENTRE, "liballeg.org");
    al_draw_text(font2, textcolor, ScreenWidth / 2, 350, ALLEGRO_ALIGN_CENTRE, "allegro.cc");
	al_draw_text(font, textcolor, ScreenWidth / 2, 375, ALLEGRO_ALIGN_CENTRE, "programming by mike mo");

    al_draw_text(font, textcolor, ScreenWidth / 2, 400, ALLEGRO_ALIGN_CENTRE, "press any key to continue");

    al_flip_display();

//www.youtube.com/watch?v=4eg1uYQhcoM
//www.youtube.com/watch?v=kucS8dNTHuM
//www.youtube.com/watch?v=04_jviOqc3Y

    do
    {
        al_wait_for_event(tempqueue, &key);
    }while(key.type != ALLEGRO_EVENT_KEY_DOWN && key.type != ALLEGRO_EVENT_DISPLAY_CLOSE);


    if(key.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        goto finished;
    else if(key.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        if(key.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            goto finished;
    }




    al_clear_to_color(al_map_rgb(0, 0, 0));



	// instructions screen
	textcolor = al_map_rgba(75, 255, 255, 255);

	drawship(30, 30, &mycolor, 1.0);
	drawship2(30, ScreenHeight / 2 - 30, &mycolor2, 1.0);

	al_draw_text(font, textcolor, ScreenWidth / 2, 45, ALLEGRO_ALIGN_CENTRE, "lightning");
	al_draw_text(font, textcolor, ScreenWidth / 2, 60, ALLEGRO_ALIGN_LEFT, "space to shoot");
	al_draw_text(font, textcolor, ScreenWidth / 2, 75, ALLEGRO_ALIGN_LEFT, "up/down to a/de/ccelerate");
	al_draw_text(font, textcolor, ScreenWidth / 2, 90, ALLEGRO_ALIGN_LEFT, "right/left to turn");
	al_draw_text(font, textcolor, ScreenWidth / 2, 105, ALLEGRO_ALIGN_LEFT, "right control to do a fullstop");

	al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2 - 15, ALLEGRO_ALIGN_CENTRE, "phantom");
	al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2, ALLEGRO_ALIGN_RIGHT, "space to shoot");
	al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2 + 15, ALLEGRO_ALIGN_RIGHT, "up to start moving or to move in rotated position, down to deccelerate");
	al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2 + 30, ALLEGRO_ALIGN_RIGHT, "right/left to turn");
	al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2 + 45, ALLEGRO_ALIGN_RIGHT, "right control to do a fullstop");
	al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2 + 60, ALLEGRO_ALIGN_RIGHT, "right shift to accelerate");

	al_draw_text(font, al_map_rgb(255, 25, 25), ScreenWidth / 2, ScreenHeight / 2 + 90, ALLEGRO_ALIGN_CENTRE, "r to reset lives");

	al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2 + 120, ALLEGRO_ALIGN_CENTRE, "press any key to continue");

	al_flip_display();


	do
	{
		al_wait_for_event(tempqueue, &key);
	} while (key.type != ALLEGRO_EVENT_KEY_DOWN && key.type != ALLEGRO_EVENT_DISPLAY_CLOSE);


	al_clear_to_color(al_map_rgb(0, 0, 0));



    direction dir2 = CTRL;
    bool finished = false;


    if(key.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        finished = true;
    else if(key.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        if(key.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            finished = true;
    }


    bool asteroidblasted = false;


    bool shipblasted = false;


    bool defmove = true;
    float temprotate = 0;


    bool framewait = true;
    int frames = 0;


	effectdata data;

	int effectframes = 0;

	bool cheat = false;

	while(!finished)
	{



		al_clear_to_color(al_map_rgb(0, 0, 0));

		mycolor = al_map_rgb(20, 150, 255);
		drawship(ScreenWidth / 2 - 30, ScreenHeight / 2 - 30, &mycolor, 1.0);

		mycolor = al_map_rgb(255, 255, 255);
		drawship2(ScreenWidth / 2 + 30, ScreenHeight / 2 - 30, &mycolor, 1.0);


		al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2, ALLEGRO_ALIGN_CENTRE, "LIGHTNING or PHANTOM");
		al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2 + 30, ALLEGRO_ALIGN_CENTRE, "Press L or P");
		al_flip_display();



		// check for keyboard input until valid response
		ALLEGRO_EVENT_QUEUE *temp = al_create_event_queue();
		al_register_event_source(temp, al_get_keyboard_event_source());
		ALLEGRO_EVENT key0;
		do
		{
			al_wait_for_event(temp, &key0);
		}while(key0.type != ALLEGRO_EVENT_KEY_DOWN || (key0.keyboard.keycode != ALLEGRO_KEY_L && key0.keyboard.keycode !=\
		ALLEGRO_KEY_P && key0.keyboard.keycode != ALLEGRO_KEY_ESCAPE));


		al_destroy_event_queue(temp);


		// set according to input
		if(key0.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if(key0.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				done = true;
				finished = true;
				goto finished;
			}
			else if(key0.keyboard.keycode == ALLEGRO_KEY_P)
			{
				defmove = false;
				al_set_target_bitmap(bitmap2);
				mycolor2 = al_map_rgb(255, 255, 255);
				drawship2(8, 11, &mycolor2, 1.0);
				al_set_target_bitmap(al_get_backbuffer(display));
			}
			else
			{
				defmove = true;
				al_set_target_bitmap(bitmap);
				mycolor = al_map_rgb(20, 150, 255);
				drawship(8, 11, &mycolor, 1.0);
				al_set_target_bitmap(al_get_backbuffer(display));

			}

		}


		al_clear_to_color(al_map_rgb(0, 0, 0));

		// set and start timer for FPS
		ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
		al_register_event_source(event_queue, al_get_keyboard_event_source());
		al_register_event_source(event_queue, al_get_display_event_source(display));
		al_register_event_source(event_queue, al_get_timer_event_source(timer));


		ALLEGRO_EVENT events;



		al_start_timer(timer);

		// game loop
		while(!done)
		{

			al_wait_for_event(event_queue, &events);


			// if keyboard key is pressed
			if(events.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch(events.keyboard.keycode)
				{
					// resets your lives (*cheat?*)
					case ALLEGRO_KEY_R:
						lives = 3;
						cheat = true;
						break;
					// set enum accordingly
					case ALLEGRO_KEY_SPACE:
						dir2 = SPACE;
						break;
					case ALLEGRO_KEY_DOWN:
						dir = DOWN;
						break;
					case ALLEGRO_KEY_UP:
						if(!defmove && shipspeed <= 0.1)
							shipspeed = 5.0;
						dir = UP;
						break;
					case ALLEGRO_KEY_RIGHT:
						dir = RIGHT;
						break;
					case ALLEGRO_KEY_LEFT:
						dir = LEFT;
						break;
					case ALLEGRO_KEY_RCTRL:
						dir = CTRL;
						break;
					case ALLEGRO_KEY_ESCAPE:
						done = true;
						finished = true;
						goto finished;
						break;
				}
			}
			else if(events.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch(events.keyboard.keycode)
				{

					// def move is if you chose lightning
					case ALLEGRO_KEY_RSHIFT:
						if(!defmove)
						{
							if(shipspeed < 15)
								shipspeed += 2.5;
						}
					case ALLEGRO_KEY_UP:
						if(defmove)
						{
							if(shipspeed < 15)
								shipspeed += 2.5;
						}
						dir = UP;
						break;
					case ALLEGRO_KEY_DOWN:
						if(shipspeed > 0)
							shipspeed -= 2.5;
						dir = DOWN;
						break;
					case ALLEGRO_KEY_RIGHT:
						dir = CTRL;
						break;
					case ALLEGRO_KEY_LEFT:
						dir = CTRL;
						break;
					case ALLEGRO_KEY_RCTRL:
						dir = CTRL;
						shipspeed = 0;
						break;
					case ALLEGRO_KEY_SPACE:
						dir2 = CTRL;
						break;
				}
			}
			else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				done = true;
				finished = true;
				goto finished;
			}

			if(events.type == ALLEGRO_EVENT_TIMER)
			{


				// checks for death
				// waits 2 seconds before checking for collisions again
				if(!death && collision(&blastlist, asteroidlist, shipx, shipy, &asteroidblasted, &shipblasted, &data))
				{
					shipx = mapwidth / 2 - 8;
					shipy = mapheight / 2 - 10;
					shipspeed = 0;
                    shipcos = 0;
                    shipsin = 0;

					rotate = 0;
					death = true;
				}
				else if(death)
					waitframes++;

				if(waitframes >= 120)
				{
					death = false;
					waitframes = 0;
				}

				switch(dir)
				{
					case RIGHT:
						rotate += ALLEGRO_PI / 18;
						break;
					case LEFT:
						rotate -= ALLEGRO_PI / 18;
						break;
					case CTRL:
						break;
				}

			

				// handles different movement for different ships
                if(defmove)
				{
					shipx += round(shipspeed * sin(rotate));
					shipy -= round(shipspeed * cos(rotate));
                    shipcos -= round(shipspeed * cos(rotate));
                    shipsin += round(shipspeed * sin(rotate));
				}
				else
				{
					if(dir == UP)
					{
						shipx += round(shipspeed * sin(rotate));
						shipy -= round(shipspeed * cos(rotate));
						temprotate = rotate;
					}
					else
					{
						shipx += round(shipspeed * sin(temprotate));
						shipy -= round(shipspeed * cos(temprotate));
					}
                    shipcos -= round(shipspeed * cos(temprotate));
                    shipsin += round(shipspeed * sin(temprotate));
				}



				// shoots lazers even when holding space
				// limited by framewait
				if(!framewait && dir2 == SPACE)
				{
					blastlist = insert_node(shipx, shipy, rotate, blastlist);
					al_play_sample(lazersound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					framewait = true;
				}
				else
				{
					if(frames <= 10)
						frames++;
					else
					{
						frames = 0;
						framewait = false;
					}
				}



				// teleports ship if near screen borders to opposite side
				if(borders(shipx, shipy))
				{

					if(shipy - 10 < 0)
                    {
						shipy = mapheight - 30;
                        shipcos = -1 * shipcos;
                    }
					else if(shipy + 25 > mapheight)
                    {
						shipy = 15;
                        shipcos = -1 * shipcos;
                    }


					if(shipx - 10 < 0)
                    {
						shipx = mapwidth - 30;
                        shipsin = -1 * shipsin;
                    }
					else if(shipx + 25 > mapwidth)
                    {
						shipx = 15;
                        shipsin = -1 * shipsin;
                    }

				}


				// plays sounds if asteroid or ship destroyed
				if(asteroidblasted)
				{
					al_play_sample(blastsound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					asteroidblasted = false;
				}
				if(shipblasted)
				{
					al_play_sample(explosionsound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					shipblasted = false;
				}


				// if level cleared advances level waits 2 seconds
				if(asteroidlist == NULL)
				{
					death = true;
					if(level == 1)
					{

						al_play_sample_instance(musicinstance);
						for(int i = 0; i < 10 * (round((mapheight + mapwidth) / 2 / 750)); i++)
						{
							float x2 = ((float)rand()/(float)(RAND_MAX)) * (mapwidth - 60) + 30;
							float y2 = ((float)rand()/(float)(RAND_MAX)) * (mapheight - 60) + 30;
							float rotaterand = ((float)rand()/(float)(RAND_MAX)) * (2 * ALLEGRO_PI - ALLEGRO_PI / 36) +\
							ALLEGRO_PI / 36;

							asteroidlist = insert_node2(x2, y2, rotaterand, 1.0, asteroidlist);
						}
						level++;
					}
					else if(level == 2)
					{
						al_stop_sample_instance(musicinstance);
						al_play_sample_instance(musicinstance1);
						for(int i = 0; i < 10 * (round((mapheight + mapwidth) / 2 / 750)); i++)
						{
							float x2 = ((float)rand()/(float)(RAND_MAX)) * (mapwidth - 60) + 30;
							float y2 = ((float)rand()/(float)(RAND_MAX)) * (mapheight - 60) + 30;
							float rotaterand = ((float)rand()/(float)(RAND_MAX)) * (2 * ALLEGRO_PI - ALLEGRO_PI / 36) +\
							ALLEGRO_PI / 36;

							asteroidlist = insert_node2(x2, y2, rotaterand, 1.5, asteroidlist);
						}
						level++;

					}
					else if(level == 3)
					{
						al_stop_sample_instance(musicinstance1);
						al_play_sample_instance(musicinstance2);
						for(int i = 0; i < 10 * (round((mapheight + mapwidth) / 2 / 750)); i++)
						{
							float x2 = ((float)rand()/(float)(RAND_MAX)) * (mapwidth - 60) + 30;
							float y2 = ((float)rand()/(float)(RAND_MAX)) * (mapheight - 60) + 30;
							float rotaterand = ((float)rand()/(float)(RAND_MAX)) * (2 * ALLEGRO_PI - ALLEGRO_PI / 36) +\
							ALLEGRO_PI / 36;

							asteroidlist = insert_node2(x2, y2, rotaterand, 1.5, asteroidlist);
						}
						for(int i = 0; i < 10 * (round((mapheight + mapwidth) / 2 / 750)); i++)
						{
							float x2 = ((float)rand()/(float)(RAND_MAX)) * (mapwidth - 60) + 30;
							float y2 = ((float)rand()/(float)(RAND_MAX)) * (mapheight - 60) + 30;
							float rotaterand = ((float)rand()/(float)(RAND_MAX)) * (2 * ALLEGRO_PI - ALLEGRO_PI / 36) +\
							ALLEGRO_PI / 36;

							asteroidlist = insert_node2(x2, y2, rotaterand, 1.0, asteroidlist);
						}

					}
				}

				draw = true;
			}


			bool first = true;
			// if timer finished and no events draw and update accordingly
			if(draw && al_is_event_queue_empty(event_queue))
			{

                al_set_target_bitmap(world);

                al_hold_bitmap_drawing(true);

                al_clear_to_color(al_map_rgb(0,0,0));

				if (data.collide && effectframes <= 30)
				{
					draweffect(5, data.xcor, data.ycor, 8, 3, line);
					effectframes++;
				}
				else
				{
					data.collide = false;
					effectframes = 0;
				}




				if(rotate > 2 * ALLEGRO_PI || rotate < -2 * ALLEGRO_PI)
				{
					rotate = 0;
				}


				if(defmove)
					al_draw_rotated_bitmap(bitmap, 8, 10, shipx, shipy, rotate, 0);
				else
					al_draw_rotated_bitmap(bitmap2, 8, 10, shipx, shipy, rotate, 0);


				draw_nodes(blastlist, blast);

				draw_nodes2(asteroidlist, asteroid, asteroid2, asteroidspeed);
                
                al_hold_bitmap_drawing(false);
                
                al_set_target_bitmap(al_get_backbuffer(display));


                al_hold_bitmap_drawing(true);

                al_draw_bitmap(world, ScreenWidth/2 - mapwidth/2 - shipsin, ScreenHeight/2 - mapheight/2 - shipcos, 0);

                al_hold_bitmap_drawing(false);


				// draws and checks for lives
				if(lives > -1)
				{
					int startx = 20;
					int starty = ScreenHeight - 20;
					ALLEGRO_COLOR livescolor = al_map_rgba(255, 20, 20, 255);
					for(int i = 0; i < lives; i++)
						drawship(startx + 16 * i, starty, &livescolor, 1.0);
				}
				else
				{
					done = true;
				}

				update_score(scorestring);

				ALLEGRO_COLOR textcolor = al_map_rgba(20, 100, 255, 255);
				al_draw_text(font, textcolor, 30, 30, ALLEGRO_ALIGN_LEFT, scorestring);


				al_flip_display();
				update_list(lazerspeed + shipspeed, &blastlist);

				update_list2(asteroidspeed, &asteroidlist);

				al_clear_to_color(al_map_rgb(0, 0, 0));
			}
		}
		al_stop_timer(timer);
		if(al_get_sample_instance_playing(musicinstance))
			al_stop_sample_instance(musicinstance);


		else if(al_get_sample_instance_playing(musicinstance1))
			al_stop_sample_instance(musicinstance1);


		else if(al_get_sample_instance_playing(musicinstance2))
			al_stop_sample_instance(musicinstance2);

		// game over screen
		al_clear_to_color(al_map_rgb(0, 0, 0));

		ALLEGRO_COLOR textcolor = al_map_rgba(255, 0, 0, 255);
		al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2, ALLEGRO_ALIGN_CENTRE, scorestring);
		al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2 - 30, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
		al_draw_text(font, textcolor, ScreenWidth / 2, ScreenHeight / 2 + 30, ALLEGRO_ALIGN_CENTRE,\
		"Press enter to play again, escape to quit");

		if(cheat)
			al_draw_text(font, al_map_rgb(250, 250, 0) , ScreenWidth / 2, ScreenHeight / 2 - 60, ALLEGRO_ALIGN_CENTRE, "you cheated!");

		al_flip_display();


		ALLEGRO_EVENT key1;
		ALLEGRO_EVENT_QUEUE *tempqueue1 = al_create_event_queue();
		al_register_event_source(tempqueue1, al_get_keyboard_event_source());
		al_register_event_source(tempqueue1, al_get_display_event_source(display));
		do
		{
			al_wait_for_event(tempqueue1, &key1);
		}while(key1.keyboard.keycode != ALLEGRO_KEY_ESCAPE && key1.keyboard.keycode != ALLEGRO_KEY_ENTER && key1.type != ALLEGRO_EVENT_DISPLAY_CLOSE);

		al_destroy_event_queue(tempqueue1);

		al_destroy_event_queue(event_queue);

		// terminates program if user decides to quit
		if(key1.type == ALLEGRO_EVENT_DISPLAY_CLOSE || key1.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			finished = true;
		// otherwise resets data/or state for another playthrough
		else
		{
			shipx = mapwidth / 2 - 8;
			shipy = mapheight / 2 - 10;
            shipcos = 0;
            shipsin = 0;
			shipspeed = 0;
			rotate = 0;
			dir = CTRL;
			dir2 = CTRL;

			score = 0;
			lives = 3;
			level = 1;
			update_score(scorestring);

			data.collide = false;
			effectframes = 0;
			cheat = false;

			done = false;
			death = true;
			destroy_list(&blastlist);
			destroy_list2(&asteroidlist);
		}

	}

        // forward goto to memory cleanup functions
	finished:
		al_destroy_font(font);
		al_destroy_font(font2);
		al_destroy_display(display);
		al_destroy_timer(timer);

		al_destroy_event_queue(tempqueue);

		al_destroy_bitmap(bitmap);
		al_destroy_bitmap(bitmap2);
		al_destroy_bitmap(blast);
		al_destroy_bitmap(asteroid);
		al_destroy_bitmap(asteroid2);
		al_destroy_bitmap(line);
        al_destroy_bitmap(world);

		al_destroy_sample(blastsound);
		al_destroy_sample(lazersound);
		al_destroy_sample(musicsound);
		al_destroy_sample(musicsound1);
		al_destroy_sample(musicsound2);
		al_destroy_sample(explosionsound);

		al_destroy_sample_instance(musicinstance);
		al_destroy_sample_instance(musicinstance1);
		al_destroy_sample_instance(musicinstance2);


		destroy_list(&blastlist);
		destroy_list2(&asteroidlist);

    return 0;
}
