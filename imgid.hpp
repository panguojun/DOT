/**
*               ͼ���ڵķ���ʶ��
*               ����·�������Լ��淶��ԭ��
*               ������һ���������ƶ���������
*               ���ɼ������ڿ�ʼ��������˱�
*               ��������һ�־ֲ�������ȡ
*/
#define GETRED(rgb)     GetBValue(rgb)
#define GETGREEN(rgb)   GetGValue(rgb)
#define GETBLUE(rgb)    GetRValue(rgb)
scope imageid
{
    static int SIZEX = 256;						// ����߶�
    static int SIZEY = 256;
    static CDymPng png;
    static char* hismap = 0;
    static int cycle_depth = 18;                 // ����
    static int wavlen = 10;                      // ����

    bool walk(rvec2 v, int ix, int iy, int depth, int phase, std::function<bool(int ix, int iy)> fun)
    {
        if (ix < 0 || ix > SIZEX - 1)
        {
            return false;
        }
        if (iy < 0 || iy > SIZEY - 1)
        {
            return false;
        }

        if (hismap[iy * SIZEX + ix] != 0)
            return false;
        hismap[iy * SIZEX + ix] = 1;

        if (fun && !fun(ix, iy))
        {
            return false;
        }
        if (depth + 1 < cycle_depth)
        {
            vec2 nv;
            walk(nv, ix + 1, iy, depth + 1, phase + 1, fun);
            walk(nv, ix, iy + 1, depth + 1, phase + 1, fun);
            walk(nv, ix - 1, iy, depth + 1, phase + 1, fun);
            walk(nv, ix, iy - 1, depth + 1, phase + 1, fun);

            v = nv;
        }
        return false;
    }
    void clearhistory()
    {
        memset(hismap, 0, SIZEX * SIZEY);
    }
    int getPixelCor(int x, int y)
    {
        if (png.Width() > 0)
        {
            y = png.Height() - 1 - y; // ���µߵ�
            return png.gifGetPixel(x, y);
        }
        return 0;
    }
    int distance(int x1, int x2, int y1, int y2)
    {
        return (int)sqrt((x1 - x2)* (x1 - x2) + (y1 - y2) * (y1 - y2));
    }
    void walk()
    {
        clearhistory();

        if (png.Width() == 0)
        {
           ASSERT( png.LoadFromFileA("C:/Users/18858/Desktop/test.png"))
        }

        for (int i = 1; i < SIZEX; i++) {
            for (int j = 1; j < SIZEY; j++)
            {
                int cor = getPixelCor(i, j);
                if (GETRED(cor) >= 50)
                {
                    vec2 v;
                    walk(v, i, j, 0, 0, [i, j](int ix, int iy)->bool {
                            int cor = getPixelCor(ix, iy);
                            if (GETRED(cor) >= 50)
                            {
                                if (distance(i, j, ix, iy) > wavlen)
                                    return false;
                                pixel(ix, iy, 0xFF00FFff);
                                return true;
                            }
                            return false;
                        });
                }
            }
        }
    }
}