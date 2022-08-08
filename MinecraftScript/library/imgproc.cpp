#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath> 
#include <opencv2/opencv.hpp>
#include <cstdint>
#include <cstring>
#include <vector>
#include <tiffio.h>
#include <string>
#include <sstream>
#include <windows.h>
#include <dirent.h>
using namespace cv;
using namespace std;

struct rect{
    int left;
    int right;
    int top;
    int bottom;
};

struct  point{
    int x;
    int y;
};

BITMAPINFOHEADER createBitmapHeader(int width, int height)
{
    BITMAPINFOHEADER bi;

    // create a bitmap
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height; //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    return bi;
}
// https://superkogito.github.io/blog/CaptureScreenUsingGdiplus.html
void ScreenShot(std::vector<uint8_t>& Pixels, int& BitsPerPixel, int width, int height, int dx, int dy){
    Mat src;
    HWND hwnd = FindWindow(NULL, (LPCSTR)"Minecraft* 1.18.2 - 多人遊戲（第三方伺服器）");
    RECT MSwh;
    GetWindowRect(hwnd, &MSwh);
    // get handles to a device context (DC)
    HDC hwindowDC = GetDC(hwnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    // define scale, height and width
    int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
    // int Fullwidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    // int Fullheight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    width = MIN(width, MSwh.right - MSwh.left);
    height = MIN(height, MSwh.bottom - MSwh.top);
    // cout << MSwh.right - MSwh.left << " " << MSwh.bottom - MSwh.top << "\n";
    // create mat object
    src.create(height, width, CV_8UC4);

    // create a bitmap
    HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    BITMAPINFOHEADER bi = createBitmapHeader(width, height);

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);

    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx + dx, screeny + dy, width, height, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    // GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, Pixels, (BITMAPINFO *)&bi, DIB_RGB_COLORS);                      //copy from hwindowCompatibleDC to hbwindow
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS); //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    BitsPerPixel = src.channels() * 8;
    Pixels.resize(width * height * src.channels());
    Pixels.assign(src.data, src.data + src.total()*src.channels());
    // imshow("Display P", Mat(height, width, src.channels() * 8 > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]));
    // waitKey(0);
}

int GetEigenvalue(vector<uint8_t>& Pixels, int width, int height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3, e = 0;
    for (int j = 0; j < height; j++) {
        for(int i=0;i<width;i++){
            for(int k=0;k<3;k++){
                e += Pixels[i*bit + j*width*bit + k];
            }
        }
    }
    return e;
}

void twoValue(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& editPixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3, tmp;
    editPixels.resize(Width * Height * bit);
    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            tmp = 0;
            for(int k=0;k<3;k++){
                tmp += Pixels[i*bit + j*Width*bit + k];
            }
            tmp /= 3;
            for(int k=0;k<3;k++){
                if(tmp > 200) editPixels[i*bit + j*Width*bit + k] = 255;
                else editPixels[i*bit + j*Width*bit + k] = 0;
            }
        }
    }
}

void cutImg(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& cutPixels, int Width, int Height, int BitsPerPixel, int cutWidth, int cutHeight, int posx, int posy){
    int bit = BitsPerPixel > 24 ? 4 : 3, idx=0;
    cutPixels.resize(cutWidth * cutHeight * bit);
    for(int j=0;j<cutHeight;j++){
        for(int i=0;i<cutWidth;i++){
            for(int k=0;k<3;k++){
                cutPixels[idx++] = Pixels[(posx+i)*bit + (posy+j)*Width*bit + k];
            }
            if(bit == 4)cutPixels[idx++] = 0;
        }
    }
}


void GaussBlur(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& blurPixels, int Width, int Height, int BitsPerPixel, float s){
    int r = 3*s+0.5, w, p, q, bit = BitsPerPixel > 24 ? 4 : 3;
    w = 2*r+1;
    float G[w], tmp;
    for(int i=0;i<w;i++){
        G[i] = pow(M_E, -((r-i)*(r-i))/(2*s*s))/sqrt(2*M_PI*s*s);
    }
    blurPixels.resize(Width * Height * bit);
    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            for(int k=0;k<3;k++){
                blurPixels[i*bit + j*Width*bit + k] = 0;
                tmp = 0.0;
                for(int n=0;n<w;n++){
                    q = j+n-r;
                    if(q<0) q=-q;
                    else if(q>=Height) q=2*Height-q-2;
                    tmp += G[n] * (float)Pixels[i*bit + q*Width*bit + k];
                }
                blurPixels[i*bit + j*Width*bit + k] = MAX(MIN(tmp, 255), 0);
            }
        }
    }
    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            for(int k=0;k<3;k++){
                tmp = 0.0;
                for(int n=0;n<w;n++){
                    p = i+n-r;
                    if(p<0) p=-p;
                    else if(p>=Width) p=2*Width-p-2;
                    tmp += G[n] * (float)blurPixels[p*bit + j*Width*bit + k];
                }
                blurPixels[i*bit + j*Width*bit + k] = MAX(MIN(tmp, 255), 0);
            }
        }
    }
}

void showedge(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& egdePixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3, Gx[3], Gy[3];
    int Gmx[3][3] = {{1,0,-1},{2,0,-2},{1,0,-1}}, Gmy[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
    float L[3] , theta[3], critical = 100;
    bool Overcritical;

    if(!(Width * Height))return;
    GaussBlur(Pixels, egdePixels, Width, Height, BitsPerPixel, 0.6);
    // egdePixels.resize(Width * Height * bit);
    for(int y=1;y<Height-1;y++){
        for(int x=1;x<Width-1;x++){
            for(int k=0;k<3;k++)Gx[k] = Gy[k] = 0;
            Overcritical = 0;
            for(int j=0;j<3;j++){
                for(int i=0;i<3;i++){
                    for(int k=0;k<3;k++){
                        Gx[k] += Pixels[(i+x-1)*bit + (j+y-1)*Width*bit + k] * Gmx[j][i];
                        Gy[k] += Pixels[(i+x-1)*bit + (j+y-1)*Width*bit + k] * Gmy[j][i];
                    }
                }
            }
            
            for(int k=0;k<3;k++){
                L[k] = sqrt(Gx[k]*Gx[k] + Gy[k]*Gy[k]);
                theta[k] = atan2(Gy[k], Gx[k]);
                // cout<<L[k]<<" ";
                Overcritical |= L[k] > critical;
            }

            for(int k=0;k<3;k++) egdePixels[x*bit + y*Width*bit + k] = L[k] > critical ? 255 : 0;
            // for(int k=0;k<3;k++) egdePixels[x*bit + y*Width*bit + k] = MIN(MAX(255.0 * pow((L[k] + (k==2?0:0)) / 600.0, 1), 0), 255);
            if(!(egdePixels[x*bit + y*Width*bit + 0] == egdePixels[x*bit + y*Width*bit + 1] && egdePixels[x*bit + y*Width*bit + 1] == egdePixels[x*bit + y*Width*bit + 2] && egdePixels[x*bit + y*Width*bit + 2] == 255)){
                for(int k=0;k<3;k++)egdePixels[x*bit + y*Width*bit + k] = 0;
            }
            // for(int k=0;k<3;k++) egdePixels[x*bit + y*Width*bit + k] = Overcritical ? 255 : 0;
        }
    }
    // imshow("Display edge", Mat(Height, Width, bit > 24 ? CV_8UC4 : CV_8UC3, &egdePixels[0]));
    // waitKey(0);
}

void findarrow(std::vector<uint8_t>& Pixels, int Width, int Height, int BitsPerPixel, int way[4]){
    int bit = BitsPerPixel > 24 ? 4 : 3, idx=0, dy, dx, mdx, mdy, wi = 0;
    float pointNum, judgeL, judgeR, judgeT, judgeB, symX, symY;
    bool isreced, rangeup;
    std::vector<uint8_t> arrowPixels;
    std::vector<struct rect> dots;
    std::vector<int> dotsX, dotsY;

    arrowPixels.resize(Width*Height*bit);
    for(int y=0;y<Height;y++){
        for(int x=0;x<Width;x++){
            for(int k=0;k<3;k++) arrowPixels[x*bit + y*Width*bit + k] = 255;
        }
    }

    for(int y=8;y<Height-8;y++){
        for(int x=8;x<Width-8;x++){
            if(Pixels[x*bit + y*Width*bit] == 0){
                pointNum = 0;
                for(int j=0;j<=16;j++){
                    for(int i=0;i<=16;i++){
                        if(Pixels[(x+i-8)*bit + (y+j-8)*Width*bit] == 0) {
                            pointNum += 16-abs(i-8)-abs(j-8);
                        }
                    }
                }
                
                if(pointNum > 1900){
                    for(int j=0;j<=16;j++){
                        for(int i=0;i<=16;i++){
                            for(int k=0;k<3;k++)arrowPixels[(x+i-8)*bit + (y+j-8)*Width*bit + k] = Pixels[(x+i-8)*bit + (y+j-8)*Width*bit + k];
                        }
                    }
                }
            }
        }
    }
    
    for (int x = 1; x < Width - 1; x++)
    {
        for(int y=1;y<Height-1;y++){
            if(arrowPixels[x*bit + y*Width*bit] != 0){
                if(arrowPixels[(x+1)*bit + y*Width*bit] == 0 && arrowPixels[(x-1)*bit + y*Width*bit] == 0) arrowPixels[x*bit + y*Width*bit] = 0;
                if(arrowPixels[(x+1)*bit + (y+1)*Width*bit] == 0 && arrowPixels[(x-1)*bit + (y-1)*Width*bit] == 0) arrowPixels[x*bit + y*Width*bit] = 0;
                if(arrowPixels[x*bit + (y+1)*Width*bit] == 0 && arrowPixels[x*bit + (y-1)*Width*bit] == 0) arrowPixels[x*bit + y*Width*bit] = 0;
                if(arrowPixels[(x+1)*bit + (y-1)*Width*bit] == 0 && arrowPixels[(x-1)*bit + (y+1)*Width*bit] == 0) arrowPixels[x*bit + y*Width*bit] = 0;
            }
        }
    }

    for(int x=1;x<Width-1;x++){
        for(int y=1;y<Height-1;y++){
            if(arrowPixels[x*bit + y*Width*bit] == 0){
                isreced = 0;
                for(int i=0;i<idx;i++){
                    isreced |= (x >= dots[i].left) && (x <= dots[i].right) && (y >= dots[i].top) && (y <= dots[i].bottom);
                }
                if (isreced) continue;
                mdx = mdy = dx = dy = 0;
                while(arrowPixels[(x+dx)*bit + y*Width*bit] == 0 && x+dx<Width) dx++;
                while(arrowPixels[x*bit + (y+dy)*Width*bit] == 0 && y+dy<Height) dy++;
                
                for(int j=1;;j++){
                    rangeup = 0;
                    for(int i=0;i<dx;i++){
                        rangeup |= arrowPixels[(x+i)*bit + (y-j)*Width*bit] == 0;
                    }
                    if(rangeup && y-mdy>1)mdy++;
                    else break;
                }
                for(int j=1;;j++){
                    rangeup = 0;
                    for(int i=0;i<dx;i++){
                        rangeup |= arrowPixels[(x+i)*bit + (y+dy)*Width*bit] == 0;
                    }
                    if(rangeup && y+dy<Height-1)dy++;
                    else break;
                }
                for(int i=1;;i++){
                    rangeup = 0;
                    for(int j=0;j<dy+mdy;j++){
                        rangeup |= arrowPixels[(x+dx)*bit + (y-mdy+j)*Width*bit] == 0;
                    }
                    if(rangeup && x+dx<Width-1)dx++;
                    else break;
                }
                dots.resize(idx + 1);
                dots[idx].left = x-mdx;
                dots[idx].right = x+dx-1;
                dots[idx].top = MAX(y-mdy, 0);
                dots[idx].bottom = y+dy-1;
                idx++;
                for(int i=dots[idx-1].left;i<=dots[idx-1].right;i++) {
                    arrowPixels[i*bit + dots[idx-1].top*Width*bit+1] = 0;
                    arrowPixels[i*bit + dots[idx-1].bottom*Width*bit+1] = 0;
                }
                for(int i=dots[idx-1].top;i<=dots[idx-1].bottom;i++) {
                    arrowPixels[dots[idx-1].left*bit + i*Width*bit+1] = 0;
                    arrowPixels[dots[idx-1].right*bit + i*Width*bit+1] = 0;
                }
                
            }
        }
    }
    
    for(int i=0;i<idx;i++){
        symX = symY = 0;
        if(dots[i].right == Width-1 || dots[i].left == 0 || dots[i].bottom == Height-1 || dots[i].top == 0)continue;
        if(dots[i].right - dots[i].left > 30 || dots[i].bottom - dots[i].top > 30) continue;
        dotsX.resize(dots[i].right-dots[i].left+1);
        dotsY.resize(dots[i].bottom-dots[i].top+1);

        for(int y=dots[i].top;y<=dots[i].bottom;y++){
            dotsY[y-dots[i].top] = 0;
            for(int x=dots[i].left;x<=dots[i].right;x++){
                if(arrowPixels[x*bit+y*Width*bit] == 0){
                    dotsY[y-dots[i].top]++;
                }
            }
        }

        for(int x=dots[i].left;x<=dots[i].right;x++){
            dotsX[x-dots[i].left] = 0;
            for(int y=dots[i].top;y<=dots[i].bottom;y++){
                if(arrowPixels[x*bit+y*Width*bit] == 0){
                    dotsX[x-dots[i].left]++;
                }
            }
        }

        for(int x=0;x<=dots[i].right-dots[i].left;x++){
            symX += abs(dotsX[x] - dotsX[dots[i].right-dots[i].left-x]);
        }

        for(int y=0;y<=dots[i].bottom-dots[i].top;y++){
            symY += abs(dotsY[y] - dotsY[dots[i].bottom-dots[i].top-y]);
        }

        judgeL = dotsX[3] - dotsX[1];
        judgeR = dotsX[dots[i].right-dots[i].left-3] - dotsX[dots[i].right-dots[i].left-1];
        judgeT = dotsY[3] - dotsY[1];
        judgeB = dotsY[dots[i].bottom-dots[i].top-3] - dotsY[dots[i].bottom-dots[i].top-1];

        if(wi > 3) break;
        if(symY > symX){
            if(judgeT < judgeB) way[wi++] = 2;
            else way[wi++] = 8;
        }
        else{
            if(judgeL < judgeR) way[wi++] = 6;
            else way[wi++] = 4;
        }
    }
    // imshow("Display orig", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]));
}

void matchway(std::vector<uint8_t>& Pixels, int Width, int Height, int BitsPerPixel, int way[4]){
    std::vector<std::uint8_t> blurPixels;
    int bit = BitsPerPixel > 24 ? 4 : 3;
    bool want;

    blurPixels.resize(Width*Height*bit);
    GaussBlur(Pixels, blurPixels, Width, Height, BitsPerPixel, 1);
    for(int y=1;y<Height-1;y++){
        for(int x=1;x<Width-1;x++){
            want = (blurPixels[x*bit + y*Width*bit] < 100) && ((blurPixels[x*bit + y*Width*bit + 1] > 200) || (blurPixels[x*bit + y*Width*bit + 2] > 120)) && (blurPixels[x*bit + y*Width*bit + 1]+blurPixels[x*bit + y*Width*bit + 2] > 260);

            if(want){
                blurPixels[x*bit + y*Width*bit] = 0;
                blurPixels[x*bit + y*Width*bit + 1] = 0;
                blurPixels[x*bit + y*Width*bit + 2] = 0;
            }
            else {
                blurPixels[x*bit + y*Width*bit] = 255;
                blurPixels[x*bit + y*Width*bit + 1] = 255;
                blurPixels[x*bit + y*Width*bit + 2] = 255;
            }
        }
    }
    
    findarrow(blurPixels, Width, Height, BitsPerPixel, way);
}

void getpos(std::vector<uint8_t>& Pixels, int Width, int Height, int BitsPerPixel, int charpos[2], int ringpos[2], bool *isOther){
    std::vector<std::uint8_t> filterPixels, matchPixels;
    int bit = BitsPerPixel > 24 ? 4 : 3, pts[3], pointNum;
    struct rect range[3];
    std::vector<point> pos[3];
    bool isout, checkpoint;
    // type 0 char, 1 other, 2 ring
    pts[0] = pts[1] = pts[2] = 0;
    
    for (int type = 0; type < 3;type++){
        range[type].left = range[type].right = range[type].top = range[type].bottom = -1;
    }
    // cout << BitsPerPixel << "\n";
    matchPixels.resize(Width * Height * bit);
    GaussBlur(Pixels, filterPixels, Width, Height, BitsPerPixel, 1);
    // imshow("Display blur", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &filterPixels[0]));
    // waitKey(0);
    for(int y=0;y<Height;y++){
        for(int x=0;x<Width;x++){
            for(int type=0;type<3;type++){
                checkpoint = (type == 0 && (filterPixels[x*bit + y*Width*bit + 1] + filterPixels[x*bit + y*Width*bit + 2] > 230) && (abs(filterPixels[x*bit + y*Width*bit + 1] - filterPixels[x*bit + y*Width*bit + 2]) < 25) && (filterPixels[x*bit + y*Width*bit] < 90)) ||
                (type == 1 && (filterPixels[x*bit + y*Width*bit + 0] + filterPixels[x*bit + y*Width*bit + 1] < 200) && (abs(filterPixels[x*bit + y*Width*bit + 1] - filterPixels[x*bit + y*Width*bit + 0]) < 30) && (filterPixels[x*bit + y*Width*bit + 2] > 150)) ||
                (type == 2 && (filterPixels[x*bit + y*Width*bit + 0] + filterPixels[x*bit + y*Width*bit + 2] > 200) && (abs(filterPixels[x*bit + y*Width*bit + 2] - filterPixels[x*bit + y*Width*bit + 0]) < 30) && (filterPixels[x*bit + y*Width*bit + 1] < 100));

                if(checkpoint){
                    pos[type].resize(pts[type]+1);
                    pos[type][pts[type]].x = x;
                    pos[type][pts[type]].y = y;
                    pts[type]++;
                    matchPixels[x*bit + y*Width*bit + type] = filterPixels[x*bit + y*Width*bit + type];
                }
                else {
                    matchPixels[x*bit + y*Width*bit + type] = 0;
                }
            }
        }
    }
    
    // cout<<"pts:"<<pts[0]<<" "<<pts[1]<<" "<<pts[2]<<"\n";
    if(!(pts[0]+pts[1]+pts[2])) {
        cout << "@@@\n";
        charpos[0] = charpos[1] = -1;
        ringpos[0] = ringpos[1] = -1;
        (*isOther) = 0;
        return;
    }

    for(int type=0;type<3;type++){
        for(int idx=0;idx<pts[type];idx++){
            if(matchPixels[pos[type][idx].x*bit + pos[type][idx].y*Width*bit + type] != 0){
                pointNum = 0;
                for(int j=0;j<=10;j++){
                    for(int i=0;i<=10;i++){
                        if(matchPixels[(pos[type][idx].x+i-5)*bit + (pos[type][idx].y+j-5)*Width*bit+ type] != 0) {
                            pointNum += 10-abs(i-5)-abs(j-5);
                        }
                    }
                }
                isout = (pos[type][idx].x < 5) || (pos[type][idx].x > Width - 5) || (pos[type][idx].y < 5) || (pos[type][idx].y > Height - 5);
                isout = (type == 0 && (pointNum < 90/*180*/ || isout)) || (type == 1 && (pointNum < 80 || isout)) || (type == 2 && (pointNum < 160 || isout));
                if(isout){
                    for(int j=idx;j<pts[type]-1;j++){
                        pos[type][j].x = pos[type][j+1].x;
                        pos[type][j].y = pos[type][j+1].y;
                    }
                    pts[type]--;
                    idx--;
                }
            }
        }
    }

    for(int type=0;type<3;type++){
        if(!pts[type])continue;

        range[type].left = range[type].right = pos[type][0].x;
        range[type].top = range[type].bottom = pos[type][0].y;

        for(int i=0;i<pts[type];i++){
            if(pos[type][i].x > range[type].right) range[type].right = pos[type][i].x;
            else if(pos[type][i].x < range[type].left) range[type].left = pos[type][i].x;
            if(pos[type][i].y > range[type].bottom) range[type].bottom = pos[type][i].y;
            else if(pos[type][i].y < range[type].top) range[type].top = pos[type][i].y;
        }
        // cout << range[0].left << " " << range[0].right << " " << range[0].top << " " << range[0].bottom << "\n";
        charpos[0] = (range[0].left + range[0].right) / 2;
        charpos[1] = (range[0].top + range[0].bottom) / 2;
        
        ringpos[0] = (range[2].left + range[2].right) / 2;
        ringpos[1] = (range[2].top + range[2].bottom) / 2;

        (*isOther) = range[1].top == -1 ? 0 : 1;

        for(int i=range[type].left;i<=range[type].right;i++) {
            Pixels[i*bit + range[type].top*Width*bit+1] = 255;
            Pixels[i*bit + range[type].bottom*Width*bit+1] = 255;
        }
        for(int i=range[type].top;i<=range[type].bottom;i++) {
            Pixels[range[type].left*bit + i*Width*bit+1] = 255;
            Pixels[range[type].right*bit + i*Width*bit+1] = 255;
        }
    }
    // imshow("Display pos", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &matchPixels[0]));
    // imshow("Display selpos", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]));
    // waitKey(0);
}

void getNPC(std::vector<uint8_t>& Pixels, int Width, int Height, int BitsPerPixel, bool *isNPC){
    std::vector<std::uint8_t> filterPixels, matchPixels;
    int bit = BitsPerPixel > 24 ? 4 : 3, pts[3], pointNum;
    struct rect range[3];
    std::vector<point> pos[3];
    bool isout, checkpoint;
    // type 0 NPC, 1 -, 2 -
    pts[0] = pts[1] = pts[2] = 0;
    
    for (int type = 0; type < 3;type++){
        range[type].left = range[type].right = range[type].top = range[type].bottom = -1;
    }

    matchPixels.resize(Width * Height * bit);
    GaussBlur(Pixels, filterPixels, Width, Height, BitsPerPixel, 1);
    // imshow("Display blur", Mat(Height, Width, bit > 24 ? CV_8UC4 : CV_8UC3, &filterPixels[0]));
    // waitKey(0);
    for(int y=0;y<Height;y++){
        for(int x=0;x<Width;x++){
            for(int type=0;type<1;type++){
                checkpoint = (type == 0 && (filterPixels[x*bit + y*Width*bit + 0] + filterPixels[x*bit + y*Width*bit + 2] < 100) && (abs(filterPixels[x*bit + y*Width*bit + 0] - filterPixels[x*bit + y*Width*bit + 2]) < 25) && (filterPixels[x*bit + y*Width*bit + 1] > 200));

                if(checkpoint){
                    pos[type].resize(pts[type]+1);
                    pos[type][pts[type]].x = x;
                    pos[type][pts[type]].y = y;
                    pts[type]++;
                    matchPixels[x*bit + y*Width*bit + type] = filterPixels[x*bit + y*Width*bit + type];
                }
                else {
                    matchPixels[x*bit + y*Width*bit + type] = 0;
                }
            }
        }
    }
    
    // cout<<"pts:"<<pts[0]<<" "<<pts[1]<<" "<<pts[2]<<"\n";
    if(!(pts[0]+pts[1]+pts[2])) {
        cout << "@@@\n";
        (*isNPC) = 0;
        return;
    }

    for(int type=0;type<1;type++){
        for(int idx=0;idx<pts[type];idx++){
            if(matchPixels[pos[type][idx].x*bit + pos[type][idx].y*Width*bit + type] != 0){
                pointNum = 0;
                for(int j=0;j<=10;j++){
                    for(int i=0;i<=10;i++){
                        if(matchPixels[(pos[type][idx].x+i-5)*bit + (pos[type][idx].y+j-5)*Width*bit+ type] != 0) {
                            pointNum += 10-abs(i-5)-abs(j-5);
                        }
                    }
                }
                isout = (pos[type][idx].x < 5) || (pos[type][idx].x > Width - 5) || (pos[type][idx].y < 5) || (pos[type][idx].y > Height - 5);
                isout = (type == 0 && (pointNum < 60 || isout)) || (type == 1 && (pointNum < 80 || isout)) || (type == 2 && (pointNum < 160 || isout));
                if(isout){
                    for(int j=idx;j<pts[type]-1;j++){
                        pos[type][j].x = pos[type][j+1].x;
                        pos[type][j].y = pos[type][j+1].y;
                    }
                    pts[type]--;
                    idx--;
                }
            }
        }
    }

    for(int type=0;type<3;type++){
        if(!pts[type])continue;

        range[type].left = range[type].right = pos[type][0].x;
        range[type].top = range[type].bottom = pos[type][0].y;

        for(int i=0;i<pts[type];i++){
            if(pos[type][i].x > range[type].right) range[type].right = pos[type][i].x;
            else if(pos[type][i].x < range[type].left) range[type].left = pos[type][i].x;
            if(pos[type][i].y > range[type].bottom) range[type].bottom = pos[type][i].y;
            else if(pos[type][i].y < range[type].top) range[type].top = pos[type][i].y;
        }
        cout << range[0].left << " " << range[0].right << " " << range[0].top << " " << range[0].bottom << "\n";

        (*isNPC) = range[0].top == -1 ? 0 : 1;

        for(int i=range[type].left;i<=range[type].right;i++) {
            Pixels[i*bit + range[type].top*Width*bit+1] = 255;
            Pixels[i*bit + range[type].bottom*Width*bit+1] = 255;
        }
        for(int i=range[type].top;i<=range[type].bottom;i++) {
            Pixels[range[type].left*bit + i*Width*bit+1] = 255;
            Pixels[range[type].right*bit + i*Width*bit+1] = 255;
        }
    }
    // imshow("Display blur", Mat(Height, Width, bit > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]));
    // waitKey(0);
}