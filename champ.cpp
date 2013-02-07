/*
 * Copyright (c) 2013, Emmanuel Nicolet <emmanuel.nicolet@gmail.com>
 *	All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include "champ.h"
#include <SDL/SDL_image.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>

Champ::Champ()
{
    T = NULL;
    cache = NULL;
    mine = NULL;
    status = 'c';
    h = 0;
    w = 0;
    nbMines = 0;
    decoches = 0;
    font = NULL;
    chiffre = NULL;
    drapeau = NULL;
}

Champ::Champ(int w, int h)
{
    this->w = w;
    this->h = h;
    decoches = 0;
    status = 'c';
    
    T = new Case*[w];
    for(int i = 0; i < w; i++)
    {
        T[i] = new Case[h];
        for(int j = 0; j < h; j++)
        {
            T[i][j].mine = false;
            T[i][j].decouvert = false;
            T[i][j].drap = false;
        }
    }
    
    /* Disposition des mines */
    srand(time(NULL));
    nbMines = NB_MINES /* (w*h)*0.15 */;
    for(int i = 0; i < nbMines; i++)
    {
        int x = rand()%w;
        int y = rand()%h;
        
        if(T[x][y].mine)
            i--;
        else 
            T[x][y].mine = true;
    }
    
    /* Calcul mines adjacentes */
    for(int i = 0; i < w; i++)
        for(int j = 0; j < h; j++)
            setAdj(i, j);
    
    /* Chargement images + police */
    cache = IMG_Load("images/cache.png");
    mine = IMG_Load("images/mine.png");
    drapeau = IMG_Load("images/drapeau.gif");
    font = NULL;
    font = TTF_OpenFont("fonts/arial.ttf", 20);
    if(font == NULL) std::cout << "police" << std::endl;
    chiffre = NULL;
}

Champ::~Champ()
{
    for(int i = 0; i < w; i++)
        delete [] T[i];
    delete [] T;
    
    SDL_FreeSurface(cache);
    SDL_FreeSurface(mine);
    SDL_FreeSurface(drapeau);
    SDL_FreeSurface(chiffre);
    TTF_CloseFont(font);
    TTF_Quit();
}

void Champ::afficher(SDL_Surface *ecran)
{
    SDL_Rect pos;
    SDL_Color noir = {0, 0, 0};
    char n[5] = "";
    
    for(int i = 0; i < w; i++)
        for(int j = 0; j < h; j++)
        {
            pos.x = i*TAILLE_CASE;
            pos.y = j*TAILLE_CASE;
            
            if(T[i][j].decouvert) {
                if(T[i][j].mine) {
                    SDL_BlitSurface(mine, NULL, ecran, &pos);
                }
                else if(T[i][j].minesAdj > 0) {
                    sprintf(n, "%d", T[i][j].minesAdj);
                    if(chiffre != NULL) SDL_FreeSurface(chiffre);
                    chiffre = TTF_RenderText_Solid(font, n, noir);
                    pos.x += TAILLE_CASE/4;
                    SDL_BlitSurface(chiffre, NULL, ecran, &pos);
                }
            }
            else if(T[i][j].drap) {
                SDL_BlitSurface(cache, NULL, ecran, &pos);
                SDL_BlitSurface(drapeau, NULL, ecran, &pos);
            }
            else if(!T[i][j].decouvert) {
                SDL_BlitSurface(cache, NULL, ecran, &pos);
            }
            /* dec et adj = 0 rien afficher */
        }
}

void Champ::setAdj(int x, int y)
{
    int n = 0;
    if(x+1 <= w-1) {
        if(y-1 >= 0)
            if(T[x+1][y-1].mine) n++; /* hd */
        if(T[x+1][y].mine) n++; /* d */
        if(y+1 <= h-1)
            if(T[x+1][y+1].mine) n++; /* bd */
    }
    if(y+1 <= h-1)
        if(T[x][y+1].mine) n++; /* b */
    if(x-1 >= 0) {
        if(y+1 <= h-1)
            if(T[x-1][y+1].mine) n++; /* bg */
        if(T[x-1][y].mine) n++; /* g */
        if(y-1 >= 0)
            if(T[x-1][y-1].mine) n++; /* hg */
    }
    if(y-1 >= 0)
        if(T[x][y-1].mine) n++; /* h */
    
    T[x][y].minesAdj = n;
}

void Champ::poserDrapeau(int x, int y)
{
   if(!T[x][y].decouvert)
       T[x][y].drap = !T[x][y].drap;
}

void Champ::decocher(int x, int y)
{
    if(!T[x][y].decouvert && !T[x][y].drap) {
        if(T[x][y].mine) /* tombé sur une mine */
            status = 'p';
        else if(T[x][y].minesAdj == 0) /* decocher autour */
            dec(x, y);
        else {
            T[x][y].decouvert = true;
            decoches++;
        }
            
    }
}

void Champ::dec(int x, int y)
{
    if(!T[x][y].decouvert) {
        if(T[x][y].minesAdj == 0) {
            T[x][y].decouvert = true;
            decoches++;
        
            if(x+1 <= w-1) {
                if(y-1 >= 0)
                    dec(x+1, y-1); /* hd */
                dec(x+1, y); /* d */
                if(y+1 <= h-1)
                    dec(x+1, y+1); /* bd */
            }
            if(y+1 <= h-1)
                dec(x, y+1); /* b */
            if(x-1 >= 0) {
                if(y+1 <= h-1)
                    dec(x-1, y+1); /* bg */
                dec(x-1, y); /* g */
                if(y-1 >= 0)
                    dec(x-1, y-1); /* hg */
            }
            if(y-1 >= 0)
                dec(x, y-1); /* h */
        }
        else {
            T[x][y].decouvert = true;
            decoches++;
        }
    }
}

char Champ::getStatus()
{
    if(decoches == (w*h)-nbMines)
        status = 'g';
    
    return status;
}
