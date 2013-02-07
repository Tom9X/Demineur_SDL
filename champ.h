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

#ifndef CHAMP_H
#define CHAMP_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define NB_MINES 50
#define TAILLE_CASE 30
#define LARGEUR   25
#define HAUTEUR   15


typedef struct Case
{
    int minesAdj;
    bool mine;
    bool decouvert;
    bool drap;
}Case;

class Champ 
{
public:
    Champ();
    Champ(int w, int h);
    ~Champ();
    
    void afficher(SDL_Surface *ecran);
    void setAdj(int x, int y);
    void poserDrapeau(int x, int Y);
    void decocher(int x, int y);
    void dec(int x, int y); /* recursif */
    char getStatus();
    
private:
    Case **T;
    SDL_Surface *cache;
    SDL_Surface *mine;
    SDL_Surface *chiffre;
    SDL_Surface *drapeau;
    char status; /* c = en cours, p = perdu, g = gagné */
    int w;
    int h;
    int nbMines;
    int decoches;
    TTF_Font *font;
};

#endif
