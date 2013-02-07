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

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <iostream>
#include "champ.h"

int main(int argc, char **argv)
{
    bool continuer = true;
    SDL_Event event;
    
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    Champ jeu(LARGEUR, HAUTEUR);

    SDL_Surface *ecran = SDL_SetVideoMode(LARGEUR*TAILLE_CASE, HAUTEUR*TAILLE_CASE, 32, SDL_HWSURFACE);
    SDL_WM_SetCaption("Demineur_SDL", NULL);

    while(jeu.getStatus() == 'c' && continuer)
    {
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT)
            continuer = false;
        if(event.type == SDL_MOUSEBUTTONDOWN)
            if(event.button.button == SDL_BUTTON_LEFT)
                jeu.decocher(event.button.x/TAILLE_CASE, event.button.y/TAILLE_CASE);
            else if(event.button.button ==SDL_BUTTON_RIGHT)
                jeu.poserDrapeau(event.button.x/TAILLE_CASE, event.button.y/TAILLE_CASE);
        
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 128, 128, 128));
        jeu.afficher(ecran);
        SDL_Flip(ecran);
    }
    
    if(jeu.getStatus() == 'g') std::cout << "Gagné !" << std::endl;
    else std::cout << "Perdu !" << std::endl;

    //TTF_Quit(); appellé dans ~Champ()
    SDL_Quit();
    return EXIT_SUCCESS;
}
