#include <iostream>
#include <raylib.h>

using namespace std;

Color Blue_Side = Color{0,102,255,255};//transparency
Color Red_Side = Color{255,51,51,255};
Color LightBlue = Color{135, 206, 250, 255};
Color LightRed  = Color{255, 160, 122, 255};
Color Yellow = Color{243,213,91,255};
int player_score=0;
int cpu_score=0;
bool game_over=false;
const int max_score=15;//to show the game upto 15 points
class Ball
{
    public:
    float x,y;
    int speed_x,speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x,y,radius ,Yellow);

    }
    void Update()
    {
        if(game_over)return;
        x=x+speed_x;
        y=y+speed_y;

        if(y + radius >= GetScreenHeight() || y-radius <=0)//check for collision
        {
            speed_y*=-1;
        }
        if(x + radius >= GetScreenWidth())//Cpu wins
        {
            cpu_score++;
            if(cpu_score>=max_score)
            {
                game_over=true;
            }
            ResetBall();
        }
        
        if( x-radius<=0)
        {
           player_score++;
            if(player_score>=max_score)
            {
                game_over=true;
            }

           ResetBall();
         }
    }
    void ResetBall()
    {
        x=GetScreenWidth()/2;
        y=GetScreenHeight()/2;
        int speed_choices[2] ={-1,1};
        speed_x*=speed_choices[GetRandomValue(0,1)];
        speed_y*=speed_choices[GetRandomValue(0,1)];
    }
    };
class Paddle
{
    protected:
    void LimitMovement()
    {
         if(y<=0)
        {
            y= 0;
        }
        if(y + h>= GetScreenHeight())//height-h
        {
            y=GetScreenHeight()-h;
        }

    }
    public:
    float x,y;
    float wid , h;
    int speed;
    void Draw()
    {
     DrawRectangleRounded(Rectangle{x,y,wid,h},0.8,0,WHITE);
 
    }
    void Update()
    {
        if(IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if(IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }
       LimitMovement();
    }
};
//for computer to operate...
class CpuPaddle : public Paddle
{
    public:
    void Update(int ball_y)
    {
        if(y+h/2>ball_y)
        {
            y=y-speed;
        }
        if(y+h/2<=ball_y)
        {
            y=y+speed;
        }
        LimitMovement();
    }


};


Ball ball;
Paddle player;
CpuPaddle cpu;

int main ()
{
    cout<<" Starting the game: "<<endl;
    const int width =1200;
    const int height=800;
    InitWindow(width,height,"SUKUNA VS GOJO - PONG BATTLE");
    SetTargetFPS(60);

    ball.radius=20;
    ball.x=width/2;
    ball.y=height/2;
    ball.speed_x= 7;
    ball.speed_y= 7;
    //player paddle(red).
    player.wid = 25;
    player.h = 120;
    player.x = width - player.wid -10;
    player.y=height/2-player.h/2;
    player.speed = 6;
    //cpu paddle (blue)
    cpu.h = 120;
    cpu.wid = 25;
    cpu.y = height/2 -cpu.h/2;
    cpu.speed = 6;

    while( WindowShouldClose()==false)
    {
        BeginDrawing();//Begin Drawing

        //Update
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        //Checking for collisions..
        if(!game_over)
        {
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.wid,player.h} ))
        {
            ball.speed_x*=-1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu.x,cpu.y,cpu.wid,cpu.h}))
        {
            ball.speed_x*=-1;
        }
    }
    //restart the game if match point..
    if (game_over && IsKeyPressed(KEY_ENTER)) {
            player_score = 0;
            cpu_score = 0;
            game_over = false;
            ball.ResetBall();
        }


        // Drawing
        ClearBackground(BLACK);//to erase the backtrack of the ball.
        DrawRectangle(0,0,width/2,height,Blue_Side);//Updating background colours.
        DrawCircle(width/2,height/2,150,Red_Side);
        DrawLine(width/2,0,width/2,height,WHITE);
        ball.Draw();
        cpu.Draw();
       player.Draw();
       DrawText(TextFormat("%i",cpu_score), width/4-20,20, 80,WHITE);
       DrawText(TextFormat("%i",player_score),3* width/4-20,20, 80,WHITE);
       //Game Over screen. 
       if (game_over) {
           if (player_score >= max_score) {
                // SUKUNA Wins (RED)
                DrawText("SUKUNA WINS!", width / 2 - 220, height / 2 - 120, 60, RED);

                DrawText(
                    "Press ENTER to challenge fate again, King of Curses.",
                    width / 2 - 380, height / 2 + 40, 30, RED
                );
            } else {
                // GOJO Wins (BLUE)
                DrawText("GOJO WINS!", width / 2 - 200, height / 2 - 120, 60, BLACK);

                DrawText(
                    "Press ENTER to step back into Gojo's domain expansion.",
                    width / 2 - 420, height / 2 + 40, 30, BLUE
                );
            }
        }

        EndDrawing();

    }
   CloseWindow(); 
    return 0;
}


    