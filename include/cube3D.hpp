#ifndef CUBE3D_HPP
#define CUBE3D_HPP

#include <stdlib.h>
#include <array>
#include <cmath>
#include <vector>

#include "common.hpp"
#include "cube.hpp"

#include "raylib.h"

const double SQRT_2 = std::sqrt(2);

class Cube3D
{
    public:
        Cube3D() { };
        void Init(Cube* _cube);

        void Update(bool fast_forward);
        void Draw(int _front_face = CUBE_GREEN, int _orient = 0);

        void Permute(std::vector<std::vector<int>> alg, std::vector<int> _front_faces, std::vector<int> _orients);
        void Update_pieces(); // update position of stickers w/ state
        void Set_state(); // update state of cube in Cube3D to state in Cube
        void Set_orientation(); // change set state to reflect change in cube (different front face, orient, or permutation) and Update_pieces()
        void Append_move_buffer(std::vector<std::vector<int>> alg, std::vector<int> _front_faces, std::vector<int> _orients);
        void Clear_buffer(); // clear move_buffer, front_faces, and orients
        void Finish_move(); // finish ongoing and buffered moves (skips animation)
        void Replay(); // 

        // these 2 functions are used by UI only:
        std::vector<int> get_alg(); // get currently executed alg joined in one vector
        int get_move_index(); // get buffer_index as if alg was one set

        void Set_front_face(int val); // set front_face (once move_buffer empty)
        void Set_orient(int val); // set orient (once move_buffer empty)

        void Set_orbital(bool val); // spin cube around

    private:
        void new_orientation(); // set front_face and orient to respective "new" variables (if they != -1 and move_buffer is empty)

        Camera camera;
        int camera_mode;
        bool orbital;
        bool fast_forward; // is button or key being pressed to fast-forward

        RenderTexture2D renderTexture;
        Cube* cube;
        std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> orig_state; // state w/o change of front face and orientation
        std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> state; // state w/ change of front face and orientation
        // backup of state before last alg. used to replay last alg
        std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> last_orig_state; // last state w/o change of front face and orientation
        std::array<std::array<int, CUBE_FACE_SIZE>, CUBE_SIZE> last_state; // last state w/ change of front face and orientation

        // position of all pieces (before rotations)
        std::array<Vector3, CUBE_FACE_SIZE * 3> piece_pos; 
        std::array<std::array<Color, CUBE_SIZE>, CUBE_FACE_SIZE * 3> piece_cols; // positions of all pieces

        // these variables act as check if front face or orient is changed, so state is changed
        int front_face;
        int orient;
        int front_face_new; // new front_face to be set (once move_buffer empty)
        int orient_new; // new orient to be set (once move_buffer empty)

        // everu set (moves in same position) in each algorithm is animated
        // every algorithm is animated as well
        // once finished, move_buffer is reset
        std::vector<std::vector<std::vector<int>>> move_buffer; // buffered moves to be executed (animated)
        std::vector<std::vector<int>> front_faces; // front_face per set in each alg in move_buffer
        std::vector<std::vector<int>> orients; // orient per set in each alg in move_buffer
        // backup of move_buffer, front_faces, and orients before last alg. used to replay last alg
        std::vector<std::vector<int>> last_alg; // last executed alg of move_buffer
        std::vector<int> last_front_faces; // front_faces of last alg
        std::vector<int> last_orients; // orients of last alg
        int alg_index; // current ongoing algorithm
        int set_index; // current ongoing set in alg
        int buffer_index; // current ongoing move by index (within alg)
        int cur_move; // current ongoing move (-1 = none)
        float angle; // current rotating angle (only used if move ongoing)
        float angle_increment; // change in angle per frame (if moving)
};

Color Cube3D_get_Colour(int cube_col); // converts Cube colour to Raylib colour
#endif
