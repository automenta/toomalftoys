/*
  CCTools SDL plugin example

  Derived from testdraw2:

  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is prodvided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.
byc.f.
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely.
*/
/* Simple program:  draw as many rando
m objects on the screen as possible */

#include <stdlib.h>
//#include <stdio.h>
#include <time.h>

//#include <SDL.h>
#include "/usr/include/SDL2/SDL.h"
  

  #define game 1  
  /*
  //int game 0;
  #define game 0
  #define level 0
#define zoom 3U
    #define popsiz 5
      #define bps 5
      #define ltx 10 //life tax
#define nugen 10// eextent of directions array, distance array, and relative offsets array
    #define blownupsandreward 33
    #define grains 200.  */

#ifdef game
Uint8 popsiz, bps, level;
Uint8 zoom, ltx, targetface;
Uint16 nugen, blownupsandreward, grains, timelimit;
Uint32 scoregoal;
#endif 

static SDL_BlendMode blendMode = SDL_BLENDMODE_NONE;


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

void mappx(   SDL_Renderer *renderer,
    SDL_DisplayMode mode, Uint8 *px ) 
    {
    	
    Sint16 x;
    Sint16 y;
//  return;
  for (x=0;x<((mode.w)/zoom)-1;x+=1){
 
    	for (y=0;y< ((mode.h)/zoom)-1;y+=1) {
 
    	 SDL_SetRenderDrawColor(renderer , (Uint8) *px , (Uint8) *px, (Uint8) 255,0);

    		zSDL_RenderDrawPoint(renderer,x,y);
    		px++;
   
    	}
    }
}

Uint16 objat(Sint32 who,Uint16 x, Uint16 y, Uint16 wormx[popsiz][bps], Uint16 wormy[popsiz][bps], Uint16 sand[grains][3], Uint16 curgra)
{// also mak sand use "who,"
 	if (curgra>popsiz*bps)   {
 		Uint16 g;
 		for (g=0;g<curgra;g++) {
 		  if (sand[g][0]==x) {
 		   if (sand[g][1]==y) {
 		     return popsiz+1+g;
 		   }
 			}
 		}
 		 		 		
 		 		Uint16 h;
 		 		for (g=0;g<popsiz;g++){

 		 		for (h=0;h<bps;h++){
 		 		  if ((wormx[g][h]==x) &&  (wormy[g][h]==y) && (g!=who)) {
 	           return g;
 		 		  	}
 		 		  }} 		
 		 		
 		 		return popsiz ; //?7;
 	} else {
 		
 		
 		 		Uint16 g;
 		 		Uint16 h;
 		 		for (g=0;g<popsiz;g++){

 		 		for (h=0;h<bps;h++){
 		 		  if ((wormx[g][h]==x) &&  (wormy[g][h]==y) && (g!=who )) {
 	           return g;
 		 		  	}
 		 		  }} 		
 		 		  
 		 		
 		for (g=0;g<curgra;g++) {
 		  if (sand[g][0]==x) {
 		   if (sand[g][1]==y) {
 		     return popsiz+1+g; // $@#&
 		   }
 			}
 		}
 		return popsiz; //£|\|\/°7¥!
 	}
}



// coud finished backsand ga"l ffurtu
//"when they die just a pieces and let it fall through
//I just if they died just something that the dog can eclipse to the bodies exhibit a bit



//#undef main
int
main(int argc, char *argv[])
{
    int done;
    SDL_Event event;
    Uint32 then, now, frames;
    SDL_Window *window;     SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_DisplayMode mode;
zoom=3;

//SDL_ShowSimpleMessageBox(0, "DRBUGgenetic worms & fallin' sand game", "BUG game mode, robots gain in strength and numbers and will try to hole your tables and cups. the goal is to keep the sand near one side of the playing field", window);


    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    /* Initialize parameters */
  //  num_objects = NUM_OBJECTS;

    /* Initialize SDL */
    if( SDL_Init (SDL_INIT_VIDEO) != 0)
    {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init fail : %s\n", SDL_GetError());
	return 1;
    }

    SDL_GetDesktopDisplayMode(0, &mode);
Uint16 cols= ((mode.w)/zoom)-1;
Uint16 rows= ((mode.h)/zoom)-1;

    Uint16 x;
    Uint16 y;
    Uint16 z;
    
    Uint32 gt; //game time
    Uint32 gs;
    Uint8 win;
    

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


#ifdef game
level=0;
zoom = 3+((level%3)*2);
cols= ((mode.w)/zoom)-1;
rows= ((mode.h)/zoom)-1;

popsiz=3*(level+1);
bps= 3*(level+1);
ltx=10-(level%10);
nugen=10*(level+1);
blownupsandreward=33*(level+1);
grains=100*(level+1);
timelimit=1000+200*(level+1);
scoregoal=7000000*(level+1);
targetface=level%4; // 0 top 1 bottom 2 west 3 east
#endif 
while ((game==0)||((game==1)&&(level < 31))) {
    Uint8 px[ (cols)][ rows ];
  for (x=0;x< cols ;x++){
    	for (y=0;y< rows;y++) {
    	  px[x][y]=((x+y+50)%256);//+(sin(x*y+y)*50);//rand()%((x+y)/10)%256);
    	}
   }

gt=0;
gs=0;
    SDL_SetRenderDrawBlendMode(renderer, blendMode);
    SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
    SDL_RenderClear(renderer);
    mappx(renderer,mode,px);

    srand((unsigned int)time(NULL));

    /* Main render loop */
    frames = 0;
    then = SDL_GetTicks();
    done = 0;
    float maxsandage=3.001;
    Uint16 curgra = 0;
    Uint16 snd[grains+1][3];
    Sint16 X[popsiz][bps];
    Sint16 Y[popsiz][bps];
    Uint8 dead[popsiz];
    Uint16 life[popsiz];
    Uint8 topx[popsiz];

    Uint16 ip=0;
    Uint8 pen=0;
       
    //+Uint8 [nugen];
    Uint8 dirs[popsiz][nugen];
    Uint8 dists[popsiz][nugen];
    Uint16 relofs[popsiz][nugen];
    for (z=0;z<popsiz;z++){
      dead[z]=0;
      life[z]=500;
      X[z][0]=rand()%(cols-1);
      Y[z][0]=rand()%(rows-1) ;
      topx[z]=(rand()%26)*10;
        
         	for (y=0;y<nugen;y++) {
         	 Uint8 same=rand()%256;
        	 dirs[z][y]=dists[z][y]=relofs[z][y]=same;
         	}
    	for (y=0;y<bps;y++) {
    		X[z][y]=X[z][0];
    		Y[z][y]=Y[z][0];
    	}
    }
    
   if (game) {
SDL_ShowSimpleMessageBox(0, "genetic worms & fallin' sand game", "in game mode, robots gain in strength and numbers and will try to hole your tables and cups. the goal is to keep the sand near one side of the playing field", window);
}
 
    
    while (!done) {
    
    
    //game code
for (z=0;z<curgra;z++){
gs+=snd[z][2];
}    
    
    
    
    gt++;
    Uint16 step=(mode.w)/100;
    Uint8 steps=100;
    float adiv = (float)gt/(float)timelimit;
    steps *= adiv; z=100*step;
     for (y=0;y<step;y++) {
      SDL_SetRenderDrawColor(renderer, (Uint8) 0 , (Uint8) 233 ,
                             44, (Uint8) 120 );

    		 SDL_RenderDrawPoint(renderer, z+y, mode.h - 3);


    }
    for (z=0;z<steps*step;z+=step) {
      for (y=0;y<step;y++) {
      SDL_SetRenderDrawColor(renderer, (Uint8) 0 , (Uint8) 222 ,
                             33, (Uint8) 100 );

    		 SDL_RenderDrawPoint(renderer, z+y, mode.h - 3);


    }} 
    if (gt==timelimit) { done=1; win=0;}
    if (gs>=scoregoal) {win=1;done=1;}


    step=(mode.h)/100;
    steps=100;
    adiv = (float)(gs % scoregoal) /(float)scoregoal;
    steps *= adiv; z=99*step;
     for (y=0;y<step;y++) {
      SDL_SetRenderDrawColor(renderer, (Uint8) 233, (Uint8)0,
                             44, (Uint8) 120 );

    		 SDL_RenderDrawPoint(renderer, mode.w - 2 , (mode.h-1)-( z+y ));


    }
    for (z=0;z<steps*step;z+=step) {
      for (y=0;y<step;y++) {
      SDL_SetRenderDrawColor(renderer, (Uint8) 222 , (Uint8) 44,
                             33, (Uint8) 00 );

    		 SDL_RenderDrawPoint(renderer, mode.w - 2 , (mode.h-1)-(z+y % (step*99)));


    }} 

   for (y=0;y<step;y++) {
        SDL_SetRenderDrawColor(renderer, (Uint8) 0, (Uint8) 50,
                             184, (Uint8) 0 );

    		 SDL_RenderDrawPoint(renderer, mode.w - 2 , (mode.h-1)-( z+y % (step*99)));
}


if (rand()%1000==0){
/*
  for (x=0;x< ((mode.w)/zoom) ;x++){
    	for (y=0;y< ((mode.h)/zoom) ;y++) {
    	  px[x][y]=(x+y)/2+(sin(x*y+y)*((rand()%10)+50)) +(tan(x*y+y)*((rand()%10)+50)) //rand()%((x+y)/10)%256);
    	}
   } mappx(renderer,mode,px); } */
   Uint8 tanamp= ((rand()%24));
Uint8 sinamp= ((rand()%24));
Uint8 cosamp= ((rand()%24));

  for (x=0;x< cols;x++){
    	for (y=0;y< rows;y++) {
    	  px[x][y]= (Uint16) (50+(x+y)+(sin(x*y+y)*sinamp) +(tan(x*x+y*y)*tanamp) +(cos(y*y+y)*cosamp))%256 ;//rand()%((x+y)/10)%256);
    	}
   } mappx(renderer,mode,px); }
        /* Check for events */
       ++frames;
        while (SDL_PollEvent(&event)) {
        SDL_Delay(2);
            if (event.type == SDL_MOUSEBUTTONUP) {
              if (pen==0) { pen=200+(rand()%56); }
              else        { pen=0; }
              // SDL_ShowSimpleMessageBox(0, "Mouse", "Some other button was pressed!", window);
            }
            if (event.type == SDL_MOUSEMOTION) {
              // draw on background
             SDL_SetRenderDrawColor(renderer, (Uint8) pen, (Uint8) pen ,
                             255, (Uint8)  pen);

Uint16 left=event.motion.x/zoom;//up,right,down;
Uint16 right=event.motion.x /zoom ;
if (event.motion.x/zoom>0) { left--; }
if (event.motion.x/zoom<cols-2) { right++; }
Uint16 up=event.motion.y /zoom ;//up,right,down;
Uint16 down=event.motion.y /zoom ;
if (event.motion.y/zoom>0) { up--; }
if (event.motion.y/zoom<rows-2) { down++; }
for (x=left;x<=right;x++){
if (x>=cols-2) {break;}
for (y=up;y<=down;y++) {
if (y>=rows-2) {break;}
              px[x][y]=pen;
     
    		 zSDL_RenderDrawPoint(renderer, x ,y);
}}
            }
        
            if ((event.type == SDL_QUIT)||(event.type == SDL_KEYDOWN &&
                    event.key.keysym.scancode == SDL_SCANCODE_AC_BACK)) {
                done = 2;
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

Uint8 tanamp= ((rand()%24));
Uint8 sinamp= ((rand()%24));
Uint8 cosamp= ((rand()%24));

  for (x=0;x< cols ;x++){
    	for (y=0;y< rows;y++) {
  	  px[x][y]=(Uint16)(50+(x+y)+(sin(x*y+y)*sinamp) +(tan(x*x+y*y)*tanamp) +(cos(y*y+y)*cosamp))%256 ;//rand()%((x+y)/10)%256);
      	}
   } mappx(renderer,mode,px); 
   
            }
         }

     //   
   // SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xaF);
    //  dd
  //  SDL_RenderClear(renderer);



       /* DrawRects(renderer);
     
     
     
     
        DrawLines(renderer);
        DrawPoints(th); */
   while (((curgra<grains-1) && (rand()>470000000)) )  {  
snd[curgra][0]  = random() % (cols-1);
snd[curgra][1]  = random() % (rows /4);
snd[curgra][2]  = 0; // age  
curgra++;
	
}
for (z=0;z<curgra;z++){
	Uint8 onwrm=0;
for (x=0;x<popsiz;x++) {
	for (y=0;y<bps;y++) {
		if ( (X[x][y]==snd[z][0])&& (Y[x][y]-1==snd[z][1])){
	onwrm=1;	
  snd[z][2]+=1;
  
  if (snd[z][2]>=maxsandage) {
  snd[z][2]+=1;
  maxsandage += 0.001f; // ratchetoh
  SDL_SetRenderDrawColor(renderer, (Uint8) px[snd[z][0]][snd[z][1]] , (Uint8) px[snd[z][0]][snd[z][1]] ,
                             255, (Uint8) px[snd[z][0]][snd[z][1]] );

    		 zSDL_RenderDrawPoint(renderer, snd[z][0], snd[z][1]);

  	 snd[z][0]= rand() % (cols-1);
				snd[z][1]=rand()% (rows/4);
				 snd[z][2]=0;
				 if ((Uint16)life[x]<1000-blownupsandreward){life[x]+=blownupsandreward;}
				 else {life[x]=1000;}
 
 }
	}}}
		 if (!onwrm) {
		for (x=0;x<curgra;x++){
			if ((snd[z][0]==snd[x][0]&&snd[z][1]==snd[x][1]-1)||(px[ snd[z][0] ][ snd[z][1] +1]<4))
			{ if ((snd[z][0])&&( snd[z][1]< (rows -1))&&(objat(-1,snd[z][0]-1,snd[z][1]+1,X,Y,snd,curgra)==popsiz &&(px[ snd[z][0]-1][ snd[z][1]+1]>3) )) { // .pops is here but I'm by all jacked is the code for // returned by aube jacket here is the code for no sand and no animals
			 SDL_SetRenderDrawColor(renderer, (Uint8) px[snd[z][0]][snd[z][1]] , (Uint8) px[snd[z][0]][snd[z][1]] ,
                             255, (Uint8) px[snd[z][0]][snd[z][1]] );

	 zSDL_RenderDrawPoint(renderer, snd[z][0], snd[z][1]);

			 snd[z][0]--; // space
			   snd[z][1]++;
			    onwrm=1;
				} else if (( (snd[z][0]< cols-2 )&&(snd[z][1]< (rows -1)))&&(objat( -1, snd[z][0]+1,snd[z][1]+1,X,Y,snd,curgra)==popsiz &&(px[ snd[z][0]+1 ][ snd[z][1]+1 ]>3) ) ){
			// yeah that's cool and that's all b.efore I start digesting it just for being over another sand
SDL_SetRenderDrawColor(renderer, (Uint8) px[snd[z][0]][snd[z][1]] , (Uint8) px[snd[z][0]][snd[z][1]] ,
                             255, (Uint8) px[snd[z][0]][snd[z][1]] );

	 zSDL_RenderDrawPoint(renderer, snd[z][0], snd[z][1]);

			   snd[z][0]++;
			    snd[z][1]++;
			    onwrm=1;
			}else {
				
	//		if (1) {
				onwrm=1;
			 snd[z][2]+=1; 	
				     
		
		}break;}}}
		if (!onwrm) {
			// zzzzfall
		// snd[z][2]=0; 
			SDL_SetRenderDrawColor(renderer, (Uint8) px[snd[z][0]][snd[z][1]] , (Uint8) px[snd[z][0]][snd[z][1]] ,
                             255, (Uint8) px[snd[z][0]][snd[z][1]] );

	 zSDL_RenderDrawPoint(renderer, snd[z][0], snd[z][1]);

			snd[z][1]+=1;
			if (snd[z][1]>= (rows-1) ){
			 snd[z][0]= rand() % (cols-1 ) ;
 				snd[z][1]=rand() % (rows /4);
 				 snd[z][2]=0;
 						}
		 SDL_SetRenderDrawColor(renderer, (Uint8)255, 128+(Uint8) rand()%128 ,
                               0, (Uint8) rand()%256 );
  
    		 zSDL_RenderDrawPoint(renderer, snd[z][0], snd[z][1]);

		
	}else{ 	 SDL_SetRenderDrawColor(renderer, (Uint8)100, (Uint8) rand()%256 ,
                              0, (Uint8) rand()%256 );
    		 zSDL_RenderDrawPoint(renderer, snd[z][0], snd[z][1]);
}}

    // walk the worms
 //return 0;//}}else{return 2;}
    //Xllb
       
 for (z=0;z<popsiz;z++)     { 
   if (life[z]>=ltx)   {
    life[z ]-=ltx;
Uint8 dir = dirs[z][ip]%4;//rand()%4;
Uint8 dist=(dists[z][(ip+relofs[z][ip])%nugen])+1;
//SDL_Delay(. 01);
  for (x=0;x<(dist%(3+(Uint8)(life[z]/60)))+1;x++){
       if (dir==0){X[z][0]-=1;
       	} else if (dir==1) {  Y[z][0]-=1;  		} else if (dir==2) { X[z][0]+=1;
       	} else if (dir==3) {  Y[z][0]+=1;
       		}
    		
       	if (X[z][0]<1) X[z][0]=1;
      	if (Y[z][0]<1) Y[z][0]=1;
      	if (X[z][0]>= cols-1) X[z][0]= cols-2;
      	if (Y[z][0]>= rows-1) Y[z][0]= rows-2;

    		 SDL_SetRenderDrawColor(renderer,px[X[z][bps-1]][Y[z][bps-1]] , px[X[z][bps-1]][Y[z][bps-1]] ,
                              255, (Uint8) 0 );
    		 zSDL_RenderDrawPoint(renderer, X[z][bps - 1], Y[z][bps-1]);

    	for (y=bps-2;y>0;y--) {
	 SDL_SetRenderDrawColor(renderer,px[X[z][bps-1]][Y[z][bps-1]] , px[X[z][bps-1]][Y[z][bps-1]] ,
                              255, (Uint8) 0 );
    		 zSDL_RenderDrawPoint(renderer, X[z][bps - 1], Y[z][bps-1]);


       	 SDL_SetRenderDrawColor(renderer, (Uint8) 255-y, (Uint8)( life[z]/4.0),
                              y, 0);
    		 zSDL_RenderDrawPoint(renderer, X[z][y] , Y[z][y]);    	
    
       	 	  X[z][y+1]=(Uint16)X[z][y];
       	 Y[z][y+1]=(Uint16)Y[z][y];
       	  } 
       	  X[z][1]=X[z][0];
       	  Y[z][1]=Y[z][0];
      	 SDL_SetRenderDrawColor(renderer, (Uint8) topx, (Uint8) topx,
                              255, 0);
    		 zSDL_RenderDrawPoint(renderer, X[z][0] , Y[z][0]);    	
if (!(topx[z]&20)){px[X[z][0]][Y[z][0]]=topx[z];
                 }
       	  Uint32 bitten;
       	  bitten=objat( z, X[z][0], Y[z][0] ,snd,X,Y,curgra);
       	  if (bitten>popsiz&& Y[z][0]>4 ){
       	       bitten -= popsiz+1;
                snd[bitten][1] -=4; // bumpfood
       	  	
       	  } else if (bitten<popsiz) {
       	  	 if (life[bitten]>1) {
       	  	 	 life[bitten]/=2;
       	  	 	 if ((Uint16)life[z]+(Uint16) life[bitten]<(Uint16) 1000) 
       	  	 	 {
       	  	 	 	life[z]+= life[bitten];
       	  	 	 } else {
       	  	 	 	life[z]=1000;
       	  	 	 }
       	  	 } else {life[bitten]=0;}
       	  }
    //   		 by755for(y=0;y<bps;y++){
   
       
       } // end for x
       	
       } // ennd if any life
       else {
       	// if deadworm
       	//life[z]=0;
       	 for (y=bps-2;y>0;y--) {

	 SDL_SetRenderDrawColor(renderer,px[X[z][bps-1]][Y[z][bps-1]] , px[X[z][bps-1]][Y[z][bps-1]] ,
                              255, (Uint8) 0 );
    		 zSDL_RenderDrawPoint(renderer, X[z][bps - 1], Y[z][bps-1]);

       	  	 SDL_SetRenderDrawColor(renderer, (Uint8) 0, (Uint8) 0,
                               255, (Uint8)0 );
    		 zSDL_RenderDrawPoint(renderer, X[z][y] , Y[z][y]);    	
    
       	 	  X[z][y+1]=(Uint16)X[z][y];
       	 Y[z][y+1]=(Uint16)Y[z][y];
       	  } 
       	  X[z][1]=X[z][0];
       	  Y[z][1]=Y[z][0];

       }
      }//end for z 
 
      Uint16 rlydead=popsiz; 	
       for (z=0;z<popsiz;z++)     { 
   Uint8 maybe=1;
   if (life[z]<1)   {
      for (y = 1; y < bps; y++) {
       if ((X[z][y]!=X[z][0]) || ( Y[z][y]!=Y[z][0]))
       {
        maybe=0; break;
      }}
      if (maybe==1) { rlydead=z;break; }
   }if (rlydead!=popsiz) break;
   }
   
   if ((rlydead<popsiz)) {
   for (z=0;z<popsiz;z++)     { 
   if (life[z]>=900)   {
      
         x=rlydead;
         for (y = 0; y < bps; y++) {
           X[x][y]=X[z][ bps-(1+y) ];
           
           
       	Y[x][y]=Y[z][ bps-(1+y) ];

       }
       for (y=0;y<nugen;y++) {
       	 dirs[x][y]=dirs[z][y];
       dists[x][y]=dists[z][y];
      relofs[x][y]=relofs[z][y];
       	
     }     
       life[z]/=1.5;
       life[x]=life[z] ;
       topx[x]=topx[z];
       
    break;   }
   }  } // mappx(renderer,mode,px);

    SDL_RenderPresent(renderer);
       SDL_UpdateWindowSurface(window);
    
    if (rand()>290000000) {//often?
    // choose up to three for a cosmic ray
 z=rand()%popsiz;
 
    dirs[(z)][(rand()%nugen)]=(rand());
      dists[(z)][(rand()%nugen)]=(rand());
        relofs[(z)][(rand()%nugen)]=(rand());
    topx[z]=(rand()%26)*10;
        
    	}
    
   /*    (x=0;x<mode.w;x++){
    	for (y=0;y<mode.h;y++) {
    	  px[x][y]=rand()%256;
    v	}
   }*/
   ip+=1;
   if (ip==nugen) {
  // return 0;
   ip = 0;
   	}
   }
    
    /* Printt out some tibming information  */
    now = SDL_GetTicks();
   if(now > then) {
        double fps = ((double) frames * 1000) / (now - then);
        SDL_Log("%i frames per second\n", fps);
    }
    
    if (game==1&&done<2) {
#ifdef game
level+=win;
zoom = 3+((level%3)*2);
cols= ((mode.w)/zoom)-1;
rows= ((mode.h)/zoom)-1;

popsiz=3*(level+1);
bps= 3*(level+1);
ltx=10-(level%10);
nugen=10*(level+1);
blownupsandreward=33*(level+1);
grains=100*(level+1);
timelimit=1000+200*(level+1);
scoregoal=7000000*(level+1);
targetface=level%4; // 0 top 1 bottom 2 west 3 east
#endif 
SDL_ShowSimpleMessageBox(0, "welcome to next lvl!", "welcome to next lvl!", window);
gt=0;
gs=0;
} else {
  done=1;
  break;
}
}

    SDL_Quit();

    return 0;
}

