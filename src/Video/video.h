#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "sdlogv/theora.h"
#include "graphics.h"

namespace Video {
    struct Video {
        FILE *f;
        SDL_Texture *texture = NULL;
        SDL_FRect rect;                  /* the rectangle where we want to display the video */
    };

    FILE *f;
    Video video;
    bool playingVideo = false;
    theora_t ctx = {0};

    static void callback(int channel) {
        Mix_Chunk *audio = theora_audio(&ctx);
        /* could we get some audio data? If so, play it! */
        if (audio)
            Mix_PlayChannel(channel, audio, 0);
    }

    void Play_Audio() {
        if (ctx.hasAudio) {
            Mix_ChannelFinished(callback);
            callback(0);
        }
    }

/* just sets a flag */
    int got_sigint = 0;

    static void sigint_handler(int signal) { got_sigint = signal; }

/**
 * Example on how to use the wrapper
 */

    void Close_Video(Video &video) {
        /* the theora thread has absolutely no clue about the SDL mixer usings its buffers, so we MUST stop the mixer first. */
        if (Mix_Playing(-1)) {
            Mix_ChannelFinished(NULL);
            Mix_HaltChannel(-1);
        }
        /* ok, now that nobody is relying on the audio buffer, we can stop the decoder and destroy the context */
        theora_stop(&ctx);
        /* as usual, free SDL resources */
        printf("Tear down\n");
        fclose(video.f);
        if (video.texture) SDL_DestroyTexture(video.texture);
//        Mix_CloseAudio();
//        Mix_Quit();
        playingVideo = false;
    }

    SDL_FRect Size_Video(Video video) {
        SDL_DisplayMode dm;
        SDL_GetWindowDisplayMode(Graphics::window, &dm);

        video.rect.w = dm.w;
        video.rect.h = ctx.h * dm.w / ctx.w;
        if (video.rect.h > dm.h) {
            video.rect.h = dm.h;
            video.rect.w = ctx.w * dm.h / ctx.h;
        }
        video.rect.x = (dm.w - video.rect.w) / 2;
        video.rect.y = (dm.h - video.rect.h) / 2;
        return video.rect;
    }

    Video Init_Video(const char *file, Video &video) {
        playingVideo = true;
        SDL_DisplayMode dm;
        /* check arguments */
        if (file == NULL) {
            printf("runVideo() no proper file passed");
            return video;
        }
        /* open the video file */
        video.f = fopen(file, "rb");
        if (!video.f) {
            fprintf(stderr, "unable to open file\n");
            return video;
        }
        /************************************************************************/
        /***                  get the duration in millisec                    ***/
        /************************************************************************/

        printf("Duration: %lu msec\n", theora_getduration(video.f));

        /************************************************************************/
        /***                       start the decoder                          ***/
        /************************************************************************/
        /* yeah, really this simple. */

        theora_start(&ctx, video.f);

        /* this is going to be extremely simple, because we're using SDL mixer to play it in the background */
        Play_Audio();

        /* Is there a video stream in the file? If so, open a window and create a texture where we will paint the video */
        if (ctx.hasVideo) {
            video.texture = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, ctx.w, ctx.h);
            Size_Video(video);
            /* the decoder was kind enough to tell us the video dimensions in advance */

        } else {
            /* in lack of a window, SDL can't handle keypress events. */
            signal(SIGINT, sigint_handler);
        }

        return video;
    }

    bool Play_Video(Video &video) {
        if (theora_playing(&ctx) && !got_sigint) {
            printf("Playing started...\n");
            theora_video(&ctx, video.texture);
            SDL_FRect rect = Size_Video(video);
            SDL_RenderCopyF(Graphics::renderer, video.texture, NULL, &rect);
            printf("Finished playing\n");
            return true;
        } else {
            return false;
        }
    }

// runs in the game loop
    void Run_Video(const char *file, bool skip) {
        if (playingVideo == false) {
            video = Init_Video(file, video);
        }
        if (playingVideo == true) {
            if (Play_Video(video) == false || skip == true) {
                Close_Video(video);
            }
        }
    }

    void Run_Audio(const char *file) {
        playingVideo = true;
        SDL_DisplayMode dm;
        /* open the video file */
        f = fopen(file, "rb");
        if (!f) {
            fprintf(stderr, "unable to open file\n");
        }
        /************************************************************************/
        /***                  get the duration in millisec                    ***/
        /************************************************************************/

        printf("Duration: %lu msec\n", theora_getduration(f));

        /************************************************************************/
        /***                       start the decoder                          ***/
        /************************************************************************/
        /* yeah, really this simple. */

        theora_start(&ctx, f);

        /* this is going to be extremely simple, because we're using SDL mixer to play it in the background */
        if (ctx.hasAudio) {
            Play_Audio();
        } else {
            /* in lack of a window, SDL can't handle keypress events. */
            signal(SIGINT, sigint_handler);
        }
    }
}


