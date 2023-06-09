#include <stdlib.h>
#include <array>
#include <string>
#include <vector>
#include "common.hpp"

#include "ui.hpp"

#include "raygui.h"

#include "colour.hpp"
#include "cube.hpp"
#include "face.hpp"
#include "tutorial.hpp"
#include "video.hpp"

UI::UI()
{
    // all required letters, needed to draw arrow keys
    // may substitute with a texture to avoid this approach in future
    static char* text = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz2' ↑↓←→↻↺";
    codepointCount = 61;
    codepoints = LoadCodepoints(text, &codepointCount);
    font = LoadFontEx("data/terminus.ttf", 48, codepoints, codepointCount);

    tutorial.Init(nullptr);

    cube = tutorial.get_cube_pointer();
    cur_face = tutorial.get_cur_face_pointer();

#ifdef OS_LIN
    video_index = 0; // valid video index to prevent code breaking
#else
    video_index = 1; // linux starts from 0, while windows starts from 1
#endif
    video.Init(video_index);
    face.Init(
        video.get_frame_pointer(),
        video.get_size(),
        video.get_scale(),
        tutorial.get_cube_pointer(),
        tutorial.get_step_pointer(),
        tutorial.get_cur_face_pointer()
    );

    int l = (int)(std::min(screenW, screenH) * 0.2);
    std::array<Rectangle, CUBE_FACE_SIZE> daisy_rects;
    std::array<Rectangle, CUBE_FACE_SIZE> fish_rects;
    RenderTexture2D daisy_render = LoadRenderTexture(l + 4, l + 4);
    RenderTexture2D fish_render = LoadRenderTexture(l + 4, l + 4);

    BeginTextureMode(daisy_render);
    ClearBackground(BLANK);

    daisy_rects = face.GetStateFrameRects(2, 2, l / 3);
    DrawRectangle(2, 2, l, l, cubeRGBCols[CUBE_SIZE]); // faster than individually setting each piece to grey
    DrawRectangleRec(daisy_rects[1], cubeRGBCols[CUBE_WHITE]);
    DrawRectangleRec(daisy_rects[3], cubeRGBCols[CUBE_WHITE]);
    DrawRectangleRec(daisy_rects[4], cubeRGBCols[CUBE_YELLOW]);
    DrawRectangleRec(daisy_rects[5], cubeRGBCols[CUBE_WHITE]);
    DrawRectangleRec(daisy_rects[7], cubeRGBCols[CUBE_WHITE]);
    face.draw_frame(2, 2, l / 3, 3, true);

    EndTextureMode();
    tutorial.Set_daisy(daisy_render.texture);

    BeginTextureMode(fish_render);
    ClearBackground(BLANK);

    fish_rects = face.GetStateFrameRects(2, 2, l / 3);
    DrawRectangle(2, 2, l, l, cubeRGBCols[CUBE_SIZE]); // faster than individually setting each piece to grey
    DrawRectangleRec(fish_rects[1], cubeRGBCols[CUBE_YELLOW]);
    DrawRectangleRec(fish_rects[3], cubeRGBCols[CUBE_YELLOW]);
    DrawRectangleRec(fish_rects[4], cubeRGBCols[CUBE_YELLOW]);
    DrawRectangleRec(fish_rects[5], cubeRGBCols[CUBE_YELLOW]);
    DrawRectangleRec(fish_rects[6], cubeRGBCols[CUBE_YELLOW]);
    DrawRectangleRec(fish_rects[7], cubeRGBCols[CUBE_YELLOW]);
    face.draw_frame(2, 2, l / 3, 3, true);

    EndTextureMode();
    tutorial.Set_fish(fish_render.texture);

    step = tutorial.get_step_pointer();
    pred_state = make_array<CUBE_FACE_SIZE>(CUBE_ANY);
};

void UI::Update()
{
#ifdef DEBUG
    if (IsKeyPressed(KEY_U)) { cube->Permute(std::vector<int>{ { M_U } }, CUBE_RED); }
    if (IsKeyPressed(KEY_Y)) { cube->Permute(std::vector<int>{ { M_UP } }); }
    if (IsKeyPressed(KEY_I)) { cube->Permute(std::vector<int>{ { M_U2 } }); }
    if (IsKeyPressed(KEY_L)) { cube->Permute(std::vector<int>{ { M_L } }); }
    if (IsKeyPressed(KEY_K)) { cube->Permute(std::vector<int>{ { M_LP } }); }
    if (IsKeyPressed(KEY_SEMICOLON)) { cube->Permute(std::vector<int>{ { M_L2 } }); }
    if (IsKeyPressed(KEY_F)) { cube->Permute(std::vector<int>{ { M_F } }); }
    if (IsKeyPressed(KEY_C)) { cube->Permute(std::vector<int>{ { M_FP } }); }
    if (IsKeyPressed(KEY_G)) { cube->Permute(std::vector<int>{ { M_F2 } }); }
    if (IsKeyPressed(KEY_R)) { cube->Permute(std::vector<int>{ { M_R } }); }
    if (IsKeyPressed(KEY_E)) { cube->Permute(std::vector<int>{ { M_RP } }); }
    if (IsKeyPressed(KEY_T)) { cube->Permute(std::vector<int>{ { M_R2 } }); }
    if (IsKeyPressed(KEY_B)) { cube->Permute(std::vector<int>{ { M_B } }); }
    if (IsKeyPressed(KEY_V)) { cube->Permute(std::vector<int>{ { M_BP } }); }
    if (IsKeyPressed(KEY_N)) { cube->Permute(std::vector<int>{ { M_B2 } }); }
    if (IsKeyPressed(KEY_D)) { cube->Permute(std::vector<int>{ { M_D } }); }
    if (IsKeyPressed(KEY_S)) { cube->Permute(std::vector<int>{ { M_DP } }); }
    if (IsKeyPressed(KEY_X)) { cube->Permute(std::vector<int>{ { M_D2 } }); }

#endif
    if (*step == 0)
    {
        if (IsKeyPressed(KEY_ONE)) { video_index = 1; }
        if (IsKeyPressed(KEY_TWO)) { video_index = 2; }
        if (IsKeyPressed(KEY_THREE)) { video_index = 3; }
        if (IsKeyPressed(KEY_FOUR)) { video_index = 4; }
        if (IsKeyPressed(KEY_FIVE)) { video_index = 5; }
        if (IsKeyPressed(KEY_SIX)) { video_index = 6; }
        if (IsKeyPressed(KEY_SEVEN)) { video_index = 7; }
        if (IsKeyPressed(KEY_EIGHT)) { video_index = 8; }
        if (IsKeyPressed(KEY_NINE)) { video_index = 9; }
        if (IsKeyPressed(KEY_ZERO)) { video_index = 0; }
        if (video_index != video.get_video_index())
        {
            video.open_camera(video_index);
            face.set_video(video.get_frame_pointer(), video.get_size(), video.get_scale());
        }

        if (IsKeyPressed(KEY_Q))
        {
            *cur_face = CUBE_YELLOW;
            cube->set_state(
                std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE>{ {
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_BLUE, CUBE_BLUE, CUBE_BLUE,
                        CUBE_RED, CUBE_WHITE, CUBE_YELLOW,
                        CUBE_BLUE, CUBE_YELLOW, CUBE_BLUE
                    } },
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_YELLOW, CUBE_GREEN, CUBE_ORANGE,
                        CUBE_RED, CUBE_ORANGE, CUBE_YELLOW,
                        CUBE_RED, CUBE_ORANGE, CUBE_WHITE
                    } },
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_WHITE, CUBE_RED, CUBE_WHITE,
                        CUBE_BLUE, CUBE_GREEN, CUBE_GREEN,
                        CUBE_GREEN, CUBE_BLUE, CUBE_YELLOW
                    } },
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_RED, CUBE_ORANGE, CUBE_YELLOW,
                        CUBE_YELLOW, CUBE_RED, CUBE_GREEN,
                        CUBE_ORANGE, CUBE_GREEN, CUBE_GREEN
                    } },
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_ORANGE, CUBE_ORANGE, CUBE_RED,
                        CUBE_ORANGE, CUBE_BLUE, CUBE_BLUE,
                        CUBE_RED, CUBE_RED, CUBE_WHITE
                    } },
                    std::array<int, CUBE_FACE_SIZE>{ {
                        CUBE_ORANGE, CUBE_WHITE, CUBE_GREEN,
                        CUBE_WHITE, CUBE_YELLOW, CUBE_WHITE,
                        CUBE_GREEN, CUBE_WHITE, CUBE_YELLOW
                    } }
                } }
            );
        }

        pred_state = face.Update();
        video.Update();
    }

    tutorial.Update(pred_state);

    temp_alg = tutorial.get_alg();
    move_index = tutorial.get_move_index();
    if (
        alg.size() != temp_alg.size() ||
        !std::equal(alg.begin(), alg.end(), temp_alg.begin())
    ) {
        alg = temp_alg;
        commands = std::vector<std::array<int, 3>>();
        for (int i = 0; i < alg.size(); ++i)
        {
            switch (alg[i] / 3)
            {
                case F_U:
                    commands.push_back(std::array<int, 3>{ F_U, loop(alg[i] % 3, 2, 3), (alg[i] % 3) == 2 });
                    break;

                case F_L:
                    commands.push_back(std::array<int, 3>{ F_L, loop((alg[i] % 3) + 1, 0, 1), (alg[i] % 3) == 2 });
                    break;

                case F_F:
                    commands.push_back(std::array<int, 3>{ F_F, loop((alg[i] % 3) + 1, 2, 3), (alg[i] % 3) == 2 });
                    break;

                case F_R:
                    commands.push_back(std::array<int, 3>{ F_R, loop(alg[i] % 3, 0, 1), (alg[i] % 3) == 2 });
                    break;

                case F_B:
                    commands.push_back(std::array<int, 3>{ F_B, loop((alg[i] % 3) + 1, 2, 3), (alg[i] % 3) == 2 });
                    break;

                case F_D:
                    commands.push_back(std::array<int, 3>{ F_D, loop((alg[i] % 3) + 1, 2, 3), (alg[i] % 3) == 2 });
                    break;
            }
        }
    }
};

void UI::Draw()
{
    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    if (*step == 0)
    {
        video.Draw();
#ifdef OS_LIN
        GuiSpinner((Rectangle){ 260, screenH - 60, 120, 40 }, NULL, &video_index, 0, 32767, false); // linux starts from 0, while windows starts from 1
#else
        GuiSpinner((Rectangle){ 260, screenH - 60, 120, 40 }, NULL, &video_index, 1, 32767, false);
#endif
        GuiLabel((Rectangle){ 390, screenH - 60, screenW, 40 }, "Video index (video output to use)");
    }
    face.Draw();
    tutorial.Draw();

    if (commands.size() != 0)
    {
        for (int i = 1; i < 3; ++i)
        {
            if (
                commands.size() > i &&
                move_index >= i
            ) {
                DrawTextEx(font, Cube_notation_str(alg[move_index - i]).c_str(), Vector2{260, screenH - 16 - 32 * 3 - i * 32 }, 48, 0, BLUE);
                DrawTextEx(font, get_command_string(move_index - i).c_str(), Vector2{ 308, screenH - 16 - 32 * 3 - i * 32 }, 48, 0, DARKGRAY);
            }
        }
        DrawTextEx(font, Cube_notation_str(alg[move_index]).c_str(), Vector2{ 260, screenH - 16 - 32 * 3 }, 48, 0, DARKBLUE);
        DrawTextEx(font, get_command_string(move_index).c_str(), Vector2{ 308, screenH - 16 - 32 * 3 }, 48, 0, BLACK);
        for (int i = 1; i < 3; ++i)
        {
            if (
                commands.size() > i &&
                move_index < commands.size() - i
            ) {
                DrawTextEx(font, Cube_notation_str(alg[move_index + i]).c_str(), Vector2{260, screenH - 16 - 32 * 3 + i * 32 }, 48, 0, BLUE);
                DrawTextEx(font, get_command_string(move_index + i).c_str(), Vector2{ 308, screenH - 16 - 32 * 3 + i * 32 }, 48, 0, DARKGRAY);
            }
        }
    }
};

std::string UI::get_command_string(int index)
{
    std::string out = "Move ";
    switch (commands[index][0])
    {
        case F_U:
            out.append("Up ");
            break;

        case F_L:
            out.append("Left ");
            break;

        case F_F:
            out.append("Front ");
            break;

        case F_R:
            out.append("Right ");
            break;

        case F_B:
            out.append("Back ");
            break;

        case F_D:
            out.append("Down ");
            break;

    }
    switch (commands[index][1])
    {
        case 0:
            out.append("face ↑");
            break;

        case 1:
            out.append("face ↓");
            break;

        case 2:
            out.append("face ←");
            break;

        case 3:
            out.append("face →");
            break;
    }
    if (commands[index][2] == 1) { out.append(" 2x"); }

    return out;
};
