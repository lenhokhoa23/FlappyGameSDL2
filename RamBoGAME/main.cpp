
#include "CommonFunc.h"
#include "PlayerObject.h"
#include "ImpTimer.h"
#include "BlockObject.h"
#include "Geometric.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640


BaseObject g_background;
BaseObject g_ground;

TTF_Font* g_font_text = NULL;
TTF_Font* g_font_MENU = NULL;
int g_high_score = 0;
void SaveHighScore(int score); 
int LoadHighScore();
void SaveHighScore(int score)
{
    std::ofstream file("highscore.txt");
    if (file.is_open())
    {
        file << score;
        file.close();
    }
}

int LoadHighScore()
{
    std::ifstream file("highscore.txt");
    int highscore = 0;
    if (file.is_open())
    {
        file >> highscore;
        file.close();
    }
    return highscore;
}
bool InitData()
{
    BOOL bSucess = true;
    int ret = SDL_Init(SDL_INIT_VIDEO); // tao cua so, hien thi do hoa
    if (ret < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Rambo Le Nho Khoa",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL);

    if (g_window != NULL)
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen != NULL)
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR,
                RENDER_DRAW_COLOR,
                RENDER_DRAW_COLOR,
                RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                bSucess = false;
        }

        if (TTF_Init() == -1)
        {
            bSucess = false;
        }

        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            bSucess = false;
        }

        g_font_text = TTF_OpenFont("font//ARCADE.ttf", 38);
        if (g_font_text == NULL)
        {
            return false;
        }

        g_font_MENU = TTF_OpenFont("font//ARCADE.ttf", 85);
        if (g_font_MENU == NULL)
        {
            return false;
        }
    }
    g_high_score = LoadHighScore();
    return bSucess;
}


bool LoadBackground()
{
    bool ret = g_background.LoadImageFile("img//bkgn.png", g_screen);
    return ret;
}




void close()
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    if (InitData() == false)
    {
        return -1;
    }

    ImpTimer fps;
    bool quit = false;

    //Make menu game 
    int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU, "Play Game", "Exit", "img//MENU.png");
    if (ret_menu == 1)
        quit = true;

    if (!LoadBackground())
    {
        return -1;
    }

    bool ret = g_ground.LoadImageFile("img//ground2.png", g_screen);
    if (ret == false)
    {
        return -1;
    }
    g_ground.SetRect(0, GROUND_MAP);

again_label:

    TextObject text_count_;
    text_count_.setColor(TextObject::WHITE_TEXT);

    PlayerObject player;
    ret = player.LoadImg("img//fl_ver3.png", g_screen);
    player.SetRect(100, 100);
    if (ret == false)
    {
        return -1;
    }

    BlockManager manage_block;
    ret = manage_block.InitBlockList(g_screen);
    if (ret == false)
        return -1;

    while (!quit)
    {
        fps.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                quit = true;
            }

            player.HandleInputAction(g_event, g_screen);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR,
            RENDER_DRAW_COLOR,
            RENDER_DRAW_COLOR,
            RENDER_DRAW_COLOR);

        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);

        manage_block.SetPlayerRect(player.GetRect()); // rectangle around p

        bool is_falling = player.GetFalling();
        if (is_falling == true)
        {
            manage_block.SetStopMoving(true);
        }

        manage_block.Render(g_screen);           // ve chuong ngai vat

        bool end_game = manage_block.GetColState();
        if (end_game == true)
        {
            player.SetFalling(true);
        }
        player.DoFalling(g_screen);
        player.Show(g_screen);

        //Draw Geometric
        GeometricFormat rectange_size(0, 0, SCREEN_WIDTH, 30);
        ColorData color_data(36, 36, 36);
        Gemometric::RenderRectange(rectange_size, color_data, g_screen);

        GeometricFormat outlie_size(1, 1, SCREEN_WIDTH - 1, 28);
        ColorData color_data1(255, 255, 255);
        Gemometric::RenderOutline(outlie_size, color_data1, g_screen);

        int count = manage_block.GetCount();
        std::string count_str = std::to_string(count);
        text_count_.SetText(count_str);
        text_count_.loadFromRenderedText(g_font_text, g_screen);
        text_count_.RenderText(g_screen, SCREEN_WIDTH * 0.5, 2);    // render text_count cach mep tren 2 pixel

        g_ground.Render(g_screen);
        SDL_RenderPresent(g_screen);

        // Make menu game over
        bool game_over = player.GetIsDie();
        if (game_over == true)
        {

            if (manage_block.GetCount() > g_high_score)
            {
                g_high_score = manage_block.GetCount();
                SaveHighScore(g_high_score);
            }

            TextObject cur_score;
            cur_score.setColor(TextObject::RED_TEXT);
            std::string cur_score_str = "YOUR CURRENT SCORE: " + std::to_string(count);
            cur_score.SetText(cur_score_str.c_str());
            cur_score.loadFromRenderedText(g_font_text, g_screen);
            cur_score.RenderText(g_screen, SCREEN_WIDTH / 2 - 230, 250);

            TextObject high_score_text;
            high_score_text.setColor(TextObject::RED_TEXT);
            std::string high_score_str = "YOUR HIGHEST SCORE: " + std::to_string(g_high_score);
            high_score_text.SetText(high_score_str.c_str());
            high_score_text.loadFromRenderedText(g_font_text, g_screen);
            high_score_text.RenderText(g_screen, SCREEN_WIDTH / 2 - 230 , 300); // Canh giữa màn hình
            SDL_RenderPresent(g_screen); // Cập nhật lại màn hình để hiển thị điểm cao nhất

            Sleep(2000);          // tam dung 500 mili s de nguoi choi nhin toan canh truoc khi ve menu game over
            int ret_menu = SDLCommonFunc::ShowMenu(g_screen, g_font_MENU,
                "Play Again", "Exit",
                "img//MENU END.png");
            if (ret_menu == 1)
            {
                quit = true;
                continue;
            }
            else
            {
                quit = false;
                manage_block.FreeBlock();
                goto again_label;
            }
        }

        //Cap the frame rate
        int val1 = fps.get_ticks();
        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
        {
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
        }
    }

    close();
    return 0;
}
