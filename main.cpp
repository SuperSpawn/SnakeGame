#include "Lehmer.h"

#define SNAKE_DESIGN '*'

#define diff(a,b) ((a)>(b) ? (a) - (b) : (b) - (a))


enum SnakeDirection
{
    RIGHT=-1,
    DOWN=-2,
    LEFT=1,
    UP=2
};



class Game
{
public:
    int snakePos, fruitPos;
    int* screen;
    int width, height;
    deque<int> snake;
    int snakeLength;
    int snakeDir;
    int ticks;

    bool gameOver;

    Game(int w, int h)
    {
        screen = new int[w * h];
        if(screen == nullptr)
            return;
        width = w;
        height = h;
        ticks = 5;

        snakeLength = 1;
        snake.push_front(0);
        snakePos = 0;
        snakeDir = RIGHT;
        gameOver = false;

        generate_fruit();
    }

    void generate_fruit()
    {
        fruitPos = Lehmer32() % (width * height);
    }

    void render_screen()
    {
        clear();
        

        for(int i = 0; i < width; ++i)
            printw("XX");
        for(int i = 0; i < width * height; ++i)
        {
            if(i % width == 0)
                printw("X\nX");
            printw("%c ", screen[i]);
        }
        printw("\n");
        for(int i = 0; i < width; ++i)
            printw("XX");

        refresh();
    }

    void update_screen()
    {
        for(int i = 0; i < width * height; ++i)
        {
            screen[i] = ' ';
        }
        
        screen[fruitPos] = '$';
        for(auto i: snake)
        {
            if(i > 0 && i < width * height)
                screen[i] = SNAKE_DESIGN;
        }
        
    }

    void update_input()
    {
        int newDir = -snakeDir;
        int ch = getch();
        switch (ch) {
            case 'w':
                newDir = UP;
                break;
            case 's':
                newDir = DOWN;
                break;
            case 'a':
                newDir = LEFT;
                break;
            case 'd':
                newDir = RIGHT;
                break;
        }

        if(newDir != -snakeDir)
            snakeDir = newDir;

    }

    void update_logic()
    {
        update_input();
        
        int oldPos = snakePos;

        switch(snakeDir)
        {
        case RIGHT:
            snakePos++;
            break;
        case DOWN:
            snakePos += width;
            break;
        case LEFT:
            snakePos--;
            break;
        case UP:
            snakePos -= width;
            break;
        }

        if(snakePos == fruitPos)
        {
            snakeLength++;
            generate_fruit();
        }       
        else
            snake.pop_back();
        
        // add new value
        snake.push_front(snakePos);

        if(screen[snakePos] == SNAKE_DESIGN)
            gameOver = true;

        if(snakePos < 0 || snakePos > width * height)
            gameOver = true;
        
        int oldX = oldPos % width;
        int snakeX = snakePos % width; 

        if(diff(oldX, snakeX) > 2)
        {
            gameOver = true;
        }

        update_screen();
    }

    void start() {
        // initialize screen
        initscr();  // Initialize ncurses
        noecho();   // Don't display input characters
        curs_set(0); // Hide the cursor
        nodelay(stdscr, TRUE);


        // Calculate the duration of each tick
        std::chrono::duration<double> tickDuration(1.0 / static_cast<double>(ticks));

        // Run the tick function for a specified number of ticks
        while (!gameOver) {
            auto start = std::chrono::high_resolution_clock::now();

            // Call the function you want to run on each tick
            render_screen();
            update_logic();

            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

            // Sleep to maintain the desired ticks per second
            std::this_thread::sleep_for(tickDuration - elapsed);
        }

        endwin();  // End ncurses before exiting
    }


    ~Game()
    {
        delete[] screen;
    }
};





int main() {
    Game game(20,20);
    game.start();

    return 0;
}
