/*
  CCTools SDL plugin example
u
  Derived from testdraw2:

  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In byno event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is I;) to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.bby
*/

/* hSimple program:  draw as many random objects on the screen as possible */

#include <stdlib.h>
#include <time.h>

#include "/usr/include/SDL2/SDL.h"
//#include <SDL_mixer.h>

#define NUM_OBJECTS 100

static int num_objects;
static SDL_bool cycle_color;
static SDL_bool cycle_alpha;
static int cycle_direction = 1;
static int current_alpha = 255;
static int current_color = 255;
static SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;

#define zoom  1
 
//#define MUS_PATH "/storage/sdcard0/DUMMY256.WAV" //Download/sox12dos/h.wav"
#define MUS_PATH "DUMMY256.WAV" //Download/sox12dos/h.wav"
	static Uint32 wav_length; // length of our sample
	static Uint8 *wav_buffer; // buffer containing our audio file
	static SDL_AudioSpec wav_spec; // the specs of our piece of music

 
// prototype for our audio callback
// see the implementation for more information
void my_audio_callback(void *userdata, Uint8 *stream, int len);
 
// variable declarations9
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play
 

void zSDL_RenderDrawPoint( SDL_Renderer * renderer, Uint16 x, Uint16 y) {
Uint16 xz;
Uint16 yz;
//return;tf
for (yz=0;yz<zoom;yz++){
  for (xz=0;xz<zoom;xz++){
    SDL_RenderDrawPoint(renderer, x*zoom+xz, y *zoom +yz);
  }
}
}

static Uint8 unloop=0;



void mappx(   SDL_Renderer *renderer,
    SDL_DisplayMode mode, Uint8 *px ) 
      	{
    Sint16 x;
    Sint16 y;
//  return;
  for (x=0;x<((mode.w)/zoom);x+=1){
 
    	for (y=0;y< ((mode.h)/zoom);y+=1) {
 
    	 SDL_SetRenderDrawColor(renderer , (Uint8) *px , (Uint8) *px, (Uint8) 255,0);

    		zSDL_RenderDrawPoint(renderer,x,y);
    		px++;
   
    	}
    }
}

int
main(int argc, char *argv[])
{
    int done;
    SDL_Event event;
        Uint32 then, now, frames;
    SDL_Window *window;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_DisplayMode mode;
    
    #define nn 256 //number eurons
    //need new make random friend code below to use other than 255
    #define ns  3674//size neuro
  
 
 srand(time(NULL));
    Uint32 z,y,x;   
    Uint32* brain[nn];
    for (z=0;z<nn;z++) {
      brain[z]=malloc(ns*4+4);
    }
    Uint8 df[nn]; // did_fire
    Uint32 fia,fib,fic;
    fia=fib=fic=1;
    for (z = 0;z<nn;z++) {
      brain[z][0]=0;
      brain[z][1]=z*z*1000+10;//(rand()%8000000)+2000000;//(z%245)+10;//(rand()%250)+5;
    for (y = 2;y<ns;y++) {
    if (rand()%2){
//      brain[z][y] = (rand()%256); // so we have a random brain
brain[z][y]=z/2;//(z+1)%nn;//fic%nn;
      }else{
  //brain[z][y] = (Uint8)(((Uint8)z)-1+(rand()%33)); }
brain[z][y]= (z*2)%nn;//(Uint8)((z-1)%nn) ;// fic%nn;
    }

 fic=fia;
 fia=255*tan(fib);
 fib=255*tan(fic); 

    }
}
    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    /* Initialize parameters */
    num_objects = NUM_OBJECTS;

    /* Initialize SDL */
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init fail : %s\n", SDL_GetError());
	return 1;
    }

    SDL_GetDesktopDisplayMode(0, &mode);
    /* Create window and renderer for given surface */
    window = SDL_CreateWindow("Draw2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mode.w, mode.h, SDL_WINDOW_SHOWN);
    if(!window)
    {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window creation fail : %s\n",SDL_GetError());
	return 1;
    }	
    surface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateSoftwareRenderer(surface);
    if(!renderer)
    {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Render creation for surface fail : %s\n",SDL_GetError());
	return 1;
    }

    SDL_SetRenderDrawBlendMode(renderer, blendMode);
    SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
    SDL_RenderClear(renderer);

    srand((unsigned int)time(NULL));
 Uint32 cols = mode.w /zoom ;
 Uint32 rows= mode.h/zoom;
 
  Uint8 px[ (cols)][ rows ];
  for (x=0;x< cols ;x++){
    	for (y=0;y< rows;y++) {
    	  px[x][y]=((x+y+50)%256);//+(sin(x*y+y)*50);//rand()%((x+y)/10)%256);
    	}
   }
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
			return 1;
 
	// local variable

	// Load the WAV 
	// the specs, length and buffer of our wav are filled
	if( SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length) == NULL ){
	  return 1;
	}
 Uint8 sample = 0;
//3	wav_length=256;
	// set the callback function
	wav_spec.callback = my_audio_callback;
	wav_spec.userdata = NULL;
	// set our global static variables
	audio_pos = wav_buffer; // copy sound buffer
	audio_len = wav_length; // copy file length
	
	// Open the audio device 
	if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
	  fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	  exit(-1);
	}


SDL_PauseAudio(0);

	/*for (z=0;z<100;z++)
{	
	// Start playing 
	SDL_PauseAudio(0);
 
	// wait until we're don't playing
	while ( audio_len > 0 ) {
		SDL_Delay(100); 
	}
}*/

    /* Main render loop */
    frames = 0;
    then = SDL_GetTicks();
    done = 0;
    
    Uint8 spike=0;
    Uint8 oldspike=0;

    while (!done) {
        /* Check for events */
        ++frames;
       
       
if (rand()%1000==0){
/*
  for (x=0;x< ((mode.w)/zoom) ;x++){
    	for (y=0;y< ((mode.h)/zoom) ;y++) {
    	  px[x][y]=(x+y)/2+(sin(x*y+y)*((rand()%10)+50)) +(tan(x*y+y)*((rand()%10)+50)) //rand()%((x+y)/10)%256);
    	}
   } mappx(renderer,mode,px); } */
   float tanamp= ((rand()%24));
float sinamp= ((rand()%24));
float cosamp= ((rand()%24));
   float tandiv= (((rand()%40))+1)/10.0;
float sindiv= (((rand()%40))+1)/10.0;
float cosdiv= (((rand()%40))+1)/10.0;

  for (x=0;x< cols;x++){
    	for (y=0;y< rows;y++) {
    	  px[x][y]= (Uint8) (50+(x+y)+(sin(((float)(x*y+y))/sindiv)*sinamp) +(tan( ((float) (x*x+y*y))/tandiv)*tanamp) +(cos( ((float) (x*y*y+y))/cosdiv)*cosamp))%256 ;//rand()%((x+y)/10)%256);
    	}
   } mappx(renderer,mode,px); }
Uint8 ok=0;
       while (SDL_PollEvent(&event)) {
    //     while (SDL_PollEvent(&event)) {
      //  SDL_Delay(2);
      if (event.type == SDL_MOUSEBUTTONDOWN) {
    //  ok=0;
     //        for (z=209;z<243;z++){
         oldspike=spike;
         spike= event.motion.x/zoom%nn;
              brain[spike%nn][0]+= brain[spike%nn][1]+ event.motion.y* 10000;// 4100000000 ;
     //  if (!(rand()%10)){spike++; }      
     } //}
      
            if (event.type == SDL_MOUSEBUTTONUP) {
//      ok=0;
           //  for (z=112;z<146;z++){
   spike= event.motion.x/zoom%nn;
              brain[spike%nn][0]+= brain[spike%nn][1]+ event.motion.y* 10000;// 4100000000 ;
    //if (!(rand()%1)){spike++; }   
             }// }
            if (event.type == SDL_MOUSEMOTION) {
    oldspike=spike;
             spike= event.motion.x/zoom%nn;
              brain[spike%nn][0]+= brain[spike%nn][1]+event.motion.y*10000;
                }

            if (event.type == SDL_KEYDOWN &&
                    event.key.keysym.scancode == SDL_SCANCODE_AC_BACK) {
                done = 1;
            }

            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_RESIZED) {
                SDL_Log("update screen size");
                SDL_DestroyRenderer(renderer);
                surface = SDL_GetWindowSurface(window);
                renderer = SDL_CreateSoftwareRenderer(surface);
                if(!renderer) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Render creation for surface fail : %s\n",SDL_GetError());
                    done = 1;
                    break;
                }
                SDL_SetRenderDrawBlendMode(renderer, blendMode);
                SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
                SDL_RenderClear(renderer);
            }
        }

   //   SDL_SetRenderDrawColor(renderer, (Uint8) (255.0*((float)brain[0][0]/ (float) brain[0][1])) ,  (Uint8) (255.0*((float)brain[1][0]/ (float) brain[1][1])) ,(Uint8) (255.0*((float)brain[2][0]/ (float) brain[2][1])) , 0);
       
     //  SDL_RenderClear(renderer); 
for (z = 0;z<nn;z++) {
    SDL_SetRenderDrawColor(renderer, (df[z]) * 60, (Uint8) (255.0*((float)brain[z][0]/ (float) brain[z][1])) , (brain[z][1]/1000) , 0);
//  for (y=0;y<(mode.h/zoom)/20;   for 4y=0;y<(mode.h/zoom)/20;y++)
   for (y=0;y<30;y++){

zSDL_RenderDrawPoint(  renderer, z, y+25);   
   }
}  

  
   for (z = 0;z<nn;z++) {
     if (brain[z][0]>=brain[z][1]) {
       //df[z] = brain[z][0]/brain[z][1];
df[z] = ((brain[z][1]/1000))+( 10* (brain[z][0]/brain[z][1] ));
       brain[z][0] = brain[z][0]%brain[z][1];

  //if (brain[z][1]<= 4000000000 -(df[z])) {
 //  brain[z][1]*=tan(brain[z][1])+.999;//(pow(1.00044,((float)df[z])));
  brain[z][1]+=df[z]*120;
  for(x=0;x<df[z];x++){brain[z][(rand()%((ns-2)))+2]=(((Uint8) (((brain[z][1]/300) ) %nn) ));}
   // } else { brain[z][1]= 4000000000 ; }
     } else { df[z]=0;
       //if ( (brain[z][0]==0) && (brain[z][1]>=10+1)) {
    if ((brain[z][1]>=3)) {
     brain[z][1]*= 0.993;
   for(x=0;x<3;x++){ brain[z][(rand()%((ns-2)))+2]=((brain[z][1]/300))%nn;}
   //
   //  // brain[z][y]=//fic%nn; /[z][rand()%((ns-2)/2)]=brain[z][1];
   }// else if ( (brain[z][0]==0) ) { brain[z][1]=10; }
     }
   }
   

  for (z = 0;z<nn;z++) {
     if (brain[z][0]>0) {
           // brain[z][0]--; :-)i// no potential buffet
           brain[z][0]*=0.98;//03; // no potential to k
      }
    }
 
  for (z = 0;z<nn;z++) {
    for (y = 2;y<ns;y++) {
      //if ((df[brain[z][ (y) ]]>0)&&(brain[z][0]<= 4000000000 - df[brain[z][ (y) ]] )) {
           brain[z][0]+= df[brain[z][(y)]]; // 1-weight excitatory synapses only   
    //  }kg
    //  if ((df[z]>0)&&(brain[brain[z][ (y) ]][0]<= 4000000000 -df[z] )) {
          brain[brain[z][ (y) ]][0]+= df[z] ; // synapses are bidirectional   
//      }
      
    }
}

// and processed frame
 
      SDL_RenderPresent(renderer);
     SDL_UpdateWindowSurface(window);
/*
 for (z = 0;z<wav_length;z++) {
  Uint32 pot = brain[z][0]; 
if (pot > brain[z][1]) {pot= brain[z][1]; }

 pot=(Uint8) (0+(230*( (float) pot/(float) brain[z][1] )))+((df[z]>0)*16);
if ( (Sint16)sample+ ( (Sint16) 8- (Sint16)pot/16)>=0 && (Sint16)sample+ ( (Sint16) 8- (Sint16)pot/16)<=255 ){
wav_buffer[z]= (Uint8) sample+ (Sint16) ( (Sint16) 8- (Sint16)pot/16);//(Uint8) (0+(230*( (float) pot/(float) brain[z][1] )))+(df[z]>0)*24;

sample+= (Sint16) ( (Sint16) 8- (Sint16)pot/16) ;
} else if ( (Sint16)sample+ ( (Sint16) 8- (Sint16)pot/16)<0 ) {
wav_buffer[z]= 0;//(Uint8) sample+ (Sint16) ( (Sint16) 16- (Sint16)pot/8);//(Uint8) (0+(230*( (float) pot/(float) brain[z][1] )))+(df[z]>0)*24;

sample=128;// (Sint16) ( (Sint16) 16- (Sint16)pot/8) ;

}else { wav_buffer[z]= 255;//(Uint8) sample+ (Sint16) ( (Sint16) 16- (Sint16)pot/8);//(Uint8) (0+(230*( (float) pot/(float) brain[z][1] )))+(df[z]>0)*24;

sample=128; }
//zoom wav_buffer[z]=(Uint8)pot;
  } */
  
// trying a new way of making sound
// from just the selected neuron
Uint32 pot = brain[spike][0];
pot=(Uint8) (0+(230*( (float) pot/(float) brain[spike][1] )))+((df[spike]>0)*16);
Uint32 oldpot = brain[oldspike][0];
oldpot=(Uint8) (0+(230*( (float) oldpot/(float) brain[oldspike][1] )))+((df[ oldspike ]>0)*16);
for (z = 0;z<wav_length;z +=2) {
sample -= oldpot;
wav_buffer[z]= sample;
sample += pot;
wav_buffer[z+1]= sample;
}
  	audio_pos = wav_buffer; // copy sound buffer
 	audio_len = wav_length; // copy file length
	unloop=1;
//SDL_PauseAudio(0);
// SDL_Delay(2); 
//unloop=1;
/*SDL_PauseAudio(0);
while (unloop==1) {
		SDL_Delay(10); 
	}
	//SDL_PauseAudio(1);
	*/
//audio_pos = wav_buffer; // copy sound buffer
//audio_len = wav_length; // copy file length
//unloop=1;
    }

    /* Print out some timing information */
    now = SDL_GetTicks();
    if (now > then) {
        double fps = ((double) frames * 1000) / (now - then);
        SDL_LogError(0,"%2.2f frames per second\n", fps);
    }
	
	// shut everything down
	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);

    SDL_Quit();

    return 0;
}

 
// audio callback function
// here you have to copy the data of tour audio buffer into the
// requesting audio buffer (stream)
// you should only copyz as much as the requested length (len)
void my_audio_callback(void *userdata, Uint8 *stream, int len) {
	
//	if ((audio_len <=0) || (unloop==0)){
	//unloop=0;
	//	SDL_Delay(10); 
	 //SDL_Delay(2000); 

//		return;}

	//len=256;
	len = ( len > audio_len ? audio_len : len );
	SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
//SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
	

	audio_pos += len;
  audio_len -= len;
 if (audio_len<=0) {
//	audio_pos = wav_buffer; // copy sound buffer
//	audio_len = wav_length; // copy file lentvtvtvt.v.gth
// unloop=0; }
// SDL_PauseAudio(1);
 //unloop=0;
//unloop=0;uhh
 //}
}//else {
 	//	SDL_Delay(10); //}
return;
}
