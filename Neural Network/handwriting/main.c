#include "acllib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h> 

static int mat[32][32];
static int mousedown = 0;
static int recog = -1;
static int clearDown = 0;
static int recogDown = 0;

void refreshPainter(){
    beginPaint();

    setBrushColor(WHITE);
    setPenColor(BLACK);
    setPenWidth(3);
    rectangle(120, 90, 440, 410);

    setPenWidth(0);
    setBrushColor(BLACK);
    for(int x = 0; x < 32; x++)
        for(int y = 0; y < 32; y++){
            if(mat[x][y]) rectangle(120 + x * 10, 90 + y * 10, 130 + x * 10, 100 + y * 10);
        }
    
    setPenWidth(3);
    setBrushColor(WHITE);
    setTextBkColor(EMPTY);
    setTextColor(BLACK);
    setTextSize(120);
    rectangle(580, 120, 680, 220);
    static char tmp[2];
    sprintf(tmp, "%d", recog);
    if(recog == -2) paintText(600, 110, "E");
    else if(recog >= 0) paintText(600, 110, tmp);

    setTextSize(30);
    if(clearDown){
        setBrushColor(BLACK);
        setTextColor(WHITE);
    } else {
        setBrushColor(WHITE);
        setTextColor(BLACK);
    }
    rectangle(520, 320, 600, 370);
    paintText(530, 330, "���");

    if(recogDown){
        setBrushColor(BLACK);
        setTextColor(WHITE);
    } else {
        setBrushColor(WHITE);
        setTextColor(BLACK);
    }
    rectangle(660, 320, 740, 370);
    paintText(670, 330, "ʶ��");

    endPaint();
}

void clearPainter(){
    memset(mat, 0, sizeof(mat));
    recog = -1;
}

void setBlack(int x, int y){
    if(x >= 0 && x < 32 && y >= 0 && y < 32)
        mat[x][y] = 1;
}

void setBlackRec(int x, int y){
    for(int i = x - 1; i <= x + 1; i++)
        for(int j = y - 1; j <= y + 1; j++)
            setBlack(i, j);
}

void recognize(){

    FILE* inp = fopen("test.txt", "w");
    for(int i = 0; i < 32; i++){
        for(int j = 0; j < 32; j++)
            fprintf(inp, "%d", mat[j][i]);
        fprintf(inp, "\n");
    }
    fclose(inp);

    system("python predictor.py");
    FILE* oup = fopen("result.txt", "r");
    fscanf(oup, "%d", &recog);
    fclose(oup);

    system("del test.txt");
    system("del result.txt");
}

void mouseEvent(int x, int y, int key, int event){
    if(event == BUTTON_DOWN && key == LEFT_BUTTON){
        mousedown = 1;
        if(x >= 520 && x <= 600 && y >= 320 && y <= 370)
            clearDown = 1, clearPainter();
        if(x >= 660 && x <= 740 && y >= 320 && y <= 370)
            recogDown = 1;
    }
    if(event == BUTTON_UP && key == LEFT_BUTTON){
        mousedown = 0;
        if(x >= 520 && x <= 600 && y >= 320 && y <= 370)
            clearDown = 0;
        if(x >= 660 && x <= 740 && y >= 320 && y <= 370)
            recogDown = 0, recognize();
    }
    if(x >= 120 && x <= 440 && y >= 90 && y <= 410 && mousedown){
        setBlackRec((x - 120) / 10, (y - 90) / 10);
    }
    refreshPainter();
}

int Setup(){
    initWindow("��д����ʶ����", 200, 200, 809, 500);
    refreshPainter();
    registerMouseEvent(mouseEvent);
    return 0;
}