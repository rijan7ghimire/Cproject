#include<SDL2/SDL.h>
#include<stdio.h>
#include<SDL2/SDL_ttf.h>
#include"main.h"
#include<SDL2/SDL_mixer.h>

#define GRAVITY 0.14f

void gameOver(GameState *game){

//     //set the drawing color to blue
//     SDL_SetRenderDrawColor(game->renderer,0,0,0,0);
//
//     //clear the screen to the above given color
//     SDL_RenderClear(game->renderer);
 //displaying the game over screen
    SDL_Surface *Surface;
    char buffer[100];
       if (game->font!=NULL){
        SDL_Color color={250,50,50,255};
        sprintf(buffer,"GAME OVER::SCORE- %d",game->score);
    Surface = TTF_RenderText_Blended(game->font,buffer,color);
   }if (Surface!=NULL){
   game->label=SDL_CreateTextureFromSurface(game->renderer,Surface);
   }
   SDL_FreeSurface(Surface);

//draw the fonts in the screen
       //set the drawing color to black
       SDL_SetRenderDrawColor(game->renderer,25,25,25,255);
      SDL_Rect fontRect={70,120,500,130};
      SDL_RenderCopy(game->renderer,game->label, NULL, &fontRect);
     SDL_RenderPresent(game->renderer);

SDL_Delay(2500);
   game->fbird.x =200;
   game->fbird.y =220;
   game->fbird.dy=0;
   game->time=0;
   game->status=0;
   game->score=0;
   game->fbird.jump=false;
   game->label=NULL;



}
void loadGame(GameState *game){
//
SDL_Surface *Surface=NULL;
//load images and create rendering texture from them
 Surface =IMG_Load("pebbles.JPG");
if (Surface==NULL)
{   printf("cannot find pebbles.jPEG !\n\n");
    SDL_Quit();
    exit(1);
}
   game->ground=SDL_CreateTextureFromSurface(game->renderer,Surface);
   SDL_FreeSurface(Surface);//free the memory the surface was using


 Surface =IMG_Load("walls.JPG");
if (Surface==NULL)
{   printf("cannot find walls.JPEG !\n\n");
    SDL_Quit();
    exit(1);
}
   game->wall=SDL_CreateTextureFromSurface(game->renderer,Surface);
   SDL_FreeSurface(Surface);//free the memory the surface was using
   //init walls
   for (int i=0;i<500;i++)
   {
       game->walls[i].x=400+i*200;
       game->walls[i].y1=0;
       int k= rand()%250;
       game->walls[i].h1=20+k;
       game->walls[i].y2=k+150;
       game->walls[i].w=60;
       game->walls[i].h2=250;
   }

   Surface =IMG_Load("bird.JPG");
if (Surface==NULL)
{   printf("cannot find bird.JPEG!\n\n");
    SDL_Quit();
    exit(1);
}
   game->bird1=SDL_CreateTextureFromSurface(game->renderer,Surface);
   SDL_FreeSurface(Surface);//free the memory the surface was using
    Surface =IMG_Load("birdh.JPG");
if (Surface==NULL)
{   printf("cannot find bird.JPEG!\n\n");
    SDL_Quit();
    exit(1);
}

   game->bird2=SDL_CreateTextureFromSurface(game->renderer,Surface);
   SDL_FreeSurface(Surface);//free the memory the surface was using

   //load sounds
   game->bgMusic =Mix_LoadWAV("bgmusic.WAV");
   game->jumpSound =Mix_LoadWAV("jump.WAV");
    game->wallColi =Mix_LoadWAV("wallcoli.WAV");

   //load Fonts
   game->font =TTF_OpenFont("murder.ttf",76);//76 font size


   game->fbird.x =200;
   game->fbird.y =220;
   game->fbird.dy=0;


}
void loadfonts(GameState *game){

   //displaying and updating the fonts
    SDL_Surface *Surface;
       if (game->font!=NULL){
        SDL_Color color={255,255,255};
        char buffer[10];
        if(game->fbird.dy!=0){
          if (game->time%105==0)
            {game->score+=1;
            sprintf(buffer,"%d",game->score);}}


    Surface = TTF_RenderText_Solid(game->font,buffer,color);
   }if (Surface!=NULL){
   game->label=SDL_CreateTextureFromSurface(game->renderer,Surface);
   }
   SDL_FreeSurface(Surface);


}
void process(GameState *game)
{

    game->time++;
    Bird *fbird=&game->fbird;
    fbird->y +=fbird->dy;
    if (game->fbird.jump==true){
    fbird->dy +=GRAVITY;
    fbird->x +=2;
    }
    if (game->time%20==0){
        if (game->fbird.animframe==0)
        {
            game->fbird.animframe=1;

        }
        else {game->fbird.animframe=0;}

    }

}

void collisionDetect(GameState *game)
{

   for (int i=0;i<500;i++){
    float bw=36,bh=24;
    float bx=game->fbird.x,by=game->fbird.y;
    float gx=game->walls[i].x,gy2=game->walls[i].y2,gy1=0,gw=game->walls[i].w,gh1=game->walls[i].h1,gh2=game->walls[i].h2;
    if (bx<0){
        game->fbird.x=0;
         game->fbird.dy=0;
          game->status=1;//1 for game over
          Mix_PlayChannel(-1,game->wallColi,0);
    }
    if (by<0){
        game->fbird.y=0;
        //stop any jump velocity
         game->fbird.dy=0;
          game->status=1;//1 for game over
          Mix_PlayChannel(-1,game->wallColi,0);
        }

    //check collision with the above wall
   if(by>=0 && by<=gh1)
   {
       if (bx+bw>=gx && bx+bw<=gx+gw){
        game->fbird.x=gx-bw;
        bx =gx-bw;
         game->fbird.dy=0;
         game->status=1;//1 for game over
         Mix_PlayChannel(-1,game->wallColi,0);

       }
       else if( bx<=gx+gw && bx>=gx){
        game->fbird.x=gx+gw;
        bx =gx+gw;
         game->fbird.dy=0;
         game->status=1;
         Mix_PlayChannel(-1,game->wallColi,0);

       }
   }
   if(by+bh<gy2&&by>gh1)
   {
    if (by+bh>gy2){
        game->fbird.y=gy2-bh;
        by=gy2-bh;
         game->fbird.dy=0;

    }
    if (by<gh1){
        game->fbird.y=gh1;
        by=gh1;
         game->fbird.dy=0;

        }
   }
   if(by+bh>=400){
    game->fbird.y=400-24;
     by=400-24;
      game->fbird.dy=0;
       game->status=1;//1 for game over
       Mix_PlayChannel(-1,game->wallColi,0);

   }
  if (by>=gy2 && by+bh<=400){

    if (bx+bw>=gx && bx+bw<=gx+gw){
        game->fbird.x=gx-bw;
        bx =gx-bw;
        game->fbird.dy=0;
        game->status=1;//1 for game over
        Mix_PlayChannel(-1,game->wallColi,0);
       }
       else if( bx<=gx+gw && bx>=gx){
        game->fbird.x=gx+gw;
        bx =gx+gw;
         game->fbird.dy=0;
         game->status=1;//1 for game over
         Mix_PlayChannel(-1,game->wallColi,0);
       }

  }
}
}

void doRender(SDL_Renderer *renderer,GameState *game){
     //render display
     //set the drawing color to blue
     SDL_SetRenderDrawColor(renderer,144,234,248,100);

     //clear the screen to the above given color
     SDL_RenderClear(renderer);

     //set the drawing color "red"
//     SDL_SetRenderDrawColor(renderer,255,0,0,255);

     if(game->fbird.animframe ==1){
     SDL_Rect rect ={game->scrollx+game->fbird.x,game->fbird.y,36,24};
     SDL_RenderCopyEx(renderer,game->bird1,NULL,&rect,0,NULL,0);}
     if (game->fbird.animframe==0){
     SDL_Rect rect1 ={game->scrollx+game->fbird.x,game->fbird.y,36,24};
     SDL_RenderCopyEx(renderer,game->bird2,NULL,&rect1,0,NULL,0);}

      //draw the wall image
      for (int i=0;i<500;i++){
            int h =(game->walls[i].x),k=(game->walls[i].y2);
            int h1 =game->walls[i].h1;

     SDL_Rect  wallRect={game->scrollx+h,0,60,h1};
     SDL_RenderCopy(renderer,game->wall,NULL,&wallRect);

      SDL_Rect  wall2Rect={game->scrollx+h,k,60,250};
     SDL_RenderCopy(renderer,game->wall,NULL,&wall2Rect);
     //we are done drawing "present" to the screen what we've drawn
     }
      //draw the ground image
     SDL_Rect  groundRect={0,400,640,80};
     SDL_RenderCopy(renderer,game->ground,NULL,&groundRect);
     SDL_RenderPresent(renderer);
      //draw the fonts in the screen
      SDL_Rect fontRect={320,0,60,100};
      SDL_RenderCopy(renderer,game->label, NULL, &fontRect);
     SDL_RenderPresent(renderer);


}
int processEvents(SDL_Window *window,GameState *game){
 int done=0;
  SDL_Event event;
        //check for events
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_WINDOWEVENT_CLOSE:
                {
                    if (window)
                    {
                       SDL_DestroyWindow(window);
                       window =NULL;
                       done=1;
                    }
                }
                break;
             case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        done =1;
                        break;
                    case SDLK_SPACE:
                     Mix_VolumeChunk(game->jumpSound,64);
                     Mix_PlayChannel(2,game->jumpSound,0);
                       game->fbird.jump=true;
                       game->fbird.dy =-4;
                    }
                    break;
                }
             case SDL_QUIT :
             //quit out of the game
                    done =1;
                    break;

            }
        }
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_LEFT]){
  game->fbird.x -=10;
  }
  if(state[SDL_SCANCODE_RIGHT]){
  game->fbird.x +=10;
  }
//    if(state[SDL_SCANCODE_UP]){
//  game->fbird.y -=10;
//  }
//  if(state[SDL_SCANCODE_DOWN]){
//  game->fbird.y +=10;
//  }
  game->scrollx=-game->fbird.x+300;


return done;}


int main(int argc, char *argv[])
{
    SDL_Window *window=NULL;      //declare a window
    SDL_Renderer *renderer=NULL;   //declare a renderer

    SDL_Init(SDL_INIT_VIDEO |SDL_INIT_AUDIO);  //initialize SDL2
    //create an application window with the following settings:
     window =SDL_CreateWindow("Flappy Bird",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,480,0);
     renderer =SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
       GameState game;
     game.renderer =renderer;
     game.scrollx = 0;
     game.fbird.jump=false;
     game.fbird.animframe=0;
     game.time =0;
     game.font =NULL;
     game.bgMusic =NULL;
     game.score =0;
     game.status=0;

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,4096);//initialize audio
     TTF_Init();

     loadGame(&game);
     int done =0;
//play bgmusic
            Mix_PlayChannel(-1,game.bgMusic,-1);
     while(!done){
            //check for events
     done= processEvents(window,&game);
     process(&game);
             //detects collision
     collisionDetect(&game);
            //render display
     doRender(renderer,&game);
            //delay
//   SDL_Delay(10);
     if (game.status==1){
        gameOver(&game);
     }
     loadfonts(&game);
     }
     //initialize ttf font



    //shutdown game and unload all memory

    SDL_DestroyTexture(game.wall);
    SDL_DestroyTexture(game.ground);
    SDL_DestroyTexture(game.bird1);
    SDL_DestroyTexture(game.bird2);
    if (game.label!=NULL)
        SDL_DestroyTexture(game.label);
    TTF_CloseFont(game.font);
     //close and destroy the windows
     Mix_FreeChunk(game.bgMusic);
     Mix_FreeChunk(game.jumpSound);
     Mix_FreeChunk(game.wallColi);
     SDL_DestroyWindow(window);
     SDL_DestroyRenderer(renderer);
    Mix_CloseAudio();
     TTF_Quit();
     //clean up
     SDL_Quit();
     return 0;


}
