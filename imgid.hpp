/**
*               图像内的符号识别
*               根据路径积分以及规范场原理：
*               向量在一个周期内移动，进行特
*               征采集在周期开始与结束两端比
*               较向量，一种局部特征提取
*/
#define GETRED(rgb)     GetBValue(rgb)
#define GETGREEN(rgb)   GetGValue(rgb)
#define GETBLUE(rgb)    GetRValue(rgb)
scope imageid
{
    static int SIZEX = 256;						// 区域尺度
    static int SIZEY = 256;
    static CDymPng png;
    static char* hismap = 0;
    static int cycle_depth = 18;                 // 周期
    static int wavlen = 10;                      // 周期

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
            y = png.Height() - 1 - y; // 上下颠倒
            return png.gifGetPixel(x, y);
        }
        return 0;
    }
    int distance(int x1, int x2, int y1, int y2)
    {
        return (int)sqrt((x1 - x2)* (x1 - x2) + (y1 - y2) * (y1 - y2));
    }
    real angle(int x1, int x2, int y1, int y2)
    {
        vec2 v1(x1, y1);
        vec2 v2(x2, y2);
        real ang = dot(v1.normcopy(),v1.normcopy());
        return ang;
    }
    bool isend(int x, int y)
    {
        return true;
    }
    bool isarrow(int x, int y)
    {
        return true;
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
                                if(isend(ix, iy))
                                    return true;
                                if(isarrow(ix, iy))
                                    return true;
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
