
#include "BlockObject.h"

INT g_pos[] = { 1250, 1500, 1750, 2000, 2250, 2500 };


BlockObject::BlockObject()
{
    is_back_ = false;
}

BlockObject::~BlockObject()
{

}


bool BlockObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImageFile(path, screen);
    return ret;
}

void BlockObject::SetPos(const int& xp, const int& yp)
{
    this->SetRect(xp, yp);
}

void BlockObject::SetXPos(const int& xp)
{
    SDL_Rect rect = this->GetRect();
    this->SetRect(xp, rect.y);
}

void BlockObject::ShowImg(SDL_Renderer* screen)
{
    this->Render(screen);
}

void BlockObject::DoRun(UINT& x_val)
{
    this->rect_.x += x_val;
    if (rect_.x + rect_.w < 0)
    {
        is_back_ = true;
    }
}

DoubleBlock::DoubleBlock()
{
    x_val_ = -3;
    is_back_ = false;
    is_pass_ = false;
}

DoubleBlock::~DoubleBlock()
{

}

bool DoubleBlock::InitBlock(SDL_Renderer* screen, const int& xp)
{
    int number = SDLCommonFunc::MyRandom(1, 10);
    int number2 = number;
    number = number * 12;

    bool ret1 = m_Topblock.LoadImg("img//topblock.png", screen);
    bool ret2 = m_BottomBlock.LoadImg("img//botblock.png", screen);

    if (number2 % 2 == true)
    {
        m_Topblock.SetPos(xp, -300 - number);
        m_BottomBlock.SetPos(xp, GROUND_MAP - 220 - number);
    }
    else
    {
        m_Topblock.SetPos(xp, -300 + number);
        m_BottomBlock.SetPos(xp, GROUND_MAP - 220 + number);
    }

    if (ret1 && ret2)
        return true;
    return false;
}

void DoubleBlock::RenderImg(SDL_Renderer* screen)
{
    m_Topblock.ShowImg(screen);
    m_BottomBlock.ShowImg(screen);
}

void DoubleBlock::DoMoving()
{
    m_Topblock.DoRun(x_val_);
    m_BottomBlock.DoRun(x_val_);
    if (m_Topblock.GetStateBack() || m_BottomBlock.GetStateBack())
    {
        is_back_ = true;
    }
}

SDL_Rect DoubleBlock::GetTopRect()
{
    return m_Topblock.GetRectPos();
}

void DoubleBlock::SetRectVal(const UINT& xp)
{
    m_Topblock.SetXPos(xp);
    m_BottomBlock.SetXPos(xp);
}

void DoubleBlock::SetIsBack(bool isBack)
{
    is_back_ = isBack;
    m_Topblock.SetBack(isBack);
    m_BottomBlock.SetBack(isBack);
}

void DoubleBlock::GetRectSlot()
{
    SDL_Rect rect_top = m_Topblock.GetRectPos();

    slot_rect_.x = rect_top.x + rect_top.w;
    slot_rect_.y = rect_top.y + rect_top.h;
    slot_rect_.w = 5;
    slot_rect_.h = 160;
}

void DoubleBlock::DrawBound(SDL_Renderer* des)
{
    GeometricFormat outlie_size(slot_rect_.x, slot_rect_.y, slot_rect_.w, slot_rect_.h);
    ColorData color_data1(255, 255, 255);
    Gemometric::RenderOutline(outlie_size, color_data1, des);
}

bool DoubleBlock::CheckPass(SDL_Rect rect)
{
    bool ret = false;
    ret = SDLCommonFunc::CheckCollision(rect, slot_rect_);
    return ret;
}

bool DoubleBlock::CheckCol(SDL_Rect pl_rect)
{
    bool ret1 = SDLCommonFunc::CheckCollision(pl_rect, m_Topblock.GetRectPos());
    bool ret2 = SDLCommonFunc::CheckCollision(pl_rect, m_BottomBlock.GetRectPos());

    if (ret1 || ret2)
    {
        return true;
    }

    return false;
}

//manage blocks

BlockManager::BlockManager()
{
    xp_max_ = 0;         //maximum number of blocks
    stop_moving_ = false;//stop_moving if need
    m_count = 0;         //score
    is_col_ = false;     //checkCollision
}

BlockManager::~BlockManager()
{
    FreeBlock();
}

void BlockManager::FreeBlock()
{
    for (int i = 0; i < m_BlockList.size(); i++)
    {
        DoubleBlock* pBlock = m_BlockList.at(i);
        delete pBlock;
        pBlock = NULL;
    }

    m_BlockList.clear();
}

bool BlockManager::InitBlockList(SDL_Renderer* screen)
{
    DoubleBlock* m_block1 = new DoubleBlock();
    DoubleBlock* m_block2 = new DoubleBlock();
    DoubleBlock* m_block3 = new DoubleBlock();
    DoubleBlock* m_block4 = new DoubleBlock();
    DoubleBlock* m_block5 = new DoubleBlock();
    DoubleBlock* m_block6 = new DoubleBlock();

    int ret = m_block1->InitBlock(screen, g_pos[0]);
    if (ret == false) return false;
    ret = m_block2->InitBlock(screen, g_pos[1]);
    if (ret == false) return false;
    ret = m_block3->InitBlock(screen, g_pos[2]);
    if (ret == false) return false;
    ret = m_block4->InitBlock(screen, g_pos[3]);
    if (ret == false) return false;
    ret = m_block5->InitBlock(screen, g_pos[4]);
    if (ret == false) return false;
    ret = m_block6->InitBlock(screen, g_pos[5]);
    if (ret == false) return false;

    m_BlockList.push_back(m_block1);
    m_BlockList.push_back(m_block2);
    m_BlockList.push_back(m_block3);
    m_BlockList.push_back(m_block4);
    m_BlockList.push_back(m_block5);
    m_BlockList.push_back(m_block6);

    xp_max_ = 5;
    return true;
}

void BlockManager::SetStopMoving(const bool& stop)
{
    stop_moving_ = stop;
}

void BlockManager::Render(SDL_Renderer* screen)
{
    for (int i = 0; i < m_BlockList.size(); i++)
    {
        DoubleBlock* pBlock = m_BlockList.at(i);

        pBlock->GetRectSlot();
        //pBlock->DrawBound(screen);

        if (!stop_moving_)
        {
            pBlock->DoMoving();

            bool ret = pBlock->GetIsBack();
            if (ret == true)
            {
                DoubleBlock* endBlock = m_BlockList.at(xp_max_);
                SDL_Rect end_rect = endBlock->GetTopRect();
                UINT xp = end_rect.x + 250;
                pBlock->SetRectVal(xp);
                pBlock->SetIsBack(false);
                pBlock->SetIsPass(false);
                xp_max_ = i;
            }

            bool isCol = pBlock->CheckCol(player_rect_);
            if (isCol == true)
            {
                is_col_ = true;
                Mix_Chunk* beep_sound = Mix_LoadWAV("sound//punch.wav");
                if (beep_sound != NULL)
                    Mix_PlayChannel(-1, beep_sound, 0);
                break;
            }

            ret = pBlock->CheckPass(player_rect_);
            if (ret == true)
            {
                if (pBlock->GetIsPass() == false)
                {
                    Mix_Chunk* beep_sound = Mix_LoadWAV("sound//ting.wav");
                    if (beep_sound != NULL)
                        Mix_PlayChannel(-1, beep_sound, 0);
                    m_count++;
                    pBlock->SetIsPass(true);
                }
            }
        }

        pBlock->RenderImg(screen);

    }
}