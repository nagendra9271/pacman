#include <stdio.h>  // printf()
#include <conio.h>  // getch()
#include <windows.h>//handles
#include<time.h>    //time
#define H 25
#define W 56
#define NR_GHOSTS 15


int count =0;
int lives=3;
int level=1;
struct coord
    {
        int x;
        int y;
    };

    struct PacMan
    {
        struct coord position;
        int vx;
        int vy;
        int lives;
        int food_collected;
    };
    struct Ghost
    {
        struct coord position;
        int vx;
        int vy;
    };
    struct Ghost allGhosts[NR_GHOSTS];

    struct Mode_values
    {
      int N_Ghosts[3];
      int food_collected[3];
    };

    struct PacMan myPacMan =
    {
        {
            .x = 1,
            .y = 1,
        },
        .vx = 0,
        .vy = 0,
        .food_collected = 0
    };

    struct Mode_values mode=
    {
      .N_Ghosts={7,15,0},
      .food_collected={927,692,806},
    };

    // Set cursor position in console
    void set_cursor_position(int x, int y)
    {
        //Initialize the coordinates
        COORD coord = { x, y };
        //Set the position
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    } // set_cursor_position



    void hidecursor()
    {
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
    }

    void initialize(char playfield[H][W])
    {
        // 1. replace each empty field in the playfield
        //    with a food field
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if (playfield[i][j]==' ')
                {
                 playfield[i][j] = '.';
                }
            }
        }
        srand(time(0));
        // 2. initialize all ghosts
        for (int i = 0; i < mode.N_Ghosts[level-1]; i++)
        {
            allGhosts[i].vx = 0;
            allGhosts[i].vy = 0;
            // try to find a (x,y) coordinate randomly where a food piece is
            int x,y;
            do
            {
                x = 1 + rand() % (W-1);
                y = 1 + rand() % (H-1);
            }
            while (playfield[y][x] != '.');
            allGhosts[i].position.x = x;
            allGhosts[i].position.y = y;
            playfield[y][x] = '\2';
        }


    } // initialize

    void user_input()
    {
        if ( _kbhit())
        {
            char c1 = _getch();

            if (c1 == -32)
            {
                char c2 = _getch();

                myPacMan.vx = 0;
                myPacMan.vy = 0;

                switch (c2)
                {
                case 72:
                    myPacMan.vy = -1;
                    break; // cursor up
                case 80:
                    myPacMan.vy = +1;
                    break; // cursor down
                case 75:
                    myPacMan.vx = -1;
                    break; // cursor left
                case 77:
                    myPacMan.vx = +1;
                    break; // cursor right
                }
            }

            //printf("c1=%d c2=%d\n", c1, c2);
        }
    }

 void move_figures(char playfield[H][W])
    {

        // 1. delete PacMan from old position
        playfield[myPacMan.position.y][myPacMan.position.x] = ' ';

        // 2. compute new desired coordinate (nx,ny)
        int nx = myPacMan.vx + myPacMan.position.x;
        int ny = myPacMan.vy + myPacMan.position.y;
        if ( playfield[ny][nx]=='\2')
        {
            myPacMan.vx = 0;
            myPacMan.vy = 0;

          if(level!=3)
            lives--;

        }
        if(lives<=0)
        {
            count=1;
        }
        // 3. test whether there is a wall at (nx,ny)
        if (playfield[ny][nx] == '#')
        {
            // Damn! There is a wall! Stop PacMan!
            myPacMan.vx = 0;
            myPacMan.vy = 0;
        }

        // 4. update PacMan's coordinate
        myPacMan.position.x += myPacMan.vx;
        myPacMan.position.y += myPacMan.vy;

        // 5. is there a food piece at the new location?
        if (playfield[ny][nx] == '.')
        {
            myPacMan.food_collected++;
        }

        // 6. put PacMan back again to playfield
        playfield[myPacMan.position.y][myPacMan.position.x] = '\1';

    }


    void show_playfield(char playfield[H][W])
    {
        for (int i = 0; i < H; i++)
        {
            set_cursor_position(60,10+i);
            for (int j = 0; j < W; j++)
            {
                if(playfield[i][j]=='\1'||playfield[i][j]=='.')
                  {
                    printf("\033[93m%c",playfield[i][j]);
                  }
                else if(playfield[i][j]=='\2')
                {
                  printf("\033[91m%c",playfield[i][j]);
                }
                else if(playfield[i][j]=='#')
                   {
                    printf("\033[94m%c",playfield[i][j]);
                   }
                else
                  {
                    printf("\033[97m%c",playfield[i][j]);
                  }
            }

            printf("\n");
        }

        printf("\033[97mTotal Score: %d\n", myPacMan.food_collected);
        printf("Lives: %d \033[91m\3",lives);
        int score=myPacMan.food_collected;
        if(score== mode.food_collected[level-1])
        {
            system("cls");
            printf("\033[92m\n\t\t\t\t\tCongratulations!!!\n\n\t\t\t\tYou completed this Level of the GAME!!!.\n");
            level++;
        }
    }

    void reset()
    {
         count=0;
         lives=3;

        myPacMan.position.x = 1;
        myPacMan.position.y = 1;
        myPacMan .vx = 0;
        myPacMan .vy = 0;
        myPacMan .food_collected = 0;
    }

    // starting of Easy Mode

void EasyMode()
{
    char playfield[H][W] =
    {
        { "########################################################" },
        { "#                                                      #" },
        { "#                                                      #" },
        { "#     ############     ############      ############  #" },
        { "#     ############     ############      ############  #" },
        { "#                                                      #" },
        { "#                                                      #" },
        { "#              #########          #########            #" },
        { "#              #########          #########            #" },
        { "#                   ######## #########                 #" },
        { "#                   ######## #########                 #" },
        { "#                      ##### ######                    #" },
        { "#                                                      #" },
        { "#                      ##### ######                    #" },
        { "#                   ######## #########                 #" },
        { "#                   ######## #########                 #" },
        { "#              #########         #########             #" },
        { "#              #########         #########             #" },
        { "# ########                                   ######### #" },
        { "# ########                                   ######### #" },
        { "#    ###                                        ###    #" },
        { "#    ###                                        ###    #" },
        { "#                                                      #" },
        { "#  ###############                       ############  #" },
        { "########################################################" },

    }; // <-- CAUTION! Semicolon necessary!

   //giving mode values;

    system("cls");
    hidecursor();
    initialize(playfield);

    while (level==1)
    {
        user_input();
        move_figures(playfield);
        show_playfield(playfield);

        Sleep(1000/30);

        if(count==1)
        {   system("cls");
            set_cursor_position(40,0);
            printf("\033[91m\tGAME OVER \n\n\n\t\t\t\t\tOops!!!!YOU LOSE THE GAME...\n\n\n");
            break;
        }
    }

}   //end Easy Mode

//Starting of Hard Mode

void HardMode()
{
    char playfield[H][W] =
    {

        { "#######################################################" },
        { "#                         ###                        ##" },
        { "#    ####  ############   ###   ############  ####   ##" },
        { "##   ####  ############   ###   ############  ####   ##" },
        { "##   ####  ############   ###   ############  ####   ##" },
        { "##                                                   ##" },
        { "##    #####        ##  ############  ##    #####     ##" },
        { "##    #####        ##       ##       ##    #####     ##" },
        { "##    #####        #######  ##   ######    #####     ##" },
        { "##    #####        ##       ##       ##    #####     ##" },
        { "##                 ##                ##              ##" },
        { "########  ###          #####   #####         ###  #####" },
        { "#          ##   ####   ##         ##  ####   ##       #" },
        { "#           #   ####   #############  ####   ##       #" },
        { "#          ##   ####                  ####   ##       #" },
        { "#############   ####   #############  ####   ##########" },
        { "##                           ##                      ##" },
        { "##       ########            ##    #########         ##" },
        { "##       ########            ##    #########         ##" },
        { "##       ########            ##    #########         ##" },
        { "##                                                   ##" },
        { "##                                                   ##" },
        { "#######################################################" },
        { "#######################################################" }
    };

    system("cls");
    hidecursor();
    initialize(playfield);

    while (level==2)
    {
        user_input();
        move_figures(playfield);
        show_playfield(playfield);

        Sleep( 1000 / 30 );
        set_cursor_position(0,0);
        if(count==1)
        {  system("cls");
           set_cursor_position(40,0);
           printf("\033[91m\tGAME OVER \n\n\n\t\t\t\t\tOops!!!!YOU LOSE THE GAME...\n\n\n");
           break;
        }

    }



}  //end hard mode


struct Ghost Moving_Ghost[3] ;

char temp[3]={'.','.','.'};

void Move_Ghosts(char playfield[H][W])
    {
      //ghost 1
      Moving_Ghost[0].vx=0;
      Moving_Ghost[0].vy=0;
      //1.removing ghost from previous position
      playfield[Moving_Ghost[0].position.y][Moving_Ghost[0].position.x]=temp[0];

      if( Moving_Ghost[0].position.x>9&& Moving_Ghost[0].position.y==1)
         Moving_Ghost[0].vx=-1;
      else if(Moving_Ghost[0].position.x==9&& Moving_Ghost[0].position.y<10)
         Moving_Ghost[0].vy=+1;
      else if(Moving_Ghost[0].position.x<45&& Moving_Ghost[0].position.y==10)
         Moving_Ghost[0].vx=+1;
      else if(Moving_Ghost[0].position.x==45&& Moving_Ghost[0].position.y>1)
         Moving_Ghost[0].vy=-1;

     // 2. compute new desired coordinate (nx,ny)
     int nx1=Moving_Ghost[0].position.x+Moving_Ghost[0].vx;
     int ny1=Moving_Ghost[0].position.y+Moving_Ghost[0].vy;
     if(playfield[ny1][nx1]=='.')
        {
           temp[0]='.';
        }
     if(playfield[ny1][nx1]==' ')
       {
         temp[0]=' ';
       }
     if(playfield[ny1][nx1]=='\1')
        {
         temp[0]=' ';
         myPacMan.vx = 0;
         myPacMan.vy = 0;
         lives--;
        }
    // 3. assigning position for ghost
     Moving_Ghost[0].position.x+=Moving_Ghost[0].vx;
     Moving_Ghost[0].position.y+=Moving_Ghost[0].vy;

     playfield[Moving_Ghost[0].position.y][Moving_Ghost[0].position.x]='\2';


      //ghost 2
      Moving_Ghost[1].vx=0;
      Moving_Ghost[1].vy=0;
      //1.removing ghost from previous position
      playfield[Moving_Ghost[1].position.y][Moving_Ghost[1].position.x]=temp[1];

      if( Moving_Ghost[1].position.x==14&& Moving_Ghost[1].position.y<16)
         Moving_Ghost[1].vy=+1;
      else if(Moving_Ghost[1].position.x<44&& Moving_Ghost[1].position.y==16)
         Moving_Ghost[1].vx=+1;
      else if(Moving_Ghost[1].position.x==44&& Moving_Ghost[1].position.y>11)
         Moving_Ghost[1].vy=-1;
      else if(Moving_Ghost[1].position.x>14&& Moving_Ghost[1].position.y==11)
         Moving_Ghost[1].vx=-1;

     // 2. compute new desired coordinate (nx,ny)
     int nx2=Moving_Ghost[1].position.x+Moving_Ghost[1].vx;
     int ny2=Moving_Ghost[1].position.y+Moving_Ghost[1].vy;
     if(playfield[ny2][nx2]=='.')
        {
           temp[1]='.';
        }
     if(playfield[ny2][nx2]==' ')
       {
         temp[1]=' ';
       }
     if(playfield[ny2][nx2]=='\1')
        {
         temp[1]=' ';
         myPacMan.vx = 0;
         myPacMan.vy = 0;
         lives--;
        }
    // 3. assigning position for ghost
     Moving_Ghost[1].position.x+=Moving_Ghost[1].vx;
     Moving_Ghost[1].position.y+=Moving_Ghost[1].vy;

      playfield[Moving_Ghost[1].position.y][Moving_Ghost[1].position.x]='\2';



      //ghost 3
      Moving_Ghost[2].vx=0;
      Moving_Ghost[2].vy=0;
      //1.removing ghost from previous position
      playfield[Moving_Ghost[2].position.y][Moving_Ghost[2].position.x]=temp[2];

      if( Moving_Ghost[2].position.x<49&& Moving_Ghost[2].position.y==22)
         Moving_Ghost[2].vx=+1;
      else if(Moving_Ghost[2].position.x==49&& Moving_Ghost[2].position.y>16)
         Moving_Ghost[2].vy=-1;
      else if(Moving_Ghost[2].position.x>9&& Moving_Ghost[2].position.y==16)
         Moving_Ghost[2].vx=-1;
      else if(Moving_Ghost[2].position.x==9&& Moving_Ghost[2].position.y<22)
         Moving_Ghost[2].vy=+1;

     // 2. compute new desired coordinate (nx,ny)
     int nx3=Moving_Ghost[2].position.x+Moving_Ghost[2].vx;
     int ny3=Moving_Ghost[2].position.y+Moving_Ghost[2].vy;
     if(playfield[ny3][nx3]=='.')
        {
           temp[2]='.';
        }
     if(playfield[ny3][nx3]==' ')
       {
         temp[2]=' ';
       }
     if(playfield[ny3][nx3]=='\1')
        {
         temp[2]=' ';
         myPacMan.vx = 0;
         myPacMan.vy = 0;
         lives--;
        }
    // 3. assigning position for ghost
     Moving_Ghost[2].position.x+=Moving_Ghost[2].vx;
     Moving_Ghost[2].position.y+=Moving_Ghost[2].vy;

      playfield[Moving_Ghost[2].position.y][Moving_Ghost[2].position.x]='\2';
    }

 // starting Insane Mode

void InsaneMode()
{

    char playfield[H][W]=
    {
        { "#######################################################" },
        { "#                                                    ##" },
        { "#    #### #############   ###   ############# ####   ##" },
        { "##   #### #############   ###   ############# ####   ##" },
        { "##                                                   ##" },
        { "##    ### ##       ##  ############  ##    ## ###    ##" },
        { "##    ### ##       ##       ##       ##    ## ###    ##" },
        { "##    ### ##       #######  ##   ######    ## ###    ##" },
        { "##    ### ##       ##       ##       ##    ## ###    ##" },
        { "##                 ##                ##              ##" },
        { "##                                                   ##" },
        { "########  ###                                ###  #####" },
        { "#          ##   ####   #####   #####   ####  ##       #" },
        { "#           #   ####   ##         ##   ####  #        #" },
        { "#          ##   ####   #############   ####  ##       #" },
        { "########  ###   ####                   ####  ###  #####" },
        { "##                                                   ##" },
        { "##                     #############                 ##" },
        { "##                          ##                       ##" },
        { "##        #######           ##           #######     ##" },
        { "##        #######           ##           #######     ##" },
        { "##        #######           ##           #######     ##" },
        { "##                                                   ##" },
        { "#######################################################" }
    };

    //intialize ghosts intial positions

    Moving_Ghost[0].position.x=45;
    Moving_Ghost[0].position.y=1;
    Moving_Ghost[1].position.x=14;
    Moving_Ghost[1].position.y=11;
    Moving_Ghost[2].position.x=9;
    Moving_Ghost[2].position.y=22;


    system("cls");
    initialize(playfield);
    while(level==3)
    {
     user_input();
     move_figures(playfield);
     Move_Ghosts(playfield);
     show_playfield(playfield);
     Sleep(1000/30);
     if(count==1)
     {
        system("cls");
        printf("\033[91m\t\t\t\t\t\tGAME OVER \n\n\n\t\t\t\t\tOops!!!!YOU LOSE THE GAME...\n\n\n");
        break;
     }

    }
}
// ending Insane Mode


int main()
{
    system("cls");
    printf("\033[92m");
    printf(" \t\t            $$$$$$    $$$$$$   $$$$$$  $$      $$$  $$$$$$  $$     $$                \n");
    printf(" \t\t            $$   $$  $$    $$ $$       $$$    $$$$ $$    $$ $$$    $$                \n");
    printf(" \t\t            $$   $$  $$    $$ $$       $$ $$ $$ $$ $$    $$ $$ $$  $$                \n");
    printf(" \t\t            $$$$$    $$$$$$$$ $$       $$  $$$  $$ $$$$$$$$ $$  $$ $$                \n");
    printf(" \t\t            $$       $$    $$ $$       $$       $$ $$    $$ $$   $$$$                \n");
    printf(" \t\t            $$       $$    $$  $$$$$$  $$       $$ $$    $$ $$    $$$                \n");
    printf("\n\n\n");

    while(1)
   {
     set_cursor_position(35,9);
     hidecursor();
     if(level>=4)
        {
          printf("\033[92m\t\t\t\t\tyou won all levels of the game");
          break;
        }
     printf("\033[94m\4\4\4\4\4 Press enter to start the level:%d \4\4\4\4\4\n",level);
     if(_kbhit())
     {
       char c1= _getch();
       if(c1==13)
        {
         if(level==1)
            EasyMode();
         else if(level==2)
            HardMode();
         else if(level==3)
            InsaneMode();
         if(level>=4)
             {
                printf("\033[92m\t\t\t\t\tyou won all levels of the game");
                break;
             }
         reset();
        }
     }

  }
}

