#include <stdbool.h>
#include <allegro5/allegro.h>
#ifndef GAMEUTIL_H


#define SCORESIZE 7

int ScreenHeight = 800;
int ScreenWidth = 800;
const int FPS = 60.0;


int score = 0;
int lives = 3;
int level = 1;


enum direction {UP, DOWN, LEFT, RIGHT, CTRL, SPACE};
typedef enum direction direction;

enum collidetype {NONE, BLAST, SHIP};
typedef enum collidetype collidetype;


// data structure for ship lazers
struct link
{
    float x;
    float y;
    float rotate;
    float dist;
    struct link *prev;
    struct link *next;
};
typedef struct link node;

// data structure for asteroids
struct link2
{
    float x;
    float y;
    float rotate;
    float scale;
    float rotspeed;
    collidetype collide;
    struct link2 *prev;
    struct link2 *next;
};
typedef struct link2 node2;

struct effectdata
{
	float xcor;
	float ycor;
	bool collide;
};
typedef struct effectdata effectdata;

void drawship2(int x, int y, ALLEGRO_COLOR* mycolor, float thickness);

void drawship(int x, int y, ALLEGRO_COLOR* mycolor, float thickness);

void drawblast(int x, int y, ALLEGRO_COLOR* mycolor);

void drawasteroid(int x, int y, ALLEGRO_COLOR* mycolor, float thickness);

void draweffect(float radius, float startx, float starty, float xscale, float yscale, ALLEGRO_BITMAP *effect, ALLEGRO_DISPLAY *display);


bool borders(int x, int y);

bool validmove(int x, int y);

void deleteNode(node **head_ref, node *del);

void update_list(float movement, node **nodehead);

void draw_nodes(node *nodehead, ALLEGRO_BITMAP *image, ALLEGRO_DISPLAY *display);

void destroy_list(node **nodehead);

node *insert_node(float x, float y, float rotate, node *nodehead);

int update_score(char *scorestring);

node2 *insert_node2(float x, float y, float rotate, float scale, node2 *nodehead);
void deleteNode2(node2 **head_ref, node2 *del);

void update_list2(float movement, node2 **nodehead);

void draw_nodes2(node2 *nodehead, ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *image2, ALLEGRO_DISPLAY *display, float asteroidspeed);

void destroy_list2(node2 **nodehead);

bool collision(node **blastlist, node2 *asteroidlist, float x, float y, bool *asteroidblasted, bool *shipblasted, effectdata *data);

#endif
