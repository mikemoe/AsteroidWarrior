#include "gameutil.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

// draws phantom ship
void drawship2(int x, int y, ALLEGRO_COLOR* mycolor, float thickness)
{
    al_draw_elliptical_arc(x, y, 8, 10, -ALLEGRO_PI * 1.40, ALLEGRO_PI * 1.80, *mycolor, thickness);

    al_draw_line(x + -6, y, x + -1, y + 4, *mycolor, thickness);

    al_draw_line(x + 6, y, x + 1, y + 4, *mycolor, thickness);
}

// draws lightning ship
void drawship(int x, int y, ALLEGRO_COLOR* mycolor, float thickness)
{
    al_draw_line(x + -8, y + 9, x + 0, y + -11, *mycolor, thickness);

    al_draw_line(x + 0, y + -11, x + 8, y + 9, *mycolor, thickness);

    al_draw_line(x + -2, y + -2, x + 8, y + 9, *mycolor, thickness);

    al_draw_line(x + 2, y + -2, x + -8, y + 9, *mycolor, thickness);
}

void drawblast(int x, int y, ALLEGRO_COLOR* mycolor)
{
    al_draw_filled_rectangle(x, y, x + 2, y + 6, *mycolor);
}

/*
void drawasteroid(int x, int y, ALLEGRO_COLOR* mycolor, float thickness)
{
    al_draw_line(x + -20, y + 20, x + -25, y + -10, *mycolor, thickness);

    al_draw_line(x + -25, y + 5, x + -25, y + -10, *mycolor, thickness);

    al_draw_line(x + -25, y + -10, x + -5, y + -10, *mycolor, thickness);

    al_draw_line(x + -5, y + -10, x + -10, y + -20, *mycolor, thickness);

    al_draw_line(x + -10, y + -20, x + 5, y + -20, *mycolor, thickness);

    al_draw_line(x + 5, y + -20, x + 20, y + -10, *mycolor, thickness);

    al_draw_line(x + 20, y + -10, x + 20, y + -5, *mycolor, thickness);

    al_draw_line(x + 20, y + -5, x + 0, y + 0, *mycolor, thickness);

    al_draw_line(x + 0, y + 0, x + 20, y + 10, *mycolor, thickness);

    al_draw_line(x + 20, y + 10, x + 10, y + 20, *mycolor, thickness);

    al_draw_line(x + 10, y + 20, x + 0, y + 15, *mycolor, thickness);

    al_draw_line(x + 0, y + 15, x + -20, y + 20, *mycolor, thickness);
}
*/


void drawasteroid(int x, int y, ALLEGRO_COLOR* mycolor, float thickness)
{
    // diagonal lines on sides
    al_draw_line(x + -30, y + 0, x + -15, y + -30, *mycolor, thickness);

    al_draw_line(x + 15, y + -30, x + 30, y + 0, *mycolor, thickness);

    al_draw_line(x + -30, y + 0, x + -15, y + 30, *mycolor, thickness);

    al_draw_line(x + 15, y + 30, x + 30, y + 0, *mycolor, thickness);

    // top and bottom lines
    al_draw_line(x + -15, y + -30, x + 15, y + -30, *mycolor, thickness);

    al_draw_line(x + -15, y + 30, x + 15, y + 30, *mycolor, thickness);


    al_draw_line(x + -30, y + 0, x + 15, y + -30, *mycolor, thickness);

    al_draw_line(x + -15, y + -30, x + 30, y + 0, *mycolor, thickness);

    al_draw_line(x + 15, y + -30, x + 15, y + 30, *mycolor, thickness);

    al_draw_line(x + 30, y + 0, x + -15, y + 30, *mycolor, thickness);

    al_draw_line(x + 15, y + 30, x + -30, y + 0, *mycolor, thickness);

    al_draw_line(x + -15, y + 30, x + -15, y + -30, *mycolor, thickness);



    al_draw_line(x + -15, y + 10, x + 0, y + 0, *mycolor, thickness);

    al_draw_line(x + 15, y + 10, x + 0, y + 0, *mycolor, thickness);

    al_draw_line(x + 0, y + -20, x + 0, y + 0, *mycolor, thickness);
}



void draweffect(float radius, float startx, float starty, float xscale, float yscale, ALLEGRO_BITMAP *effect)
{
	
	float rotate = ALLEGRO_PI / 3;
	float angle = 0;

	for (int i = 0; i < 6; i++)
	{
		float rotation = rotate * i;

		startx += radius * sin(rotation);
		starty -= radius * cos(rotation);

		switch (i)
		{
			case 0:
				angle = ALLEGRO_PI / 4;
				break;
			case 1:
				angle = 2 * ALLEGRO_PI / 4;
				break;
			case 2:
				angle = ALLEGRO_PI - ALLEGRO_PI / 4;
				break;
			case 3:
				angle = ALLEGRO_PI / 4;
				break;
			case 4:
				angle = 2 * ALLEGRO_PI / 4;
				break;
			case 5:
				angle = ALLEGRO_PI - ALLEGRO_PI / 4;
				break;
			default:
				angle = 0;
		}

		al_draw_scaled_rotated_bitmap(effect, 0.5, 0.5, startx, starty, xscale, yscale, angle, 0);
	}
}

bool borders(int x, int y)
{
    bool collide = false;
    // if ship is near screen borders set flag to true
    if(x - 4 < 0 || y - 10 < 0 || x + 20 > mapwidth || y + 25 > mapheight)
        collide = true;


    return collide;
}


bool validmove(int x, int y)
{
    // returns false if ship is near screen borders
    bool valid = true;
    if(borders(x, y))
    {
        if(y - 10 < 0 || y + 25 > mapheight)
            valid = false;
        if(x - 4 < 0 || x + 20 > mapwidth)
            valid = false;
    }

    return valid;
}

/*
these functions are for ship lazer doubly list manipulation


*/

void deleteNode(node **head_ref, node *del)
{
  /* base case */
    if(*head_ref == NULL || del == NULL)
        return;

   /* If node to be deleted is head node */
    if(*head_ref == del)
        *head_ref = del->next;

    /* Change next only if node to be deleted is NOT the last node */
    if(del->next != NULL)
        del->next->prev = del->prev;

     /* Change prev only if node to be deleted is NOT the first node */
    if(del->prev != NULL)
        del->prev->next = del->next;

  /* Finally, free the memory occupied by del*/
    free(del);
    return;
}

void update_list(float movement, node **nodehead)
{
    node *trav = *nodehead;
    // traverse entire list of doubly linked nodes
    while(trav != NULL)
    {
        // if ship nears screen borders
        if(borders(trav->x, trav->y))
        {

            // teleport to opposite side of screen
            if(trav->y - 10 < 0)
                trav->y = mapheight - 30;
            else if(trav->y + 25 > mapheight)
                trav->y = 15;


            if(trav->x - 10 < 0)
                trav->x = mapwidth - 30;
            else if(trav->x + 25 > mapwidth)
                trav->x = 15;


        }

        // if node list empty
        if(trav == NULL)
            break;

        // update distance node (*ship lazer*) has traveled
        trav->dist += sqrt(2 * pow(movement, 2));

        // if traveled greater than 2k delete node (*lazer*)
        if(trav->dist > 2000)
        {
            node *freenode = trav;
            trav = trav->next;
            deleteNode(nodehead, freenode);
            continue;
        }

        // rotate and advance lazer
        trav->x += movement * sin(trav->rotate);
        trav->y -= movement * cos(trav->rotate);

        // update trav pointer to next node
        trav = trav->next;
    }
}

void draw_nodes(node *nodehead, ALLEGRO_BITMAP *image)
{
    node *trav = nodehead;
    // iterate through lazers
    while(trav != NULL)
    {
        // draw lazer
        al_draw_rotated_bitmap(image, 1, 3, trav->x, trav->y, trav->rotate, 0);
        // next in lazer list
        trav = trav->next;
    }
}

void destroy_list(node **nodehead)
{
    node *freenode;
    node *trav = *nodehead;
    // iterate list
    while(trav != NULL)
    {
        // delete element
        freenode = trav;
        trav = trav->next;
        deleteNode(nodehead, freenode);
    }
}

node *insert_node(float x, float y, float rotate, node *nodehead)
{
    node *trav;
    node *newnode;
    // if list empty make new node and point nodehead to it
    if(nodehead == NULL)
    {
        nodehead = malloc(sizeof(node));
        nodehead->prev = NULL;
        nodehead->next = NULL;
        nodehead->x = x;
        nodehead->y = y;
        nodehead->rotate = rotate;
        nodehead->dist = 0;
    }

    // traverse list until null and make new node
    else
    {
        trav = nodehead;
        while(trav->next != NULL)
            trav = trav->next;
        trav->next = malloc(sizeof(node));
        newnode = trav->next;
        newnode->prev = trav;
        newnode->next = NULL;
        newnode->x = x;
        newnode->y = y;
        newnode->rotate = rotate;
        newnode->dist = 0;
    }
    return nodehead;
}

int update_score(char *scorestring)
{
    int error = 0;
    int len = strlen(scorestring);
    char buffer[SCORESIZE + 1];
    // copy integer value into a buffer including null char
    snprintf(buffer, SCORESIZE + 1, "%d", score);
    int lenbuf = strlen(buffer);

    // if score is greater than set scoresize set error to 1
    if(score >= (int)pow(10, SCORESIZE))
    {
        error = 1;
    }
    // otherwise, copy in scorestring to be displayed
    else
    {
		int scorelen = SCORESIZE;
        for(int i = len; scorelen > 0; scorelen--, i--, lenbuf--)
        {
           if(lenbuf > 0)
               scorestring[i - 1] = buffer[lenbuf - 1];
           else
               scorestring[i - 1] = '0';
        }
    }

    return error;
}

/*
these are for asteroid doubly
list manipulation
*/

// comments are the same as above besides minor details 
node2 *insert_node2(float x, float y, float rotate, float scale, node2 *nodehead)
{
    node2 *trav;
    node2 *newnode;
    if(nodehead == NULL)
    {
        nodehead = malloc(sizeof(node2));
        nodehead->prev = NULL;
        nodehead->next = NULL;
        nodehead->x = x;
        nodehead->y = y;
        nodehead->rotate = rotate;
        nodehead->rotspeed = 0;
        nodehead->scale = scale;
        nodehead->collide = NONE;
    }
    else
    {
        trav = nodehead;
        while(trav->next != NULL)
            trav = trav->next;
        trav->next = malloc(sizeof(node2));
        newnode = trav->next;
        newnode->prev = trav;
        newnode->next = NULL;
        newnode->x = x;
        newnode->y = y;
        newnode->rotate = rotate;
        newnode->rotspeed = 0;
        newnode->scale = scale;
        newnode->collide = NONE;
    }
    return nodehead;
}

void deleteNode2(node2 **head_ref, node2 *del)
{
  /* base case */
    if(*head_ref == NULL || del == NULL)
        return;

   /* If node to be deleted is head node */
    if(*head_ref == del)
        *head_ref = del->next;

    /* Change next only if node to be deleted is NOT the last node */
    if(del->next != NULL)
        del->next->prev = del->prev;

     /* Change prev only if node to be deleted is NOT the first node */
    if(del->prev != NULL)
        del->prev->next = del->next;

  /* Finally, free the memory occupied by del*/
    free(del);
    return;
}

void update_list2(float movement, node2 **nodehead)
{
    node2 *trav = *nodehead;
    while(trav != NULL)
    {
        if(borders(trav->x, trav->y))
        {
            if(trav->y - 10 < 0)
                trav->y = mapheight - 30;
            else if(trav->y + 25 > mapheight)
                trav->y = 15;


            if(trav->x - 10 < 0)
                trav->x = mapwidth - 30;
            else if(trav->x + 25 > mapwidth)
                trav->x = 15;

        }


        if(trav->collide == BLAST && trav->scale <= 0.6)
        {
            node2 *freenode = trav;
            trav = trav->next;
            deleteNode2(nodehead, freenode);
            score += 75;
            continue;
        }


        if(trav->collide == BLAST && trav->scale >= 1.4)
        {
            trav->scale = 0.5;
            float rotate1 = trav->rotate <= -360 ? 0 : trav->rotate - 10;
            float rotate2 = trav->rotate >= 360 ? 0 : trav->rotate + 10;

            insert_node2(trav->x, trav->y, trav->rotate, trav->scale, *nodehead);
            insert_node2(trav->x, trav->y, rotate1, trav->scale, *nodehead);
            insert_node2(trav->x, trav->y, rotate2, trav->scale, *nodehead);






            node2 *freenode = trav;
            trav = trav->next;
            deleteNode2(nodehead, freenode);
            continue;
        }

        if(trav->collide != NONE)
        {
            if(trav->collide == SHIP)
            {
                lives--;
                trav->collide = NONE;
            }
            else if(trav->collide == BLAST)
            {
                trav->scale = 0.5;
                float rotate1 = trav->rotate <= -360 ? 0 : trav->rotate - 10;
                float rotate2 = trav->rotate >= 360 ? 0 : trav->rotate + 10;

                insert_node2(trav->x, trav->y, rotate1, trav->scale, *nodehead);
                insert_node2(trav->x, trav->y, rotate2, trav->scale, *nodehead);

                node2 *freenode = trav;
                trav = trav->next;
                deleteNode2(nodehead, freenode);
                continue;
            }
        }

        trav->x += round(movement * sin(trav->rotate));
        trav->y -= round(movement * cos(trav->rotate));
        trav = trav->next;
    }
}

void draw_nodes2(node2 *nodehead, ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *image2, float asteroidspeed)
{

    node2 *trav = nodehead;
    while(trav != NULL)
    {
		// if rotation reaches limits reset it to 0
        if(trav->rotspeed + trav->rotate >= 360 || trav->rotspeed + trav->rotate <= -360)
            trav->rotspeed = 0;

        trav->rotspeed += 0.5;


		// draw appropriate asteroid based off scale
        if(trav->scale <= 1.1)
            al_draw_scaled_rotated_bitmap(image, 20, 22.5, trav->x, trav->y, trav->scale, trav->scale, trav->rotate + trav->rotspeed, 0);
        else
            al_draw_scaled_rotated_bitmap(image2, 20, 22.5, trav->x, trav->y, trav->scale, trav->scale, trav->rotate + trav->rotspeed, 0);

        trav = trav->next;
    }
}

void destroy_list2(node2 **nodehead)
{
    node2 *freenode;
    node2 *trav = *nodehead;
    while(trav != NULL)
    {
        freenode = trav;
        trav = trav->next;
        deleteNode2(nodehead, freenode);
    }
}


bool collision(node **blastlist, node2 *asteroidlist, float x, float y, bool *asteroidblasted, bool *shipblasted, effectdata *data)
{

    *asteroidblasted = false;
    *shipblasted = false;


    node *trav = *blastlist;
    node2 *trav2 = asteroidlist;

    bool collision = false;

	// iterate through asteroid list
    while(trav2 != NULL)
    {
		// iterate through ship lazer list
        while(trav != NULL)
        {
			// if lazer is near asteroid set collision to BLAST
            if((abs(trav->x - trav2->x) <= 30 * trav2->scale) && (abs(trav->y - trav2->y) <= 30 * trav2->scale))
            {
               *asteroidblasted = true;


			   data->xcor = trav2->x;
			   data->ycor = trav2->y;
			   data->collide = true;


               trav2->collide = BLAST;

			   // update lazer list and delete lazer node
               node *freenode = trav;
               trav = trav->next;
               deleteNode(blastlist, freenode);
               continue;
            }

			// update lazer list
            trav = trav->next;
        }
		// reset lazer list to head and advance asteroid list
        trav = *blastlist;
        trav2 = trav2->next;
    }

	// set asteroid list to head
    trav2 = asteroidlist;

	// iterate asteroid list
    while(trav2 != NULL)
    {
		// if ship close to asteroid set collision to SHIP and raise flag
        if(abs(x - trav2->x) <= 30 * trav2->scale && abs(y - trav2->y) <= 30 * trav2->scale)
        {
            trav2->collide = SHIP;
            collision = true;

			data->xcor = x;
			data->ycor = y;
			data->collide = true;


            *shipblasted = true;
        }

        trav2 = trav2->next;
    }

    return collision;
}

// should be a multiple of ten
void drawmap(int height, int width, bool is_live[numrows][numcols])
{
    for (int i = 0; i < numrows; i++)
    {
        for(int j = 0; j < numcols; j++)
        {

            if(is_live[i][j])
                al_draw_filled_rectangle(j*10,i*10,(j+1)*10,(i+1)*10,al_map_rgba(0, 15, 30, 255));
        }
    }

    for (int i = 0; i < numrows; i++)
    {
        for (int j = 0; j < numcols; j++)
        {
            bool life;
            int numclose = 0;

            if(j - 1 > 0)
            {
                if(is_live[i][j - 1])
                    numclose++;
            }

            if(j - 1 > 0 && i - 1 > 0)
            {
                if(is_live[i - 1][j - 1])
                    numclose++;
            }
            
            if(i - 1 > 0)
            {
                if(is_live[i - 1][j])
                    numclose++;
            }

            if(j + 1 < numcols)
            {
                if(is_live[i][j + 1])
                    numclose++;
            }

            if(j + 1 < numcols && i + 1 < numrows)
            {
                if(is_live[i + 1][j + 1])
                    numclose++;
            }
            
            if(i + 1 < numrows)
            {
                if(is_live[i + 1][j])
                    numclose++;
            }

            if(i - 1 > 0 && j + 1 < numcols)
            {
                if(is_live[i - 1][j + 1])
                    numclose++;
            }

            if(i + 1 < numrows && j - 1 > 0)
            {
                if(is_live[i + 1][j - 1])
                    numclose++;
            }

            if(is_live[i][j])
            {
                if(numclose < 2)
                    life = false;
                if(numclose == 2 || numclose == 3)
                    life = true;
                if(numclose > 3)
                    life = false;
            }
            else
            {
                if(numclose == 3)
                    life = true;
                else
                    life = false;
            }


            is_live[i][j] = life;
        }
    }

}
