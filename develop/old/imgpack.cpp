#include <iostream>
#include <cmath> 
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>
#include <math.h>
#include <tiffio.h>
#include <string>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>
extern "C" {
#include <xdo.h>
}
using namespace cv;
using namespace std;

struct POINT{
    int x;
    int y;
};

struct RECT{
    int left;
    int right;
    int top;
    int bottom;
};

struct COLOR{
    int R;
    int G;
    int B;
};

bool dirExists(const std::string &path) {
    struct stat info;
    if (stat(path.c_str(), &info) == 0 && info.st_mode & S_IFDIR) {
        return true;
    }
    return false;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void ScreenShot(std::vector<uint8_t>& Pixels, int& Width, int& Height, int& BitsPerPixel)
{
    // Display* display = XOpenDisplay(nullptr);
    // Window root = DefaultRootWindow(display);
    Display* display = XOpenDisplay(nullptr);
    Window root = ScreenOfDisplay(display, 0)->root;
    XWindowAttributes attributes = {0};
    // XGetWindowAttributes(display, root, &attributes);
    string id = exec("xdotool search --name \"Google Chrome\"");
    cout<<id<<"\n";
    int x,y;
    XGetWindowAttributes(display, atoi(id.data()), &attributes); //
    // FileZilla 
    // Google Chrome 14680087
    Width = attributes.width;
    Height = attributes.height;
    x = attributes.x;
    y = attributes.y;
    cout<<x<<" "<<y<<"\n";
    XImage* img = XGetImage(display, root, x, y , Width, Height, AllPlanes, ZPixmap);
    BitsPerPixel = img->bits_per_pixel;
    Pixels.resize(Width * Height * BitsPerPixel/8);

    memcpy(&Pixels[0], img->data, Pixels.size());

    XDestroyImage(img);
    XCloseDisplay(display);
}

void CutandPaste(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& pastePixels, int Width, int Height, int BitsPerPixel, RECT rangeCut, int pasteX, int pasteY){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    pastePixels.resize(Width * Height * bit);
    for(int y=0; y<=rangeCut.bottom-rangeCut.top; y++){
        for(int x=0; x<=rangeCut.right-rangeCut.left; x++){
            for(int k=0; k<bit; k++){
                pastePixels[(pasteX + x) * bit + (pasteY + y) * Width * bit + k] = Pixels[(rangeCut.left + x) * bit + (rangeCut.top + y) * Width * bit + k];
            }
        }
    }
}

void cutImg(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& cutPixels, int Width, int Height, int BitsPerPixel, RECT range){
    int bit = BitsPerPixel > 24 ? 4 : 3, idx=0;
    cutPixels.resize((range.right - range.left + 1) * (range.bottom - range.top + 1) * bit);
    for(int j=range.top; j<=range.bottom; j++){
        for(int i=range.left; i<=range.right; i++){
            for(int k=0;k<3;k++){
                cutPixels[idx++] = Pixels[i*bit + j*Width*bit + k];
            }
            if(bit == 4)cutPixels[idx++] = 0;
        }
    }
}
void twoValue(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& editPixels, int Width, int Height, int BitsPerPixel, int gate){
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
                if(tmp > gate) editPixels[i*bit + j*Width*bit + k] = 255;
                else editPixels[i*bit + j*Width*bit + k] = 0;
            }
        }
    }
}

void drawBorder(std::vector<uint8_t>& Pixels, int Width, int Height, int BitsPerPixel, RECT range){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    range.left = max(range.left, 1);
    range.right = min(range.right, Width - 2);
    range.top = max(range.top, 1);
    range.bottom = min(range.bottom, Height - 2);
    for(int x=range.left; x<=range.right; x++){
        Pixels[x*bit + (range.top-1)*Width*bit + 1] = 0;
        Pixels[x*bit + (range.bottom+1)*Width*bit + 1] = 0;
    }
    for(int y=range.top; y<=range.bottom; y++){
        Pixels[(range.left-1)*bit + y*Width*bit + 1] = 0;
        Pixels[(range.right+1)*bit + y*Width*bit + 1] = 0;
    }
}

// @@@@
void stackFind(std::vector<uint8_t>& Pixels, int Width, int Height, int BitsPerPixel, int x, int y, RECT *range, float distance, int maxWidth, int maxHeight){
    int bit = BitsPerPixel > 24 ? 4 : 3, way[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    Pixels[x*bit + y*Width*bit] = 255;
    for(int dy=-ceil(distance); dy<=ceil(distance); dy++){
        for(int dx=-ceil(distance); dx<=ceil(distance); dx++){
            if(x + dx < 0 || x + dx >= Width || y + dy < 0 || y + dy >= Height) continue;
            if(sqrt(dx*dx + dy*dy) > distance) continue;
            if(range->right - (x + dx) > maxWidth || x + dx - range->left > maxWidth || range->bottom - (y + dy) > maxHeight || y + dy - range->top > maxHeight ) continue;
            if(Pixels[(x + dx)*bit + (y + dy)*Width*bit] == 0){
                if(x + dx < range->left) range->left = x + dx;
                else if(x + dx > range->right) range->right = x + dx;
                if(y + dy < range->top) range->top = y + dy;
                else if(y + dy > range->bottom) range->bottom = y + dy;
                stackFind(Pixels, Width, Height, BitsPerPixel, x + dx, y + dy, range, distance, maxWidth, maxHeight);
            }
        }
    }
    // for(int i=0;i<4;i++){
    //     if(Pixels[(x + way[i][0])*bit + (y + way[i][1])*Width*bit] == 0){
    //         if(x + way[i][0] < range->left) range->left = x + way[i][0];
    //         else if(x + way[i][0] > range->right) range->right = x + way[i][0];
    //         if(y + way[i][1] < range->top) range->top = y + way[i][1];
    //         else if(y + way[i][1] > range->bottom) range->bottom = y + way[i][1];
    //         stackFind(Pixels, Width, Height, BitsPerPixel, x + way[i][0], y + way[i][1], range, maxWidth, maxHeight);
    //     }
    // }
}
void findBlock(std::vector<uint8_t>& Pixels, int Width, int Height, int BitsPerPixel, int maxWidth, int maxHeight, std::vector<RECT>& blocks, float distance, bool save){
    int bit = BitsPerPixel > 24 ? 4 : 3, blockL = 0;
    int dx, dy;
    RECT tmpRange;
    std::vector<std::uint8_t> findPixels;
    // std::vector<RECT> blocks;

    findPixels.resize(Width * Height * bit);
    for(int i=0; i<Width * Height * bit; i++) findPixels[i] = Pixels[i];

    for(int y=0; y<Height; y++){
        for(int x=0; x<Width; x++){
            if((int)findPixels[x*bit + y*Width*bit] == 0){
                blocks.resize(++blockL);
                blocks[blockL-1].left = blocks[blockL-1].right = x;
                blocks[blockL-1].top = blocks[blockL-1].bottom = y;
                stackFind(findPixels, Width, Height, BitsPerPixel, x, y, &blocks[blockL-1], distance, maxWidth, maxHeight);
            }
        }
    }

    for(int i=0; i<blockL; i++){
        dx = blocks[i].right - blocks[i].left;
        dy = blocks[i].bottom - blocks[i].top;
        if((dx <= 10 || dx > maxWidth) && (dy <= 10 || dy > maxHeight)){
            for(int k=i+1; k<blockL; k++){
                blocks[k-1].left = blocks[k].left;
                blocks[k-1].right = blocks[k].right;
                blocks[k-1].top = blocks[k].top;
                blocks[k-1].bottom = blocks[k].bottom;
            }
            blockL--;
            i--;
            continue;
        }

        for(int j=i+1; j<blockL; j++){
            tmpRange.left = min(blocks[i].left, blocks[j].left);
            tmpRange.right = max(blocks[i].right, blocks[j].right);
            tmpRange.top = min(blocks[i].top, blocks[j].top);
            tmpRange.bottom = max(blocks[i].bottom, blocks[j].bottom);
            dx = tmpRange.right - tmpRange.left;
            dy = tmpRange.bottom - tmpRange.top;
            if(dx <= maxWidth && dy <= maxHeight){
                blocks[i].left = tmpRange.left;
                blocks[i].right = tmpRange.right;
                blocks[i].top = tmpRange.top;
                blocks[i].bottom = tmpRange.bottom;
                for(int k=j+1; k<blockL; k++){
                    blocks[k-1].left = blocks[k].left;
                    blocks[k-1].right = blocks[k].right;
                    blocks[k-1].top = blocks[k].top;
                    blocks[k-1].bottom = blocks[k].bottom;
                }
                blockL--;
                j--;
            }
        }
    }

    blocks.resize(blockL);

    if(save){
        std::vector<std::uint8_t> cutPixels;
        for(int i=0; i<blockL; i++){
            cutImg(Pixels, cutPixels, Width, Height, BitsPerPixel,  blocks[i]);
            imwrite((string)"ocr_numbers/" + to_string(i) + (string)".jpg", Mat(blocks[i].bottom - blocks[i].top + 1, blocks[i].right - blocks[i].left + 1, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &cutPixels[0]));
            drawBorder(Pixels, Width, Height, BitsPerPixel, blocks[i]);
        }
        imshow("drawBorder", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]));
        waitKey(0);
    }
    else{
        for(int i=0; i<blockL; i++){
            drawBorder(Pixels, Width, Height, BitsPerPixel, blocks[i]);
            // imshow("drawBorder", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]));
            // waitKey(0);
        }
    }
    
}

void twist(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& twistPixels, int Width, int Height, int BitsPerPixel, int posx, int posy, float twist_rate){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    float newx, newy, dx, dy, newv;
    float r2, sine, cosine, theta, rmax = MIN(MIN(posx, posy),MIN(Width - posx, Height - posy));
    twistPixels.resize(Width * Height * bit);
    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            r2 = (i-posx)*(i-posx) + (j-posy)*(j-posy);
            theta = twist_rate * pow((1 - MIN(sqrt(r2), rmax) / rmax),2) * M_PI / 180.0;
            sine = sin(theta);
            cosine = cos(theta);
            newx = MAX( MIN(cosine * (i-posx) - sine * (j-posy)+posx, Width-1), 0);
            newy = MAX( MIN(sine * (i-posx) + cosine * (j-posy)+posy, Height-1), 0);
            dx = newx - floor(newx);
            dy = newy - floor(newy);
            newx -= dx;
            newy -= dy;
            for(int k=0;k<3;k++){
                newv = (1.0-dx) * (1.0-dy) * (float)Pixels[newx * bit + newy * Width * bit + k];
                newv += (1.0-dx) * dy * (float)Pixels[newx * bit + (newy+1) * Width * bit + k];
                newv += dx * (1.0-dy) * (float)Pixels[(newx+1) * bit + newy * Width * bit + k];
                newv += dx * dy * (float)Pixels[(newx+1) * bit + (newy+1) * Width * bit + k];
                twistPixels[i * bit + j * Width * bit + k] = newv;
            }
        }
    }
}

void blackhole(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& blackholePixels, int Width, int Height, int BitsPerPixel, int posx, int posy, float holesize, float gravity, float factor){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    float r, newx, newy, dx, dy, newv, rmax = MIN(MIN(posx, posy),MIN(Width - posx, Height - posy));
    bool isIn;
    blackholePixels.resize(Width * Height * bit);
    posx = MAX(MIN(posx, Width), 0);
    posy = MAX(MIN(posy, Height), 0);

    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            r = sqrt((i-posx)*(i-posx) + (j-posy)*(j-posy));
            newx = (i-posx) * pow( 1 + 64*gravity/(pow(r+15,1.9)+10), factor ) + posx;
            newy = (j-posy) * pow( 1 + 64*gravity/(pow(r+15,1.9)+10), factor ) + posy;
            dx = newx - floor(newx);
            dy = newy - floor(newy);
            newx -= dx;
            newy -= dy;
            isIn = (newx > 0) && (newx < Width) && (newy > 0) && (newy < Height);
            for(int k=0;k<3;k++){
                if(r <= holesize) blackholePixels[i*bit + j*Width*bit + k] = 0;
                else {
                    if(isIn){
                        newv = (1.0-dx) * (1.0-dy) * (float)Pixels[newx * bit + newy * Width * bit + k];
                        newv += (1.0-dx) * dy * (float)Pixels[newx * bit + (newy+1) * Width * bit + k];
                        newv += dx * (1.0-dy) * (float)Pixels[(newx+1) * bit + newy * Width * bit + k];
                        newv += dx * dy * (float)Pixels[(newx+1) * bit + (newy+1) * Width * bit + k];
                        blackholePixels[i*bit + j*Width*bit + k] = newv;
                    }
                    else {
                        blackholePixels[i*bit + j*Width*bit + k] = 255;
                    }
                }
            }
        }
    }
}

void negative(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& negativePixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    negativePixels.resize(Width * Height * bit);
    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            for(int k=0;k<3;k++){
                negativePixels[i*bit + j*Width*bit + k] = 255 - Pixels[i*bit + j*Width*bit + k];
            }
        }
    }
}

void RGBtoYUV(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& YUBPixels, int Width, int Height, int BitsPerPixel, char type){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    float tmp;
    YUBPixels.resize(Width * Height * bit);
    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            if(type == 'Y')tmp = (float)Pixels[i*bit + j*Width*bit]*0.299 + (float)Pixels[i*bit + j*Width*bit + 1]*0.587 + (float)Pixels[i*bit + j*Width*bit + 2]*0.114;
            else if(type == 'U')tmp = (float)Pixels[i*bit + j*Width*bit]*(-0.169) + (float)Pixels[i*bit + j*Width*bit + 1]*(-0.331) + (float)Pixels[i*bit + j*Width*bit + 2]*0.5 + 128;
            else if(type == 'V')tmp = (float)Pixels[i*bit + j*Width*bit]*0.5 + (float)Pixels[i*bit + j*Width*bit + 1]*(-0.419) + (float)Pixels[i*bit + j*Width*bit + 2]*(-0.081) + 128;
            else tmp = (float)Pixels[i*bit + j*Width*bit]/3.0 + (float)Pixels[i*bit + j*Width*bit + 1]/3.0 + (float)Pixels[i*bit + j*Width*bit + 2]/3.0;
            for(int k=0;k<3;k++){
                YUBPixels[i*bit + j*Width*bit + k] = tmp;
            }
        }
    }
}

void GammaTransform(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& gammaPixels, int Width, int Height, int BitsPerPixel, float gamma){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    gammaPixels.resize(Width * Height * bit);
    for(int k=0;k<4;k++){
        for(int j=0;j<Height;j++){
            for(int i=0;i<Width;i++){
                gammaPixels[i*bit + j*Width*bit + k] = MAX( MIN( round( pow( Pixels[i*bit + j*Width*bit + k] / 255.0, gamma) * 255.0), 255), 0);
            }
        }
    }
}

void jpeg_compress(std::vector<uint8_t>& Pixels, std::vector<char>& jpgPixels, int Width, int Height, int BitsPerPixel, float Q){
    int W_8 = Width/8 + 1, H_8 = Height/8 + 1;
    int* encode; 
    float Qm[8][8] = {{16,11,10,16,24,40,51,61},{12,12,14,19,26,58,60,55},{14,13,16,24,40,57,69,56},{14,17,22,29,51,87,80,62},{18,22,37,56,68,109,103,77},{24,35,55,64,81,104,113,92},{49,64,78,87,103,121,120,101},{72,92,95,98,112,100,103,99}};
    int bit = BitsPerPixel > 24 ? 4 : 3;
    float S = Q < 50 ? 5000.0/Q : 200.0 - 2.0*Q, g[8][8], G[8][8], au, av;
    float omega[64];
    jpgPixels.resize(Width * Height * bit);
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            Qm[i][j] = floor((S*Qm[i][j]+50)/100);
            if(Qm[i][j] == 0)Qm[i][j] = 1;
        }
    }

    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            omega[8*i + j] = cos((2*i+1)*j*M_PI/16.0);
        }
    }

    for(int k=0;k<3;k++){
        for(int j=0;j<H_8;j++){
            for(int i=0;i<W_8;i++){
                for(int y=0;y<8;y++){
                    for(int x=0;x<8;x++){
                        if(8*i+x >= Width || 8*j+y >= Height) g[x][y] = 0;
                        else g[x][y] = (int)Pixels[(8*i+x)*bit + (8*j+y)*Width*bit + k] - 128;
                    }
                }

                for(int v=0;v<8;v++){
                    for(int u=0;u<8;u++){
                        G[u][v] = 0;
                        av = v == 0 ? M_SQRT1_2 : 1;
                        for(int y=0;y<8;y++){
                            G[u][v] += av * g[u][y] * omega[8*y + v] / 2.0;
                        }
                    }
                }

                for(int v=0;v<8;v++){
                    for(int u=0;u<8;u++){
                        g[u][v] = G[u][v];
                        G[u][v] = 0;
                    }
                }

                for(int v=0;v<8;v++){
                    for(int u=0;u<8;u++){
                        G[u][v] = 0;
                        au = u == 0 ? M_SQRT1_2 : 1;
                        for(int x=0;x<8;x++){
                            G[u][v] += au * g[x][v] * omega[8*x + u] / 2.0;
                        }
                    }
                }

                for(int v=0;v<8;v++){
                    for(int u=0;u<8;u++){
                        G[u][v] = round(G[u][v] / Qm[v][u]);
                        if(8*i+u < Width && 8*j+v < Height) jpgPixels[(8*i+u)*bit + (8*j+v)*Width*bit + k] = (char)G[u][v];
                    }
                }
            }
        }
    }
    
    //return encode;
}

void jpeg_decompress(std::vector<char>& jpgPixels, std::vector<uint8_t>& Pixels, int Width, int Height, int BitsPerPixel, float Q){
    int W_8 = Width/8 + 1, H_8 = Height/8 + 1;
    int* encode; 
    float Qm[8][8] = {{16,11,10,16,24,40,51,61},{12,12,14,19,26,58,60,55},{14,13,16,24,40,57,69,56},{14,17,22,29,51,87,80,62},{18,22,37,56,68,109,103,77},{24,35,55,64,81,104,113,92},{49,64,78,87,103,121,120,101},{72,92,95,98,112,100,103,99}};
    int bit = BitsPerPixel > 24 ? 4 : 3;
    Q = Q < 1 ? 1 : Q;
    float S = Q < 50 ? 5000.0/Q : 200.0 - 2.0*Q, g[8][8], G[8][8], au, av;
    float omega[64];
    Pixels.resize(Width * Height * bit);
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            Qm[i][j] = floor((S*Qm[i][j]+50)/100);
            if(Qm[i][j] == 0)Qm[i][j] = 1;
        }
    }

    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            omega[8*i + j] = cos((2*i+1)*j*M_PI/16.0);
        }
    }

    for(int k=0;k<3;k++){
        for(int j=0;j<H_8;j++){
            for(int i=0;i<W_8;i++){
                for(int v=0;v<8;v++){
                    for(int u=0;u<8;u++){
                        if(8*i+u >= Width || 8*j+v >= Height) G[u][v] = 0;
                        else G[u][v] = jpgPixels[(8*i+u)*bit + (8*j+v)*Width*bit + k] * Qm[v][u];
                    }
                }

                for(int y=0;y<8;y++){
                    for(int x=0;x<8;x++){
                        g[x][y] = 0;
                        for(int v=0;v<8;v++){
                            av = v == 0 ? M_SQRT1_2 : 1;
                            g[x][y] += av * G[x][v] * omega[8*y + v] / 2.0;
                        }
                    }
                }

                for(int y=0;y<8;y++){
                    for(int x=0;x<8;x++){
                        G[x][y] = g[x][y];
                        g[x][y] = 0;
                    }
                }

                for(int y=0;y<8;y++){
                    for(int x=0;x<8;x++){
                        g[x][y] = 0;
                        for(int u=0;u<8;u++){
                            au = u == 0 ? M_SQRT1_2 : 1;
                            g[x][y] += au  * G[u][y] * omega[8*x + u] / 2.0;
                        }
                    }
                }

                for(int y=0;y<8;y++){
                    for(int x=0;x<8;x++){
                        if(8*i+x < Width && 8*j+y < Height) Pixels[(8*i+x)*bit + (8*j+y)*Width*bit + k] = MAX(MIN(g[x][y] + 128, 255), 0);
                    }
                }
            }
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

void showedge(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& egdePixels, int Width, int Height, int BitsPerPixel, int critical){
    int bit = BitsPerPixel > 24 ? 4 : 3, Gx[3], Gy[3];
    int Gmx[3][3] = {{1,0,-1},{2,0,-2},{1,0,-1}}, Gmy[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
    float L[3] , theta[3];
    bool Overcritical;
    std::vector<uint8_t> blurPixels;

    if(!(Width * Height))return;

    GaussBlur(Pixels, blurPixels, Width, Height, BitsPerPixel, 1.2);
    egdePixels.resize(Width * Height * bit);
    for(int y=1;y<Height-1;y++){
        for(int x=1;x<Width-1;x++){
            for(int k=0;k<3;k++)Gx[k] = Gy[k] = 0;
            Overcritical = 0;
            for(int j=0;j<3;j++){
                for(int i=0;i<3;i++){
                    for(int k=0;k<3;k++){
                        Gx[k] += blurPixels[(i+x-1)*bit + (j+y-1)*Width*bit + k] * Gmx[j][i];
                        Gy[k] += blurPixels[(i+x-1)*bit + (j+y-1)*Width*bit + k] * Gmy[j][i];
                    }
                }
            }
            for(int k=0;k<3;k++){
                L[k] = sqrt(Gx[k]*Gx[k] + Gy[k]*Gy[k]);
                theta[k] = atan2(Gy[k], Gx[k]);
                Overcritical |= L[k] > critical;
            }

            for(int k=0;k<3;k++) egdePixels[x*bit + y*Width*bit + k] = Overcritical ? 255 : 0;
        }
    }
}

void sharp(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& sharpPixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    std::vector<uint8_t> blurPixels;
    sharpPixels.resize(Width * Height * bit);
    GaussBlur(Pixels, blurPixels, Width, Height, BitsPerPixel, 2);

    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            for(int k=0;k<3;k++){
                sharpPixels[i*bit + j*Width*bit + k] = MIN(MAX(2*Pixels[i*bit + j*Width*bit + k] - blurPixels[i*bit + j*Width*bit + k], 0), 255);
            }
        }
    }
    imshow("Display sharp", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &sharpPixels[0]));
}

void imgFloor(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& sixteenPixels, int Width, int Height, int BitsPerPixel, int flr){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    sixteenPixels.resize(Width * Height * bit);
    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            for(int k=0;k<3;k++){
                sixteenPixels[i*bit + j*Width*bit + k] = Pixels[i*bit + j*Width*bit + k] - Pixels[i*bit + j*Width*bit + k] % flr + flr - 1;
                // sixteenPixels[i*bit + j*Width*bit + k] = 256 - ((256 - Pixels[i*bit + j*Width*bit + k]) % flr);
                // sixteenPixels[i*bit + j*Width*bit + k] = Pixels[i*bit + j*Width*bit + k] - Pixels[i*bit + j*Width*bit + k] % flr;
            }
        }
    }
}

// @@@@
void filter(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& filterPixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    std::vector<uint8_t> blurPixels;
    filterPixels.resize(Width * Height * bit);

    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            if(Pixels[i*bit + j*Width*bit + 0] < 100 && abs(Pixels[i*bit + j*Width*bit + 1] - Pixels[i*bit + j*Width*bit + 2] + 50) < 70 && Pixels[i*bit + j*Width*bit + 1] + Pixels[i*bit + j*Width*bit + 2] > 270){
                for(int k=0;k<3;k++){
                    filterPixels[i*bit + j*Width*bit + k] = 255;
                }
            }
            else{
                for(int k=0;k<3;k++){
                    filterPixels[i*bit + j*Width*bit + k] = 0;
                }
            }
        }
    }
}

void special(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& specialPixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    specialPixels.resize(Width * Height * bit);
    // wid 102 hei 177
    int range[5][4] = { // left right top bottom
        {27,65,0,25},
        {10,90,38,58},
        {20,80,78,98},
        {0,100,118,138},
        {20,80,157,177},
    }, dots[5] = {0,0,0,0,0}, outside = 0;
    float rate[5], outsiderate;
    bool isin;

    for(int j=0;j<Height;j++){
        for(int i=0;i<Width;i++){
            isin = 0;
            for(int p=0;p<5;p++){
                if(i > range[p][0] && i < range[p][1] && j > range[p][2] && j < range[p][3]){
                    if(Pixels[i*bit + j*Width*bit] == 255) dots[p]++;
                    for(int k=0;k<3;k++){
                        specialPixels[i*bit + j*Width*bit + k] = Pixels[i*bit + j*Width*bit + k];
                    }
                    isin = 1;
                    break;
                }
            }
            if(!isin){
                if(Pixels[i*bit + j*Width*bit] == 255) outside++;
                for(int k=0;k<2;k++){
                    specialPixels[i*bit + j*Width*bit + k] = 255;
                }
            }
        }
    }
    cout<<"outside:"<<outside<<" "<<outside / 11104.0<<"\n";
    for(int p=0;p<5;p++) {
        cout<<"dots"<<p<<": "<<dots[p]<<" ";
        rate[p] = dots[p] / (float)((range[p][1]-range[p][0])*(range[p][3]-range[p][2]));
        cout<<rate[p]<<"\n";
    }
}

void matrixTrans(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& transPixels, int Width, int Height, int BitsPerPixel, RECT range, float mtx[2][2]){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    float det = mtx[0][0] * mtx[1][1] - mtx[0][1] * mtx[1][0], mtxRev[2][2] = {{mtx[1][1]/det, -mtx[0][1]/det}, {-mtx[1][0]/det, mtx[0][0]/det}};
    float centerX = (range.left + range.right)/2, centerY = (range.top + range.bottom)/2, tmpx, tmpy, floatx, floaty;
    transPixels.resize(Width * Height * bit);
    // for(int i=0; i<Width * Height * bit; i++) transPixels[i] = 255;
    
    // for(int y=range.top; y<=range.bottom; y++){
    //     for(int x=range.left; x<=range.right; x++){
    for(int y=0; y<=Height; y++){
        for(int x=0; x<=Width; x++){
            for(int k=0; k<bit; k++){
                // tmpx = (float)(x-centerX)*mtx[0][0] + (float)(y-centerY)*mtx[0][1] + centerX;
                // tmpy = (float)(x-centerX)*mtx[1][0] + (float)(y-centerY)*mtx[1][1] + centerY;
                // if(range.left <= x && x < range.right && range.top <= y && y < range.bottom){
                //     transPixels[tmpx*bit + tmpy*Width*bit + k] = Pixels[x*bit + y*Width*bit + k];
                //     Pixels[x*bit + y*Width*bit + k] = 255;
                // }
                // else{
                //     transPixels[x*bit + y*Width*bit + k] = Pixels[x*bit + y*Width*bit + k];
                // }

                tmpx = ((float)x-centerX)*mtxRev[0][0] + ((float)y-centerY)*mtxRev[0][1] + centerX;
                tmpy = ((float)x-centerX)*mtxRev[1][0] + ((float)y-centerY)*mtxRev[1][1] + centerY;
                if(range.left <= tmpx && tmpx <= range.right && range.top <= tmpy && tmpy <= range.bottom){
                    floatx = tmpx - floor(tmpx);
                    tmpx = floor(tmpx);
                    floaty = tmpy - floor(tmpy);
                    tmpy = floor(tmpy);
                    transPixels[x*bit + y*Width*bit + k] = 
                    (1-floatx) * (1-floaty) * Pixels[tmpx*bit + tmpy*Width*bit + k] +
                    floatx * (1-floaty) * Pixels[(tmpx+1)*bit + tmpy*Width*bit + k] +
                    (1-floatx) * floaty * Pixels[tmpx*bit + (tmpy+1)*Width*bit + k] +
                    floatx * floaty * Pixels[(tmpx+1)*bit + (tmpy+1)*Width*bit + k];
                }
                // else{
                //     transPixels[x*bit + y*Width*bit + k] = Pixels[x*bit + y*Width*bit + k];
                // }
            }
        }
    }
    
}

void noisePoint(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& noicePixels, int Width, int Height, int BitsPerPixel, int dotAmount, int lineAmount){
    int bit = BitsPerPixel > 24 ? 4 : 3, rx ,ry, px, py, tmpy, div = 20, lineY, delay;
    float frequency, waveH;
    RECT range;
    std::vector<uint8_t> tmpPixels;
    noicePixels.resize(Width * Height * bit);
    tmpPixels.resize(Width * Height * bit);
    for(int i=0;i<Width * Height * bit; i++) tmpPixels[i] = 255;
    for(int n=0; n<lineAmount; n++){
        lineY = Height/2 + rand()%20 - 10;
        delay = rand()%30 - 15;
        frequency = 0.9 + rand()%200 / 1000;
        waveH = 10 + rand()%100 / 5;
        for(float x=0; x<Width; x+=1){
            tmpy = (waveH * sin(x * M_PI / 180 * frequency + delay) + lineY);
            if(tmpy>0 && tmpy <Height) for(int k=0; k<bit; k++) tmpPixels[(int)x * bit + tmpy * Width * bit + k] = 0;
        }
    }
    GaussBlur(tmpPixels, noicePixels, Width, Height, BitsPerPixel, 0.4);
     for(int i=0;i<Width * Height * bit; i++) tmpPixels[i] = noicePixels[i];

    for(int n=0; n<dotAmount; n++){
        do{
            rx = pow(rand()%10, 3) / 300;
            ry = pow(rand()%10, 3) / 300;
            px = rand()%Width;
            py = rand()%Height;
        }
        while(px + rx >= Width || py + ry >= Height);

        for(int d=0; d<div; d++){
            for(int k=0; k<bit; k++) tmpPixels[(int)(px + rx*d/div) * bit + (int)(py + ry*d/div) * Width * bit + k] = 0;
        }
    }
    GaussBlur(tmpPixels, noicePixels, Width, Height, BitsPerPixel, 0.6);
    for(int i=0;i<Width * Height * bit; i++) if(Pixels[i] < 200) noicePixels[i] = Pixels[i];

}

void matrixMul(float mtxA[2][2], float mtxB[2][2], float mtx[2][2]){
    float rmtx[2][2] = {{0, 0}, {0, 0}};
    rmtx[0][0] = mtxA[0][0] * mtxB[0][0] + mtxA[0][1] * mtxB[1][0];
    rmtx[0][1] = mtxA[0][0] * mtxB[0][1] + mtxA[0][1] * mtxB[1][1];
    rmtx[1][0] = mtxA[1][0] * mtxB[0][0] + mtxA[1][1] * mtxB[1][0];
    rmtx[1][1] = mtxA[1][0] * mtxB[0][1] + mtxA[1][1] * mtxB[1][1];
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            mtx[i][j] = rmtx[i][j];
        }
    }
}

void verification(int num, int Width, int Height, int specNum){
    // num = 6 W = 251, H = 51;
    int filesNum = 32, bit = 4, findex = 0, BitsPerPixel = 24, dx, dy;
    float theta;
    DIR *dir;
    Mat imgtest;
    struct dirent *ent;
    string route = "ocr_numbers/", fullName = "", tmpFile;
    std::vector<string> files;
    std::vector<std::uint8_t> Pixels, finalPixels, tmpPixels;
    std::vector<RECT> numPos, numPosTrans;
    // {{1.2, 0}, {0, 1.3}} {{cos(0.5), -sin(0.5)}, {sin(0.5), cos(0.5)}}
    float mtx[2][2], shrink[2][2] = {{0.8, 0}, {0, 0.9}}, rotate[2][2] = {{cos(0.5), -sin(0.5)}, {sin(0.5), cos(0.5)}}, tangent[2][2] = {{1, 0}, {0, 1}};
    
    // srand((unsigned int)time(NULL));
    files.resize(filesNum);
    Pixels.resize(Width * Height * bit);
    finalPixels.resize(Width * Height * bit);
    numPos.resize(num);
    numPosTrans.resize(num);

    for(int i=0; i<Width * Height * bit; i++){
        Pixels[i] = finalPixels[i] = 255;
    }

    if ((dir = opendir (route.data())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            imgtest = imread(route + (ent->d_name));
            if(imgtest.cols && imgtest.rows) files[findex++] = ent->d_name;
        }
        closedir (dir);
    } else {
        return;
    }

    for(int i=0; i<num; i++){
        if(specNum == -1){
            tmpFile = files[rand()%filesNum];
        }
        else{
            tmpFile = files[specNum];
        }
        fullName += tmpFile[0];
        imgtest = imread(route + tmpFile);
        tmpPixels.resize(imgtest.cols * imgtest.rows * imgtest.channels());
        tmpPixels.assign(imgtest.data, imgtest.data + imgtest.total()*imgtest.channels());
        imshow("imgtest", Mat(imgtest.rows, imgtest.cols, imgtest.channels() * 8 > 24 ? CV_8UC4 : CV_8UC3, &tmpPixels[0]));
        
        // cout << fileName << "\n";

        numPos[i].left = 21+42*i - imgtest.cols/2;
        numPos[i].right = numPos[i].left + imgtest.cols;
        numPos[i].top = (Height-imgtest.rows)/2;
        numPos[i].bottom = numPos[i].top + imgtest.rows;

        for(int x=0; x<imgtest.cols; x++){
            for(int y=0; y<imgtest.rows; y++){
                for(int k=0; k<imgtest.channels(); k++){
                    Pixels[(numPos[i].left + x) * imgtest.channels() + (numPos[i].top + y) * Width * imgtest.channels() + k] = tmpPixels[x * imgtest.channels() + y * imgtest.cols * imgtest.channels() + k];
                }
            }
        }
    }
    
    imshow("orig", Mat(Height, Width, imgtest.channels() * 8 > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]));

    for(int i=0; i<num; i++) {
        shrink[0][0] = 0.9 - ((float)(random()%25)) / 100.0;
        shrink[1][1] = 0.9 - ((float)(random()%25)) / 100.0;
        theta = ((float)(rand()%80) - 40.0) * M_PI / 180.0;
        rotate[0][0] = cos(theta);
        rotate[0][1] = -sin(theta);
        rotate[1][0] = sin(theta);
        rotate[1][1] = cos(theta);

        matrixMul(rotate, shrink, mtx);
        matrixTrans(Pixels, finalPixels, Width, Height, BitsPerPixel, numPos[i], mtx);
    }
    for(int i=0; i<Width * Height * bit; i++){
        Pixels[i] = finalPixels[i];
        finalPixels[i] = 255;
    }

    for(int i=0; i<num; i++) {
        twist(Pixels, finalPixels, Width, Height, BitsPerPixel, (numPos[i].left + numPos[i].right)/2 + rand()%5 - 10, (numPos[i].top + numPos[i].bottom)/2 + rand()%5 - 10, rand()%100 - 50);
        for(int i=0; i<Width * Height * bit; i++) Pixels[i] = finalPixels[i];
    }

    imshow("transDone", Mat(Height, Width, imgtest.channels() * 8 > 24 ? CV_8UC4 : CV_8UC3, &finalPixels[0]));
    twoValue(finalPixels, finalPixels, Width, Height, imgtest.channels() * 8, 200);
    findBlock(finalPixels, Width, Height, imgtest.channels() * 8, 35, 35, numPos, 1, 0);
    imshow("findBorder", Mat(Height, Width, imgtest.channels() * 8 > 24 ? CV_8UC4 : CV_8UC3, &finalPixels[0]));

    for(int i=0; i<Width * Height * bit; i++){
        finalPixels[i] = 255;
    }

    for(int i=0;i<num;i++){
        for(int j=i+1;j<num;j++){
            if(numPos[i].left > numPos[j].left){
                numPos[i].left ^= numPos[j].left;
                numPos[j].left ^= numPos[i].left;
                numPos[i].left ^= numPos[j].left;
                numPos[i].right ^= numPos[j].right;
                numPos[j].right ^= numPos[i].right;
                numPos[i].right ^= numPos[j].right;
                numPos[i].top ^= numPos[j].top;
                numPos[j].top ^= numPos[i].top;
                numPos[i].top ^= numPos[j].top;
                numPos[i].bottom ^= numPos[j].bottom;
                numPos[j].bottom ^= numPos[i].bottom;
                numPos[i].bottom ^= numPos[j].bottom;
            }
        }
    }

    for(int i=0; i<num; i++){
        do{
            dx = rand()%42 - 21;
            dy = rand()%Height - (int)(Height/2);
            numPosTrans[i].left = numPos[i].left + dx;
            numPosTrans[i].right = numPos[i].right + dx;
            numPosTrans[i].top = numPos[i].top + dy;
            numPosTrans[i].bottom = numPos[i].bottom + dy;
        }
        while(numPosTrans[i].left + dx < (i == 0 ? 0 : numPosTrans[i-1].right) || numPosTrans[i].right + dx >= (i == num-1 ? Width : numPos[i+1].left) || numPosTrans[i].top + dy < 0 || numPosTrans[i].bottom + dy >= Height);
    }

    for(int i=0; i<num; i++) CutandPaste(Pixels, finalPixels, Width, Height, BitsPerPixel, numPos[i], numPosTrans[i].left, numPosTrans[i].top);
    imshow("cutpastePixels", Mat(Height, Width, imgtest.channels() * 8 > 24 ? CV_8UC4 : CV_8UC3, &finalPixels[0]));
    noisePoint(finalPixels, Pixels, Width, Height, BitsPerPixel, 150 * num, 3);
    imshow("finals", Mat(Height, Width, imgtest.channels() * 8 > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]));
    if(num == 1){
        if(!dirExists((string)"ocr_numPack/" + tmpFile)){
            mkdir(((string)"ocr_numPack/" + tmpFile).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
        imwrite((string)"ocr_numPack/" + tmpFile + (string)"/" + fullName + (string)"_" + to_string(100000000 - rand()%90000000) + (string)".jpg", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]));
    }
    else{
        imwrite((string)"ocr_numPack/" + fullName + (string)"_" + to_string(100000000 - rand()%90000000) + (string)".jpg", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]));
    }
    // waitKey(0);
}

void colorSolve(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& colorPixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3, gate = 30, colorNum = 0, pos, dx, dy, rangeMinX[2] = {10, 32}, rangeMinY[2] = {10, 32}, dotTmp, poolR = 1;
    float Dcolor;
    bool ISfitColor;
    std::vector<COLOR> colors;
    std::vector<RECT> colorsRange;
    std::vector<uint8_t> tmpPixels;

    colorPixels.resize(Width * Height * bit);
    tmpPixels.resize(Width * Height * bit);

    imshow("origPixels", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]));
    for(int y=0; y<Height; y++){
        for(int x=0; x<Width; x++){
            // k = 012 BGR
            ISfitColor = 0;
            for(int c=0; c<colorNum; c++){
                Dcolor = 0;
                Dcolor += pow(Pixels[x*bit + y*Width*bit] - colors[c].B, 2);
                Dcolor += pow(Pixels[x*bit + y*Width*bit + 1] - colors[c].G, 2);
                Dcolor += pow(Pixels[x*bit + y*Width*bit + 2] - colors[c].R, 2);
                Dcolor = sqrt(Dcolor);
                if(Dcolor < gate){
                    colorPixels[x*bit + y*Width*bit] = colors[c].B;
                    colorPixels[x*bit + y*Width*bit + 1] = colors[c].G;
                    colorPixels[x*bit + y*Width*bit + 2] = colors[c].R;
                    ISfitColor = 1;

                    colorsRange[c].left = min(x, colorsRange[c].left);
                    colorsRange[c].right = max(x, colorsRange[c].right);
                    colorsRange[c].top = min(y, colorsRange[c].top);
                    colorsRange[c].bottom = max(y, colorsRange[c].bottom);
                    break;
                }
            }
            if(!ISfitColor){
                colors.resize(++colorNum);
                colors[colorNum-1].B = Pixels[x*bit + y*Width*bit];
                colors[colorNum-1].G = Pixels[x*bit + y*Width*bit + 1];
                colors[colorNum-1].R = Pixels[x*bit + y*Width*bit + 2];

                colorsRange.resize(colorNum);
                colorsRange[colorNum-1].left = x;
                colorsRange[colorNum-1].right = x;
                colorsRange[colorNum-1].top = y;
                colorsRange[colorNum-1].bottom = y;
            }
        }
    }
    imshow("colorPixels", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &colorPixels[0]));
    // for(int c=0; c<colorNum; c++) drawBorder(colorPixels, Width, Height, BitsPerPixel, colorsRange[c]);

    // for(int c=0; c<colorNum; c++){
    //     dx = colorsRange[c].right - colorsRange[c].left;
    //     dy = colorsRange[c].bottom - colorsRange[c].top;
    //     // if(rangeMinX[1] < dx || dx < rangeMinX[0] || rangeMinY[1] < dy || dy < rangeMinY[0]){
    //     if(10<dx && dx<40 && 10<dy && dy<40){
    //         for(int dc=c+1; dc<colorNum; dc++){
    //             colors[dc-1].B = colors[dc].B;
    //             colors[dc-1].G = colors[dc].G;
    //             colors[dc-1].R = colors[dc].R;
    //             colorsRange[dc-1].left = colorsRange[dc].left;
    //             colorsRange[dc-1].right = colorsRange[dc].right;
    //             colorsRange[dc-1].top = colorsRange[dc].top;
    //             colorsRange[dc-1].bottom = colorsRange[dc].bottom;
    //             colorNum--;
    //         }
    //     }
    // }

    // for(int y=0; y<Height; y++){
    //     for(int x=0; x<Width; x++){
    //         for(int c=0; c<colorNum; c++){
    //             pos = x*bit + y*Width*bit;
    //             if((int)colorPixels[pos] == colors[c].B && (int)colorPixels[pos + 1] == colors[c].G && (int)colorPixels[pos + 2] == colors[c].R){
    //                 // colorPixels[pos] = colorPixels[pos + 1] = colorPixels[pos + 2] = 255;
    //                 colorPixels[pos] = (int)(c/64) * 32;
    //                 colorPixels[pos + 1] = (int)((c%64)/8) * 32;
    //                 colorPixels[pos + 2] = (c%8) * 32;
    //                 break;
    //             }
    //         }
    //     }
    // }

    // for(int i=0; i<Width * Height * bit; i++) tmpPixels[i] = colorPixels[i];
    
    imgFloor(colorPixels, tmpPixels, Width, Height, BitsPerPixel, 64);
    imshow("floorPixels", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &tmpPixels[0]));

    for(int y=poolR; y<Height-poolR; y++){
        for(int x=poolR; x<Width-poolR; x++){
            pos = x*bit + y*Width*bit;
            dotTmp = 0;
            if(tmpPixels[pos] == 255) continue;
            for(int j=-poolR; j<1+poolR; j++){
                for(int i=-poolR; i<1+poolR; i++){
                    // if(tmpPixels[(x+i)*bit + (y+j)*Width*bit] == tmpPixels[pos] && tmpPixels[(x+i)*bit + (y+j)*Width*bit + 1] == tmpPixels[pos+1] && tmpPixels[(x+i)*bit + (y+j)*Width*bit + 2] == tmpPixels[pos+2]) dotTmp++;
                    if(tmpPixels[(x+i)*bit + (y+j)*Width*bit] == 255 && tmpPixels[(x+i)*bit + (y+j)*Width*bit + 1] == 255 && tmpPixels[(x+i)*bit + (y+j)*Width*bit + 2] == 255) dotTmp++;
                }
            }
            if(dotTmp > 6){
                for(int j=-poolR; j<1+poolR; j++){
                    for(int i=-poolR; i<1+poolR; i++){
                        tmpPixels[(x+i)*bit + (y+j)*Width*bit] = tmpPixels[(x+i)*bit + (y+j)*Width*bit + 1] = tmpPixels[(x+i)*bit + (y+j)*Width*bit + 2] = 255;
                    }
                }
            }
        }
    }

    // for(int i=0; i<Width * Height * bit; i++) tmpPixels[i] = min(max(tmpPixels[i]*tmpPixels[i]/192, 0), 255);
    // GammaTransform(tmpPixels, colorPixels, Width, Height, BitsPerPixel, 0.5);
    
    twoValue(tmpPixels, tmpPixels, Width, Height, BitsPerPixel, 200);
    // findBlock(tmpPixels, Width, Height, BitsPerPixel, 32, 32, colorsRange, 2, 0);
    // for(int i=0; i<colorsRange.size(); i++) drawBorder(tmpPixels, Width, Height, BitsPerPixel, colorsRange[i]);

    cout << colorNum << "\n";
    imshow("finalPixels", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &tmpPixels[0]));
    waitKey(0);
}

void imgFilter(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& filterPixels, std::vector<uint8_t>& resPixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3;

    resPixels.resize(Width * Height * bit);
    filterPixels.resize(Width * Height * bit);

    for(int i=0; i<Width * Height; i++){
        if(filterPixels[i*bit] == 255) resPixels[i*bit] = resPixels[i*bit + 1] = resPixels[i*bit + 2] = 255;
        else for(int k=0; k<3; k++) resPixels[i*bit + k] = Pixels[i*bit + k];
    }
}

void colorGroup(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& tmpPixels, int Width, int Height, int BitsPerPixel, int x, int y, RECT *colorsRange, COLOR dotColor, float colorGate, float distance){
    int bit = BitsPerPixel > 24 ? 4 : 3, colorNum = 0;
    float Dcolor;
    COLOR thisColor;
    tmpPixels.resize(Width * Height * bit);

    for(int k=0; k<3; k++) tmpPixels[x*bit + y*Width*bit + k] = 255;

    for(int dy=-ceil(distance); dy<=ceil(distance); dy++){
        for(int dx=-ceil(distance); dx<=ceil(distance); dx++){
            // k = 012 BGR
            if(dx*dx + dy*dy > distance*distance) continue;
            if(x+dx<0 || x+dx>=Width || y+dy<0 || y+dy>=Height) continue;
            if(tmpPixels[(x+dx)*bit + (y+dy)*Width*bit] == 0 && Pixels[(x+dx)*bit + (y+dy)*Width*bit] != 255){
                Dcolor = 0;
                Dcolor += pow(Pixels[(x+dx)*bit + (y+dy)*Width*bit] - dotColor.B, 2);
                Dcolor += pow(Pixels[(x+dx)*bit + (y+dy)*Width*bit + 1] - dotColor.G, 2);
                Dcolor += pow(Pixels[(x+dx)*bit + (y+dy)*Width*bit + 2] - dotColor.R, 2);
                Dcolor = sqrt(Dcolor);
                if(Dcolor < colorGate){
                    colorsRange->left = min(colorsRange->left, x + dx);
                    colorsRange->right = max(colorsRange->right, x + dx);
                    colorsRange->top = min(colorsRange->top, y + dy);
                    colorsRange->bottom = max(colorsRange->bottom, y + dy);
                    thisColor.B = Pixels[(x+dx)*bit + (y+dy)*Width*bit];
                    thisColor.G = Pixels[(x+dx)*bit + (y+dy)*Width*bit + 1];
                    thisColor.R = Pixels[(x+dx)*bit + (y+dy)*Width*bit + 2];
                    colorGroup(Pixels, tmpPixels, Width, Height, BitsPerPixel, x+dx, y+dy, colorsRange, thisColor, colorGate, distance);
                }
            }
        }
    }
}

void gridFind(std::vector<uint8_t>& Pixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3, mul[5] = {-1,-1,0,1,1}, multmp, dotNumX[Width][Height], dotNumY[Width], W = 8, H = 2, dotMax = 0, dotNum;
    int dmul[3] = {1,-2,1}, dmultmp;
    float tmp;
    bool countX = 0, countY = 0, inRange;
    std::vector<uint8_t> tmpPixels, tmpPixels2;
    std::vector<int> XposST, XposED, YposST, YposED, dmulV;
    tmpPixels.resize(Width * Height * bit);
    tmpPixels2.resize(Width * Height * bit);
    dmulV.resize(Width);

    for(int x=0;x<Width; x++){
        dotNumY[x] = 0;
        for(int y=0; y<Height; y++){
            for(int dx=-W; dx<1+W; dx++){
                if(x+dx<0 || x+dx>=Width) continue;
                if(Pixels[(x+dx)*bit + y*Width*bit] == 0) dotNumY[x]++;
            }
        }
    }
    
    for(int y=0; y<Height; y++){
        for(int x=0;x<Width; x++){
            dotMax = 1;
            for(int w=-15; w<16; w++){
                if(x+w<0 || x+w>=Width) continue;
                dotMax = max(dotMax, dotNumY[x+w]);
            }
            for(int k=0; k<3; k++) tmpPixels[x*bit + y*Width*bit + k] = 255 * (1.0 - (float)dotNumY[x] / (float)dotMax);
        }
    }
    // for(int i=0; i<Width * Height * bit; i++) tmpPixels2[i] = tmpPixels[i];
    // GaussBlur(tmpPixels2, tmpPixels, Width, Height, BitsPerPixel, 0.6);
    imshow("tmpPixelsY", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &tmpPixels[0]));
}

void colorSolve2(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& colorPixels, int Width, int Height, int BitsPerPixel){
    int bit = BitsPerPixel > 24 ? 4 : 3, pos, dotTmp, poolR, poolRLst[] = {1, 2, 3, 6}, poolRLstGate[] = {7, 22, 45, 162}, dx, dy, tmpl, tmpr;
    float Dcolor, colorDst = 30;
    float rotate[2][2] = {{cos(M_PI/4.0), -sin(M_PI/4.0)}, {sin(M_PI/4.0), cos(M_PI/4.0)}};
    bool ISfitColor;
    COLOR dotColor;
    std::vector<COLOR> colors;
    std::vector<RECT> colorsRange;
    std::vector<uint8_t> tmpPixels, filterPixels;
    std::vector<POINT> dotPoint;

    colorPixels.resize(Width * Height * bit);
    tmpPixels.resize(Width * Height * bit);
    filterPixels.resize(Width * Height * bit);
    colorsRange.resize(1);

    imshow("origPixels", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]));

    for(int i=0; i<Width * Height * bit; i++) {
        colorPixels[i] = max(Pixels[i]-20, 0);
        tmpPixels[i] = filterPixels[i] = 0;
    }
    imgFloor(colorPixels, colorPixels, Width, Height, BitsPerPixel, 64);
    // imshow("floorPixels", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &colorPixels[0]));
    twoValue(colorPixels, colorPixels, Width, Height, BitsPerPixel, 150);
    // imshow("twoValuePixels", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &colorPixels[0]));
    for(int pr=0;pr<4;pr++){
        poolR = poolRLst[pr];
        for(int y=poolR; y<Height-poolR; y++){
            for(int x=poolR; x<Width-poolR; x++){
                pos = x*bit + y*Width*bit;
                dotTmp = 0;
                if(colorPixels[pos] == 255) continue;
                for(int j=-poolR; j<1+poolR; j++){
                    for(int i=-poolR; i<1+poolR; i++){
                        if(x+i<0 || x+i >= Width || y+j<0 || y+j>Height) continue;
                        if(colorPixels[(x+i)*bit + (y+j)*Width*bit] == 255 && colorPixels[(x+i)*bit + (y+j)*Width*bit + 1] == 255 && colorPixels[(x+i)*bit + (y+j)*Width*bit + 2] == 255) dotTmp++;
                    }
                }
                if(dotTmp > poolRLstGate[pr]){
                    for(int j=-poolR; j<1+poolR; j++){
                        for(int i=-poolR; i<1+poolR; i++){
                            colorPixels[(x+i)*bit + (y+j)*Width*bit] = colorPixels[(x+i)*bit + (y+j)*Width*bit + 1] = colorPixels[(x+i)*bit + (y+j)*Width*bit + 2] = 255;
                        }
                    }
                }
            }
        }
    }
    imshow("colorPixels", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &colorPixels[0]));
    gridFind(colorPixels, Width, Height, BitsPerPixel);
    
    imgFilter(Pixels, colorPixels, filterPixels, Width, Height, BitsPerPixel);
    imshow("filterPixels", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &filterPixels[0]));
    for(int i=0; i<Width * Height * bit; i++) tmpPixels[i] = 0;
    for(int y=0; y<Height; y++) {
        for(int x=0; x<Width; x++) {
            colorsRange.back().right = colorsRange.back().left = x;
            colorsRange.back().top = colorsRange.back().bottom = y;
            dotColor.B = filterPixels[x*bit + y*Width*bit];
            dotColor.G = filterPixels[x*bit + y*Width*bit + 1];
            dotColor.R = filterPixels[x*bit + y*Width*bit + 2];
            colorGroup(filterPixels, tmpPixels, Width, Height, BitsPerPixel, x, y, &colorsRange.back(), dotColor, 25, 3);
            dx = colorsRange.back().right - colorsRange.back().left;
            dy = colorsRange.back().bottom - colorsRange.back().top;
            if((10<dx && dx<305) || (8<dy && dy<305)){
                colorsRange.resize(colorsRange.size() + 1);
            }
        }
    }
    colorsRange.resize(colorsRange.size() - 1);
    cout<<"bef "<<colorsRange.size()<<"\n";

    for(int i=0; i<colorsRange.size(); i++){;
        dx = colorsRange[i].right - colorsRange[i].left;
        dy = colorsRange[i].bottom - colorsRange[i].top;
        if(dx <= 10 || dx >= 34 || dy <= 10 || dy >= 34 || 1){
            for(int j=i+1; j<colorsRange.size(); j++){
                int tmpW, tmpH;
                RECT tmpRC;
                tmpRC.left = min(colorsRange[i].left, colorsRange[j].left);
                tmpRC.right = max(colorsRange[i].right, colorsRange[j].right);
                tmpRC.top = min(colorsRange[i].top, colorsRange[j].top);
                tmpRC.bottom = max(colorsRange[i].bottom, colorsRange[j].bottom);
                tmpW = tmpRC.right-tmpRC.left;
                tmpH = tmpRC.bottom-tmpRC.top;
                if((10 <= tmpW && tmpW < 36) && (10 <= tmpH && tmpH < 36) && (abs(colorsRange[i].left-colorsRange[j].right)<=8 || abs(colorsRange[i].right-colorsRange[j].left)<=8)){
                    colorsRange[i] = tmpRC;
                    for(int k=j+1; k<colorsRange.size(); k++){
                        colorsRange[k-1] = colorsRange[k];
                    }
                    i--;
                    colorsRange.resize(colorsRange.size()-1);
                    break;
                }
            }
        }
    }
    
    std::sort(std::begin(colorsRange), std::end(colorsRange), [](RECT a, RECT b){return a.left > b.left;});
    for(int i=0; i<colorsRange.size(); i++){
        dx = colorsRange[i].right - colorsRange[i].left;
        dy = colorsRange[i].bottom - colorsRange[i].top;
        if(dy/dx > 1.5){
            for(int k=i+1; k<colorsRange.size(); k++){
                colorsRange[k-1] = colorsRange[k];
            }
            i--;
            colorsRange.resize(colorsRange.size()-1);
        }
        tmpl = min(colorsRange[i].left, colorsRange[i+1].left);
        tmpr = max(colorsRange[i].right, colorsRange[i+1].right);
        if(0 && i+1 < colorsRange.size() && ((tmpr - tmpl) / (colorsRange[i+1].right - colorsRange[i+1].left + colorsRange[i].right - colorsRange[i].left) < 0.5)){
            if(colorsRange[i+1].right - colorsRange[i+1].left > colorsRange[i].right - colorsRange[i].left){
                for(int k=i; k<colorsRange.size()-1; k++){
                    colorsRange[k] = colorsRange[k+1];
                }
            }
            else{
                for(int k=i+1; k<colorsRange.size()-1; k++){
                    colorsRange[k] = colorsRange[k+1];
                }
            }
            i--;
            colorsRange.resize(colorsRange.size()-1);
        }
    }

    cout<<"aft "<<colorsRange.size()<<"\n";

    for(int i=0; i<Width * Height * bit; i++) tmpPixels[i] = 255;
    for(int i=0; i<colorsRange.size(); i++) matrixTrans(colorPixels, tmpPixels, Width, Height, BitsPerPixel, colorsRange[i], rotate);
    imshow("rotatePixels", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &tmpPixels[0]));

    for(int i=0; i<colorsRange.size(); i++) drawBorder(colorPixels, Width, Height, BitsPerPixel, colorsRange[i]);
    
    imshow("finalPixels", Mat(Height, Width, BitsPerPixel > 24 ? CV_8UC4 : CV_8UC3, &colorPixels[0]));
    waitKey(0);
}

void matchNum(std::vector<uint8_t>& Pixels, std::vector<uint8_t>& colorPixels, int Width, int Height, int BitsPerPixel, string ans){
    int bit = BitsPerPixel > 24 ? 4 : 3;
    int widths[] = {16, 24, 32}, heights[] = {16, 24, 32};
    RECT theRange;
    std::vector<uint8_t> tmpPixels, filterPixels;

    colorPixels.resize(Width * Height * bit);
    tmpPixels.resize(Width * Height * bit);
    filterPixels.resize(Width * Height * bit);

    for(int w=0; w<3; w++){
        for(int h=0; h<3; h++){
            
        }
    }
}