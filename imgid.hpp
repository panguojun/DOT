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
    static int SIZEX = 256;						 // 区域尺度
    static int SIZEY = 256;
    static int cycle_depth = 888;                // 周期
    static int wavlen = 28;                      // 波长

    static CDymPng png;
    static char* hismap = 0;

    bool walk(int ix, int iy, int depth, int phase, std::function<bool(int ix, int iy)> fun)
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
            walk(ix + 1, iy, depth + 1, phase + 1, fun);
            walk(ix, iy + 1, depth + 1, phase + 1, fun);
            walk(ix - 1, iy, depth + 1, phase + 1, fun);
            walk(ix, iy - 1, depth + 1, phase + 1, fun);
            walk(ix - 1, iy - 1, depth + 1, phase + 1, fun);
            walk(ix + 1, iy - 1, depth + 1, phase + 1, fun);
            walk(ix - 1, iy + 1, depth + 1, phase + 1, fun);
            walk(ix + 1, iy + 1, depth + 1, phase + 1, fun);
        }
        return false;
    }
    void clearhistory()
    {
        memset(hismap, 0, SIZEX * SIZEY);
    }
    // ---------------------------------
    // image
    // ---------------------------------
    int get_pixel(int x, int y)
    {
        x < 0 ? x = 0 : 0;
        y < 0 ? y = 0 : 0;
        x > png.Width()  ? x = png.Width() : 0;
        y > png.Height() ? y = png.Height() : 0;
        {
            y = png.Height() - 1 - y; // 上下颠倒
            return png.gifGetPixel(x, y);
        }
        return 0;
    }
    bool hittest(int x, int y)
    {
        return GETRED(get_pixel(x, y)) > 50;
    }
    // ---------------------------------
    // math
    // ---------------------------------
    int distance(int x1, int y1, int x2, int y2)
    {
        return (int)sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }
    real angle(int x1, int x2, int y1, int y2)
    {
        vec2 v1(x1, y1);
        vec2 v2(x2, y2);
        real ang = dot(v1.normcopy(),v2.normcopy());
        return ang;
    }
    int igrad(int x, int y)
    {
        int con_num = 0;
        con_num += hittest(x - 1, y);
        con_num += hittest(x, y - 1);
        con_num += hittest(x + 1, y);
        con_num += hittest(x, y + 1);
        con_num += hittest(x - 1, y - 1);
        con_num += hittest(x + 1, y - 1);
        con_num += hittest(x - 1, y + 1);
        con_num += hittest(x + 1, y + 1);
       
        return con_num;
    }
    // ---------------------------------
    // logic
    // ---------------------------------
    bool isend(int x, int y)
    {
        return igrad(x, y) < 2;
    }
    bool isarrow(int x, int y)
    {
        return igrad(x, y) >= 4;
    }
    bool iscircle(int x, int y)
    {
        return true;
    }
    real calcsize(vector<vec2> plist)
    {
        real mdis = 0;
        for (crauto em1 : plist)
        for (crauto em2 : plist)
        {
            real dis = (em1 - em2).len();
            if (dis > mdis)
            {
                mdis = dis;
            }
        }
    }
    // ---------------------------------
    // walk
    // 几何，代数，周期性, PI, E
    // ---------------------------------
    void walk()
    {
        clearhistory();
        if (png.Width() == 0)
        {
           ASSERT(png.LoadFromFileA("C:/Users/18858/Desktop/test.png"))
        }
        for (int i = 1; i < SIZEX; i++) {
            for (int j = 1; j < SIZEY; j++)
            {
                if (hismap[j * SIZEX + i] == 0 && hittest(i, j))
                {
                    vector<vec2> endpoints, arrows;
                    PRINT("walk ------------------ ");
                    int cor = RNDCOR;
                    int x0 = i, y0 = j;
                    walk(i, j, 0, 0, [&x0, &y0, &endpoints, &arrows, cor](int ix, int iy) ->bool {
                        if (hittest(ix, iy))
                        {
                            if (isend(ix, iy))
                            {
                               // PRINT("END");
                                pointi(ix, iy, 2, 0xFF00FFff);
                                endpoints.emplace_back(ix, iy);
                               // if (distance(x0, y0, ix, iy) > wavlen)
                                 //   return false;
                            }
                            if (isarrow(ix, iy))
                            {
                                //PRINT("ARROW " << ix << "," << iy);
                                arrows.emplace_back(ix, iy);
                                pointi(ix, iy, 2, 0xFFFF00ff);
                               // if (distance(x0, y0, ix, iy) > wavlen)
                                //    return false;
                            }
                           // PRINT(ix <<","<<iy);
                            pixel(ix, iy, cor);
                            return true;
                        }
                        return false;
                    });
                    {
                        if (!endpoints.empty() && !arrows.empty())
                        {
                            for (int j = 0; j < arrows.size(); j++)
                            {
                                real mdis = 0;
                                int index = 0;
                                for (int i = 0; i < endpoints.size(); i++)
                                {
                                    crauto em = endpoints[i];
                                    real dis = (em - arrows[j]).len();
                                    if (dis > mdis)
                                    {
                                        index = i;
                                        mdis = dis;
                                    }
                                }
                                if (mdis > wavlen)
                                {
                                    drawline(endpoints[index] / IMAGESCALE, arrows[j] / IMAGESCALE, 0xFF00FF00);
                                    PRINT("============ARROW " << endpoints[index].x << "," << endpoints[index].y << "->" << arrows[j].x << "," << arrows[j].y);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
