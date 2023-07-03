#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "sdlogv/theora.h"


/************************************************************************/
/***                          audio player                            ***/
/************************************************************************/
/* this is going to be extremely simple, because we're using SDL mixer to play it in the background */

namespace Audio {

//    bool playingVideo = false;
//    FILE *f;
//    theora_t ctx = {0};
//    int got_sigint = 0;
//
//    static void sigint_handler(int signal) { got_sigint = signal; }
//
//    static void callback(int channel) {
//        Mix_Chunk *audio = theora_audio(&ctx);
//        /* could we get some audio data? If so, play it! */
//        if (audio)
//            Mix_PlayChannel(channel, audio, 0);
//    }
//
//    void Run_Audio() {
//        if (ctx.hasAudio) {
//            Mix_ChannelFinished(callback);
//            callback(0);
//        }
//    }
//
//
//    void Init_Audio(const char *file) {
//        playingVideo = true;
//        SDL_DisplayMode dm;
//        /* open the video file */
//        f = fopen(file, "rb");
//        if (!f) {
//            fprintf(stderr, "unable to open file\n");
//        }
//        /************************************************************************/
//        /***                  get the duration in millisec                    ***/
//        /************************************************************************/
//
//        printf("Duration: %lu msec\n", theora_getduration(f));
//
//        /************************************************************************/
//        /***                       start the decoder                          ***/
//        /************************************************************************/
//        /* yeah, really this simple. */
//
//        theora_start(&ctx, f);
//
//        /* this is going to be extremely simple, because we're using SDL mixer to play it in the background */
//        if (ctx.hasAudio) {
//            Run_Audio();
//        } else {
//            /* in lack of a window, SDL can't handle keypress events. */
//            signal(SIGINT, sigint_handler);
//        }
//    }
}