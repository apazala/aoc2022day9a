#include <cstdio>
#include <unordered_set>
#include <unordered_map>

int loadInputFile();
int nextLine(char *dst);

using std::unordered_set;
using std::unordered_map;

struct position
{
    int x;
    int y;

    position():x(0),y(0){}
    position(int x, int y):x(x),y(y){}

    void move(const position& d)
    {
        x+=d.x;
        y+=d.y;
    }

    bool operator==(const position& p) const
    {
        return (x == p.x) && (y == p.y);
    }

    position operator-(const position& p)const
    {
        return {x-p.x, y-p.y};
    }
};

struct positionHash { 
    size_t operator()(const position& p) const
    {
        return p.x*31+p.y;
    }
};

int main()
{
    char line[16];

    if(loadInputFile() < 0){
        fprintf(stderr, "Can't open: input.txt\n");
        return 1;
    }


    unordered_map<char, position> directionMovesMap;
    directionMovesMap['R'] = {1, 0};
    directionMovesMap['L'] = {-1, 0};
    directionMovesMap['U'] = {0, 1};
    directionMovesMap['D'] = {0, -1};
    

    position tail = {0, 0};
    position head = {0, 0};
    unordered_set<position, positionHash> visitedTailPositions;
    visitedTailPositions.insert(tail);

    unordered_map<position, position, positionHash> tailCorrectionMovesMap;
    for(int x = -1; x <= 1; x++)
        for(int y = -1; y <= 1; y++)
            tailCorrectionMovesMap[position(x, y)] = {0, 0};
    
    for(int y = -1; y <= 1; y++)
        tailCorrectionMovesMap[position(2, y)] = {1, y};
    for(int y = -1; y <= 1; y++)
        tailCorrectionMovesMap[position(-2, y)] = {-1, y};
    for(int x = -1; x <= 1; x++)
        tailCorrectionMovesMap[position(x, 2)] = {x, 1};
    for(int x = -1; x <= 1; x++)
        tailCorrectionMovesMap[position(x, -2)] = {x, -1};
    

    position headDir, tailDir;
    int n;
    char dirChar;
    while(nextLine(line)){
        if(sscanf(line, " %c %d", &dirChar, &n) != 2) break;

        headDir = directionMovesMap[dirChar];
        while(n--){
            head.move(headDir);
            tailDir = tailCorrectionMovesMap[head-tail];
            tail.move(tailDir);
            visitedTailPositions.insert(tail);
        }
    }

    printf("%zu\n", visitedTailPositions.size());

}

#define BUFSIZE 1 << 14

char fileBuf[BUFSIZE];
char *pbuf = fileBuf;
char *pbufend = fileBuf;

int loadInputFile()
{
    FILE *pf = fopen("input.txt", "r");
    if (!pf)
        return -1;
    pbuf = fileBuf;
    pbufend = fileBuf + fread(fileBuf, 1, BUFSIZE, pf);
    fclose(pf);

    return 0;
}

int nextLine(char *dst)
{
    char c;
    char *orig = dst;

    // read line:
    while (pbuf < pbufend && (c = *(pbuf++)) != '\n')
    {
        *(dst++) = c;
    }
    *dst = '\0';
    // return line length
    return dst - orig;
}