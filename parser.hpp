/**
           图形化编程语言
           分析图片里的符号
*/
class enum SYMBOL
{
    ARROW,
    DOT,
    CIRCLE,
    ROUND
}
bool reached(int x, int y)
{
    // todo
}
symbol getsymbol(int x, int y)
{
    symbol s;
    // todo
    return s;
}
void parser(const image& img)
{
    for(int i = 0; i < img.width; i ++)
    for(int j = 0; j < img.height; j ++)
    {
        if(reached(i, j))
            continue;
        pixel p = img(i, j);
        symbol s = getsymbol(i, j);
        if(s.type == SYMBOL::DOT)
        {
        }
        else if(s.type == SYMBOL::ARROW)
        {
        }
        else if(s.type == SYMBOL::CIRCLE)
        {
        }
    }
}
