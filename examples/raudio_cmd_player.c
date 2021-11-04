/*******************************************************************************************
*
*   raudio example - Using raudio library to play some audios
*
*   NOTE: This example does not require any graphic device, it runs directly on the console.
*
*   DEPENDENCIES:
*       raudio - A simple and easy-to-use audio library based on miniaudio
*
*   COMPILATION:
*       gcc -o raudio_cmd_player.exe raudio_cmd_player.c ..\src\raudio.c /
*           -I..\src -I..\src\external -L. -Wall -std=c99  /
*           -DRAUDIO_STANDALONE -DSUPPORT_FILEFORMAT_WAV -DSUPPORT_FILEFORMAT_OGG
*
*   LICENSE: zlib/libpng
*
*   This example is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   Copyright (c) 2014-2019 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
********************************************************************************************/

#include "raudio.h"             // raudio library

#include <stdio.h>              // Required for: printf()
#include <time.h>               // Required for: clock()

#if defined(_WIN32)
    #include <conio.h>          // Windows only, no stardard library
#else
    // Required for kbhit() function in non-Windows platforms
    #include <stdio.h>
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

#define KEY_ESCAPE  27

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void WaitTime(int ms);       // Simple time wait in milliseconds
static void PlayWaveCLI(Wave wave); // Play provided wave through CLI
#if !defined(_WIN32)
static int kbhit(void);             // Check if a key has been pressed
static char getch();                // Get pressed character
#endif

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Play audio file if provided (argv[0] is the program itself)
    if (argc > 1)
    {
        Wave wave = LoadWave(argv[1]);  // Load audio (WAV, OGG, FLAC, MP3)
        PlayWaveCLI(wave);              // Play loaded audio
        UnloadWave(wave);               // Unload audio data
    }

    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Simple time wait in milliseconds
static void WaitTime(int ms)
{
    if (ms > 0)
    {
        int currentTime = clock()*1000/CLOCKS_PER_SEC;  // Current time in milliseconds
        int totalTime = currentTime + ms;               // Total required time in ms to return from this timeout

        int percent = 0;
        int prevPercent = percent;

        // Wait until current ms time matches total ms time
        while (currentTime <= totalTime)
        {
            // Check for key pressed to stop playing
            if (kbhit())
            {
                int key = getch();
                if ((key == 13) || (key == 27)) break;    // KEY_ENTER || KEY_ESCAPE
            }

            currentTime = clock()*1000/CLOCKS_PER_SEC;

            // Print console time bar
            percent = (int)(((float)currentTime/totalTime)*100.0f);

            if (percent != prevPercent)
            {
                printf("\r[");
                for (int j = 0; j < 50; j++)
                {
                    if (j < percent/2) printf("=");
                    else printf(" ");
                }
                printf("] [%02i%%]", percent);

                prevPercent = percent;
            }
        }

        printf("\n\n");
    }
}

// Play provided wave through CLI
static void PlayWaveCLI(Wave wave)
{
    float waveTimeMs = (float)wave.frameCount*1000.0/(wave.sampleRate*wave.channels);

    InitAudioDevice();                  // Init audio device
    Sound fx = LoadSoundFromWave(wave); // Load audio wave

    printf("\n//////////////////////////////////////////////////////////////////////////////////\n");
    printf("//                                                                              //\n");
    printf("// raudio CLI audio player                                                      //\n");
    printf("//                                                                              //\n");
    printf("// more info and bugs-report: github.com/raysan5/raudio                         //\n");
    printf("//                                                                              //\n");
    printf("// Copyright (c) 2019 Ramon Santamaria (@raysan5)                               //\n");
    printf("//                                                                              //\n");
    printf("//////////////////////////////////////////////////////////////////////////////////\n\n");

    printf("Playing sound [%.2f sec.]. Press ENTER to finish.\n", waveTimeMs/1000.0f);

    PlaySound(fx);                      // Play sound
    WaitTime(waveTimeMs);               // Wait while audio is playing
    UnloadSound(fx);                    // Unload sound data
    CloseAudioDevice();                 // Close audio device
}

#if !defined(_WIN32)
// Check if a key has been pressed
static int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

// Get pressed character
static char getch() { return getchar(); }
#endif
