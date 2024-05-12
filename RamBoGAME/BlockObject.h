
#ifndef BLOCK_OBJECT_H_
#define BLOCK_OBJECT_H_

#include "Geometric.h"

class BlockObject : BaseObject
{
public:
    BlockObject();
    ~BlockObject();
    bool LoadImg(std::string path, SDL_Renderer* screen);
    void SetPos(const int& xp, const int& yp);
    void SetXPos(const int& xp);
    SDL_Rect GetRectPos() const { return this->GetRect(); }
    void ShowImg(SDL_Renderer* screen);
    void DoRun(UINT& x_val);
    bool GetStateBack() const { return is_back_; }
    void SetBack(bool b) { is_back_ = b; }
private:
    bool is_back_;
    int x_pos_;
    int y_pos_;
};



class DoubleBlock
{
public:
    DoubleBlock();
    ~DoubleBlock();
    bool InitBlock(SDL_Renderer* screen, const int& xp);
    void RenderImg(SDL_Renderer* screen);
    void DoMoving();
    bool GetIsBack() const { return is_back_; }
    void SetIsBack(bool isBack);
    SDL_Rect GetTopRect();
    void SetRectVal(const UINT& xp);
    void GetRectSlot();
    void DrawBound(SDL_Renderer* des);
    bool CheckPass(SDL_Rect rect);
    bool GetIsPass() const { return is_pass_; }
    void SetIsPass(const bool& pp) { is_pass_ = pp; }
    bool CheckCol(SDL_Rect pl_rect);
private:
    BlockObject m_Topblock;
    BlockObject m_BottomBlock;
    UINT x_val_;
    bool is_back_;
    SDL_Rect slot_rect_;
    bool is_pass_;
};


class BlockManager
{
public:
    BlockManager();
    ~BlockManager();

    bool InitBlockList(SDL_Renderer* screen);
    void Render(SDL_Renderer* screen);
    void SetStopMoving(const bool& stop);
    void SetPlayerRect(SDL_Rect pRect) { player_rect_ = pRect; }
    bool GetColState() const { return is_col_; }
    INT GetCount() const { return m_count; }
    void FreeBlock();
private:
    std::vector<DoubleBlock*> m_BlockList;
    UINT xp_max_;
    bool stop_moving_;
    SDL_Rect player_rect_;
    INT m_count;
    bool is_col_;
};
#endif