#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath> 
#include <opencv2/opencv.hpp>
// #include <cstdint>
#include <cstring>
#include <vector>
// #include <tiffio.h>
#include <string>
// #include <sstream>
// #include <dirent.h>

#include "systemOS.h"

#ifdef SYS_LINUX
#endif
#ifdef SYS_WINDOWS
#include <windows.h>
#endif

#include "picture.h"


RANGE::RANGE(){
    this->left = this->right = this->top = this->bottom = 0;
};
RANGE::RANGE(int L, int R, int T, int B){
    this->left = L;
    this->right = R;
    this->top = T;
    this->bottom = B;
    this->regularize();
};
RANGE::RANGE(std::initializer_list<int> POS){
    int size = POS.size();
    this->left = size > 0 ? *(POS.begin() + 0) : 0;
    this->right = size > 1 ? *(POS.begin() + 1) : 0;
    this->top = size > 2 ? *(POS.begin() + 2) : 0;
    this->bottom = size > 3 ? *(POS.begin() + 3) : 0;
    this->regularize();
};

void RANGE::retract(RANGE range){
    this->left = MAX(this->left, range.left);
    this->right = MIN(this->right, range.right);
    this->top = MAX(this->top, range.top);
    this->bottom = MIN(this->bottom, range.bottom);
};
void RANGE::regularize(){
    int tmp;
    if(this->left > this->right){
        tmp = this->left;
        this->left = this->right;
        this->right = tmp;
    }
    if(this->top > this->bottom){
        tmp = this->top;
        this->top = this->bottom;
        this->bottom = tmp;
    }
};




inline double getFloatValueAt(PICTURE &sourcePic, double x, double y, int k){
    if(x<0 || x>=sourcePic.width || y<0 || y>=sourcePic.height) return 255;
    int X = floor(x), Y = floor(y);
    double rtn = 0, dx = x - X, dy = y - Y;
    rtn += (1.0-dx) * (1.0-dy) * (double)sourcePic.Pixels[X * sourcePic.bit + Y * sourcePic.width * sourcePic.bit + k];
    rtn += (1.0-dx) * dy * (double)sourcePic.Pixels[X * sourcePic.bit + (Y+1) * sourcePic.width * sourcePic.bit + k];
    rtn += dx * (1.0-dy) * (double)sourcePic.Pixels[(X+1) * sourcePic.bit + Y * sourcePic.width * sourcePic.bit + k];
    rtn += dx * dy * (double)sourcePic.Pixels[(X+1) * sourcePic.bit + (Y+1) * sourcePic.width * sourcePic.bit + k];
    return rtn;
};



// for linux
#ifdef SYS_LINUX
PICTURE::PICTURE(){};
PICTURE::PICTURE(char* window){
    this->hwnd = exec((std::string)"xdotool search --name \"" + this->hwnd + "\"");
};
PICTURE::PICTURE(int width, int height){
    this->width = width;
    this->height = height;
};
PICTURE::PICTURE(int width, int height, char* window){
    this->width = width;
    this->height = height;
    this->hwnd = exec((std::string)"xdotool search --name \"" + this->hwnd + "\"");
};

PICTURE PICTURE::setWindow(char* window){
    this->hwnd = exec((std::string)"xdotool search --name \"" + this->hwnd + "\"");
    return *this;
};
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

PICTURE PICTURE::ScreenShot(int dx, int dy){
    // Display* display = XOpenDisplay(nullptr);
    // Window root = DefaultRootWindow(display);
    Display* display = XOpenDisplay(nullptr);
    Window root = ScreenOfDisplay(display, 0)->root;
    XWindowAttributes attributes = {0};
    // XGetWindowAttributes(display, root, &attributes);
    this->hwnd = exec((std::string)"xdotool search --name \"" + this->hwnd + "\"");
    cout<<this->hwnd<<"\n";
    XGetWindowAttributes(display, atoi(this->hwnd.data()), &attributes); //
    // FileZilla 
    // Google Chrome 14680087
    this->width = MIN(this->width, attributes.width);
    this->height = MIN(this->height, attributes.height);
    cout<<x<<" "<<y<<"\n";
    XImage* img = XGetImage(display, root, dx, dy , this->width, this->height, AllPlanes, ZPixmap);
    this->bit = img->bits_per_pixel/8;
    this->Pixels.resize(this->width, this->height);

    memcpy(&this->Pixels[0], img->data, this->Pixels.size());

    XDestroyImage(img);
    XCloseDisplay(display);
}
#endif









// https://superkogito.github.io/blog/CaptureScreenUsingGdiplus.html
// https://superkogito.github.io/blog/2020/07/26/capture_screen_using_gdiplus.html
#ifdef SYS_WINDOWS
PICTURE::PICTURE(){};
PICTURE::PICTURE(char* window){
    this->hwnd = FindWindow(NULL, (LPCSTR)window);
};
PICTURE::PICTURE(int width, int height){
    this->width = width;
    this->height = height;
};
PICTURE::PICTURE(int width, int height, char* window){
    this->width = width;
    this->height = height;
    this->hwnd = FindWindow(NULL, (LPCSTR)window);
};

PICTURE PICTURE::setWindow(char* window){
    this->hwnd = FindWindow(NULL, (LPCSTR)window);
    return *this;
};

BITMAPINFOHEADER createBitmapHeader(int width, int height){
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
};

PICTURE PICTURE::screenShot(int dx, int dy){
    cv::Mat src;
    RECT MSwh;
    GetWindowRect(this->hwnd, &MSwh);
    // get handles to a device context (DC)
    HDC hwindowDC = GetDC(this->hwnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    // define scale, height and width
    int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
    // int Fullwidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    // int Fullheight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    this->width = MIN(this->width, MSwh.right - MSwh.left);
    this->height = MIN(this->height, MSwh.bottom - MSwh.top);
    // cout << MSwh.right - MSwh.left << " " << MSwh.bottom - MSwh.top << "\n";
    // create mat object
    src.create(this->height, this->width, CV_8UC4);

    // create a bitmap
    HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, this->width, this->height);
    BITMAPINFOHEADER bi = createBitmapHeader(this->width, this->height);

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);

    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, this->width, this->height, hwindowDC, screenx + dx, screeny + dy, this->width, this->height, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    // GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, Pixels, (BITMAPINFO *)&bi, DIB_RGB_COLORS); //copy from hwindowCompatibleDC to hbwindow
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, this->height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS); //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(this->hwnd, hwindowDC);

    this->bit = src.channels();
    this->resize(this->width, this->height, this->bit);
    this->Pixels.assign(src.data, src.data + src.total()*src.channels());
    return *this;
};
#endif
void PICTURE::display(){
    cv::imshow("Display P", cv::Mat(this->height, this->width, this->bit > 3 ? CV_8UC4 : CV_8UC3, &this->Pixels[0]));
    cv::waitKey(0);
}
PICTURE PICTURE::resize(int width, int height, int bit){
    if((int)this->Pixels.size() < width * height * bit) this->Pixels.resize(width * height * bit);
    this->width = width;
    this->height = height;
    this->bit = bit;
    return *this;
};






PICTURE PICTURE::twoValue(PICTURE &targetPic, int gate){
    int tmp;
    targetPic.resize(this->width, this->height, this->bit);
    for(int j=0;j<this->height;j++){
        for(int i=0;i<this->width;i++){
            tmp = 0;
            for(int k=0;k<3;k++){
                tmp += Pixels[i*this->bit + j*this->width*this->bit + k];
            }
            tmp /= 3;
            for(int k=0;k<3;k++){
                if(tmp > gate) targetPic.Pixels[i*this->bit + j*this->width*this->bit + k] = 255;
                else targetPic.Pixels[i*this->bit + j*this->width*this->bit + k] = 0;
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::subPic(PICTURE &targetPic, RANGE range){
    int idx=0;
    range.regularize();
    range.retract(RANGE(0, this->width, 0, this->height));
    targetPic.resize((range.right - range.left + 1), (range.bottom - range.top + 1), this->bit);
    for(int j=range.top; j<=range.bottom; j++){
        for(int i=range.left; i<=range.right; i++){
            if(i<0 || i>=this->width || j<0 || j>=this->height){
                targetPic.Pixels[idx++] = 0;
            }
            else{
                for(int k=0;k<3;k++){
                    targetPic.Pixels[idx++] = this->Pixels[i*this->bit + j*this->width*this->bit + k];
                }
                if(targetPic.bit == 4)targetPic.Pixels[idx++] = 0;
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::GaussBlur(PICTURE &targetPic, double rate){
    int r = 3*rate+0.5, w, p, q;
    w = 2*r+1;
    float G[w], tmp;
    for(int i=0;i<w;i++){
        G[i] = pow(M_E, -((r-i)*(r-i))/(2*rate*rate))/sqrt(2*M_PI*rate*rate);
    }
    targetPic.resize(this->width, this->height, this->bit);
    for(int j=0;j<this->height;j++){
        for(int i=0;i<this->width;i++){
            for(int k=0;k<3;k++){
                targetPic.Pixels[i*this->bit + j*this->width*this->bit + k] = 0;
                tmp = 0.0;
                for(int n=0;n<w;n++){
                    q = j+n-r;
                    if(q<0) q=-q;
                    else if(q>=this->height) q=2*this->height-q-2;
                    tmp += G[n] * (float)this->Pixels[i*this->bit + q*this->width*this->bit + k];
                }
                targetPic.Pixels[i*this->bit + j*this->width*this->bit + k] = MAX(MIN(tmp, 255), 0);
            }
        }
    }
    for(int j=0;j<this->height;j++){
        for(int i=0;i<this->width;i++){
            for(int k=0;k<3;k++){
                tmp = 0.0;
                for(int n=0;n<w;n++){
                    p = i+n-r;
                    if(p<0) p=-p;
                    else if(p>=this->width) p=2*this->width-p-2;
                    tmp += G[n] * (float)targetPic.Pixels[p*this->bit + j*this->width*this->bit + k];
                }
                targetPic.Pixels[i*this->bit + j*this->width*this->bit + k] = MAX(MIN(tmp, 255), 0);
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::twist(PICTURE &targetPic, int posx, int posy, double rate){
    double newx, newy;
    double r2, sine, cosine, theta, rmax = MIN(MIN(posx, posy),MIN(this->width - posx, this->height - posy));
    targetPic.resize(this->width, this->height, this->bit);
    for(int j=0;j<this->height;j++){
        for(int i=0;i<this->width;i++){
            r2 = (i-posx)*(i-posx) + (j-posy)*(j-posy);
            theta = rate * pow((1 - MIN(sqrt(r2), rmax) / rmax),2) * M_PI / 180.0;
            sine = sin(theta);
            cosine = cos(theta);
            newx = MAX( MIN(cosine * (i-posx) - sine * (j-posy)+posx, this->width-1), 0);
            newy = MAX( MIN(sine * (i-posx) + cosine * (j-posy)+posy, this->height-1), 0);
            for(int k=0;k<3;k++){
                targetPic.Pixels[i * this->bit + j * this->width * this->bit + k] = getFloatValueAt(*this, newx, newy, k);
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::blackhole(PICTURE &targetPic, int posx, int posy, double gravity, double factor){
    float r, newx, newy;
    targetPic.resize(this->width, this->height, this->bit);
    posx = MAX(MIN(posx, this->width), 0);
    posy = MAX(MIN(posy, this->height), 0);

    for(int j=0;j<this->height;j++){
        for(int i=0;i<this->width;i++){
            r = sqrt((i-posx)*(i-posx) + (j-posy)*(j-posy));
            newx = (i-posx) * pow( 1 + 64*gravity/(pow(r+15,1.9)+10), factor ) + posx;
            newy = (j-posy) * pow( 1 + 64*gravity/(pow(r+15,1.9)+10), factor ) + posy;
            for(int k=0;k<3;k++){
// std::cout<<"@ "<<newx<<", "<<newy<<", "<<k<<", "<<targetPic.Pixels.size()<<", "<<targetPic.bit<<", "<<getFloatValueAt(*this, newx, newy, k)<<"\n";
                targetPic.Pixels[i*this->bit + j*this->width*this->bit + k] = getFloatValueAt(*this, newx, newy, k);
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::negative(PICTURE &targetPic){
    targetPic.resize(this->width, this->height, this->bit);
    for(int j=0;j<this->height;j++){
        for(int i=0;i<this->width;i++){
            for(int k=0;k<3;k++){
                targetPic.Pixels[i*this->bit + j*this->width*this->bit + k] = 255 - Pixels[i*this->bit + j*this->width*this->bit + k];
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::RGBtoYUV(PICTURE &targetPic, char type){
    double tmp;
    targetPic.resize(this->width, this->height, this->bit);
    for(int j=0;j<this->height;j++){
        for(int i=0;i<this->width;i++){
            if(type == 'Y')tmp = (float)Pixels[i*this->bit + j*this->width*this->bit]*0.299 + (float)Pixels[i*this->bit + j*this->width*this->bit + 1]*0.587 + (float)Pixels[i*this->bit + j*this->width*this->bit + 2]*0.114;
            else if(type == 'U')tmp = (float)Pixels[i*this->bit + j*this->width*this->bit]*(-0.169) + (float)Pixels[i*this->bit + j*this->width*this->bit + 1]*(-0.331) + (float)Pixels[i*this->bit + j*this->width*this->bit + 2]*0.5 + 128;
            else if(type == 'V')tmp = (float)Pixels[i*this->bit + j*this->width*this->bit]*0.5 + (float)Pixels[i*this->bit + j*this->width*this->bit + 1]*(-0.419) + (float)Pixels[i*this->bit + j*this->width*this->bit + 2]*(-0.081) + 128;
            else tmp = (float)Pixels[i*this->bit + j*this->width*this->bit]/3.0 + (float)Pixels[i*this->bit + j*this->width*this->bit + 1]/3.0 + (float)Pixels[i*this->bit + j*this->width*this->bit + 2]/3.0;
            for(int k=0;k<3;k++){
                targetPic.Pixels[i*this->bit + j*this->width*this->bit + k] = tmp;
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::GammaTransform(PICTURE &targetPic, double gamma){
    targetPic.resize(this->width, this->height, this->bit);
    for(int k=0;k<4;k++){
        for(int j=0;j<this->height;j++){
            for(int i=0;i<this->width;i++){
                targetPic.Pixels[i*this->bit + j*this->width*this->bit + k] = MAX( MIN( round( pow( Pixels[i*this->bit + j*this->width*this->bit + k] / 255.0, gamma) * 255.0), 255), 0);
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::jpeg_compress(PICTURE &targetPic, double Q){
    int W_8 = this->width/8 + 1, H_8 = this->height/8 + 1;
    float Qm[8][8] = {{16,11,10,16,24,40,51,61},{12,12,14,19,26,58,60,55},{14,13,16,24,40,57,69,56},{14,17,22,29,51,87,80,62},{18,22,37,56,68,109,103,77},{24,35,55,64,81,104,113,92},{49,64,78,87,103,121,120,101},{72,92,95,98,112,100,103,99}};
    float S = Q < 50 ? 5000.0/Q : 200.0 - 2.0*Q, g[8][8], G[8][8], au, av;
    float omega[64];
    targetPic.resize(this->width, this->height, this->bit);
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
                        if(8*i+x >= this->width || 8*j+y >= this->height) g[x][y] = 0;
                        else g[x][y] = (int)this->Pixels[(8*i+x)*this->bit + (8*j+y)*this->width*this->bit + k] - 128;
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
                        if(8*i+u < this->width && 8*j+v < this->height) targetPic.Pixels[(8*i+u)*this->bit + (8*j+v)*this->width*this->bit + k] = (char)G[u][v];
                    }
                }
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::jpeg_decompress(PICTURE &targetPic, double Q){
    int W_8 = this->width/8 + 1, H_8 = this->height/8 + 1;
    float Qm[8][8] = {{16,11,10,16,24,40,51,61},{12,12,14,19,26,58,60,55},{14,13,16,24,40,57,69,56},{14,17,22,29,51,87,80,62},{18,22,37,56,68,109,103,77},{24,35,55,64,81,104,113,92},{49,64,78,87,103,121,120,101},{72,92,95,98,112,100,103,99}};
    Q = Q < 1 ? 1 : Q;
    float S = Q < 50 ? 5000.0/Q : 200.0 - 2.0*Q, g[8][8], G[8][8], au, av;
    float omega[64];
    targetPic.resize(this->width, this->height, this->bit);
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
                        if(8*i+u >= this->width || 8*j+v >= this->height) G[u][v] = 0;
                        else G[u][v] = this->Pixels[(8*i+u)*this->bit + (8*j+v)*this->width*this->bit + k] * Qm[v][u];
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
                        if(8*i+x < this->width && 8*j+y < this->height) targetPic.Pixels[(8*i+x)*this->bit + (8*j+y)*this->width*this->bit + k] = MAX(MIN(g[x][y] + 128, 255), 0);
                    }
                }
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::sharp(PICTURE &targetPic, double rate){
    targetPic.resize(this->width, this->height, this->bit);
    this->GaussBlur(targetPic, rate);

    for(int j=0;j<this->height;j++){
        for(int i=0;i<this->width;i++){
            for(int k=0;k<3;k++){
                targetPic.Pixels[i*this->bit + j*this->width*this->bit + k] = MIN(MAX(2*this->Pixels[i*this->bit + j*this->width*this->bit + k] - targetPic.Pixels[i*this->bit + j*this->width*this->bit + k], 0), 255);
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::imgFloor(PICTURE &targetPic, int flr){
    targetPic.resize(this->width, this->height, this->bit);
    for(int j=0;j<this->height;j++){
        for(int i=0;i<this->width;i++){
            for(int k=0;k<3;k++){
                targetPic.Pixels[i*this->bit + j*this->width*this->bit + k] = this->Pixels[i*this->bit + j*this->width*this->bit + k] - this->Pixels[i*this->bit + j*this->width*this->bit + k] % flr + flr - 1;
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::matrixTrans(PICTURE &targetPic, int posx, int posy, double mat[4], RANGE range){
    double newx, newy;
    range.regularize();
    range.retract(RANGE(0, this->width, 0, this->height));
    targetPic.resize(this->width, this->height, this->bit);
    for(int j=range.top; j<=range.bottom; j++){
        for(int i=range.left; i<=range.right; i++){
            newx = MAX( MIN(mat[0] * (i-posx) + mat[1] * (j-posy) + posx, this->width-1), 0);
            newy = MAX( MIN(mat[2] * (i-posx) + mat[3] * (j-posy) + posy, this->height-1), 0);
            for(int k=0;k<3;k++){
                targetPic.Pixels[i*this->bit + j*this->width*this->bit + k] = getFloatValueAt(*this, newx, newy, k);
            }
        }
    }
    return targetPic;
};
PICTURE PICTURE::rotateD(PICTURE &targetPic, int posx, int posy, double deg, RANGE range){
    double c = cos(deg*M_PI/180), s = sin(deg*M_PI/180), mat[4] = {c, -s, s, c};
    matrixTrans(targetPic, posx, posy, mat, range);
    return targetPic;
};