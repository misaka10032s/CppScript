# geometry.h
- class POINT2D
```
  POINT2D matrixTrans(POINT2D &resultP, const double mat[4], const POINT2D &datumP);
  POINT2D rotateR(POINT2D &resultP, double rad);
  POINT2D rotateD(POINT2D &resultP, double deg);
  POINT2D rotateR(POINT2D &resultP, double rad, POINT2D datumP);
  POINT2D rotateD(POINT2D &resultP, double deg, POINT2D datumP);
  POINT2D symmetry(POINT2D &resultP);
  POINT2D symmetry(POINT2D &resultP, POINT2D datumP);
  POINT2D symmetry(POINT2D &resultP, LINE2D datumL);
  POINT2D symmetryX(POINT2D &resultP);
  POINT2D symmetryY(POINT2D &resultP);
  POINT2D telescopic(POINT2D &resultP, double multipleX, double multipleY);
  POINT2D telescopic(POINT2D &resultP, double multipleX, double multipleY, POINT2D datumP);
  double distance(const POINT2D &point1);
  double distance(const LINE2D &line1);
```

- class LINE2D
```
  LINE2D();
  LINE2D(POINT2D a, POINT2D b);
  LINE2D(POINT2D a, double m);
  LINE2D(double a, double b, double c);

  double A,B,C;

  LINE2D rotateR(LINE2D &resultL, double rad);
  LINE2D rotateD(LINE2D &resultL, double deg);
  LINE2D rotateR(LINE2D &resultL, double rad, POINT2D datumP);
  LINE2D rotateD(LINE2D &resultL, double deg, POINT2D datumP);
  LINE2D symmetry(LINE2D &resultL);
  LINE2D symmetry(LINE2D &resultL, POINT2D datumP);
  LINE2D symmetry(LINE2D &resultL, LINE2D datumL);
  LINE2D symmetryX(LINE2D &resultL);
  LINE2D symmetryY(LINE2D &resultL);
  LINE2D angelBisector(LINE2D &resultL, LINE2D datumL);
  LINE2D angelBisector2(LINE2D &resultL,LINE2D datumL);
  LINE2D telescopic(LINE2D &resultL, double multipleX, double multipleY);
  LINE2D telescopic(LINE2D &resultL, double multipleX, double multipleY, POINT2D datumP);
  double distance(POINT2D point1);
  POINT2D intersection(LINE2D line1);
```

- class POLYGON
```
  POLYGON();
  POLYGON(std::vector<POINT2D> points);
  POLYGON(std::initializer_list<double> Xpoints, std::initializer_list<double> Ypoints);
  POLYGON(std::initializer_list<std::initializer_list<double>> points);

  std::vector<POINT2D> vertex;
  int vertexNum;
  POINT2D center;
  double radius, area;

  POLYGON rotateR(POLYGON &resultP, double rad);
  POLYGON rotateD(POLYGON &resultP, double deg);
  POLYGON rotateR(POLYGON &resultP, double rad, POINT2D datumP);
  POLYGON rotateD(POLYGON &resultP, double deg, POINT2D datumP);
  POLYGON symmetry(POLYGON &resultP);
  POLYGON symmetry(POLYGON &resultP, POINT2D datumP);
  POLYGON symmetry(POLYGON &resultP, LINE2D datumL);
  POLYGON symmetryX(POLYGON &resultP);
  POLYGON symmetryY(POLYGON &resultP);
  POLYGON telescopic(POLYGON &resultP, double multipleX, double multipleY);
  POLYGON telescopic(POLYGON &resultP, double multipleX, double multipleY, POINT2D datumP);
  double getRadius();
  double getArea();
  POINT2D getCenter();
  POLYGON update();
  bool isContain(POINT2D point1);
  bool isCollide(POLYGON polygon1);
```
- class CONICSECTION


# graph.h
- struct EDGE
```
  EDGE();
  EDGE(std::string fromV, std::string toV, int weight);
  EDGE(std::string fromV, std::string toV, int weight, bool directional);

  std::string fromV;
  std::string toV;
  int weight;
  bool directional;

  bool operator == (EDGE const &EDGE1) const;
  bool operator != (EDGE const &EDGE1) const;
```

- struct GRAPH
```
  GRAPH();

  void addEdge(std::string P0, std::string P1, int W);
  void addEdge(EDGE const &newEdge);

  int cost, vertexNum;
  std::vector<EDGE> edges;
  std::unordered_map<std::string, bool> vertices;
```
methods:
- GRAPH MST_P(std::vector\<EDGE\> const &links);
- GRAPH MST_K(std::vector\<EDGE\> const &links);
- GRAPH shortestRoute(std::vector<EDGE> const &links, std::string start, std::string end);

# heap.h
- struct NODE
```
  NODE(std::string key, int weight);
  NODE();

  std::string key;
  int weight;
  NODE* children[2];
  int childrenNum[2];
  NODE* parent;
```

- class HEAP
```
  private:
      bool htype; // 0 = min, 1 = max
      int minmaxMultiply;
      NODE *rootHeap = new NODE("", 0);

  public:
      HEAP(bool type);
      HEAP();
      ~HEAP();

      void push(int key, int weight);
      void push(std::string key, int weight);
      NODE pop();
      NODE get();
      void list(NODE *tmpHeap);
```

- class SMMH
```
  private:
      NODE* getValue(NODE *nodeList[2], int index);
      NODE *rootHeap = new NODE("", 0);

  public:
      SMMH();
      ~SMMH();

      void push(int, int weight);
      void push(std::string key, int weight);
      NODE pop(int popIdx);
      NODE pop();
      NODE popMin();
      NODE popMax();
      NODE getMin();
      NODE getMax();
      void list(NODE *tmpHeap);
```

- template <typename T> class listHEAP
```
  private:
      std::vector<T> _listheap;
      int getChild(int idx, bool lr);
      int getParent(int idx);
      std::function<bool(T, T)> compare;
      // bool *compare(int i0, int i1);

  public:
      listHEAP(bool (*cp)(T i0, T i1));
      ~listHEAP();
      void push(T item);
      T pop();
      void list();
```

- template <typename T> class STACK
```
  private:
      bool _direction;
      int _idx, _btmidx;
      std::vector<T> _STACK;
  public:
      STACK(int num);

      bool canPop;

      T pop();
      bool push(T item);
      void reset();
      void list();
      void reverse();
```

- template <typename T> class QUEUE
```
  private:
      bool _type, _direction; // _type 0 will detect full then stop push, 1 will cover the old data if queue is full
      int _popIdx, _pushIdx;
      std::vector<T> _QUEUE;
  public:
      QUEUE(int num, bool type);
      QUEUE(int num);

      bool canPop;

      T pop();
      bool push(T item);
      void reset();
      void list();
      void reverse();
```

methods:
- void swapNODE(NODE *N0, NODE *N1);
- int getChildrenLength(NODE checkingNode);

# number3.h

# picture.h
- class RANGE
```
  public:
      RANGE();
      RANGE(int L, int R, int T, int B);
      RANGE(std::initializer_list<int> POS);

      void retract(RANGE range);
      void regularize();

      int left, right, top, bottom;
```

- class PICTURE
```
  public:
      PICTURE();
      PICTURE(const char* window);
      PICTURE(int width, int height);
      PICTURE(int width, int height, const char* window);

      std::vector<uint8_t> Pixels;
      int width, height, bit;

      #ifdef SYS_LINUX
      std::string hwnd;
      #endif
      #ifdef SYS_WINDOWS
      HWND hwnd;
      #endif

      void display();
      void display(std::string windowName);
      PICTURE resize(int width, int height, int bit);
      PICTURE setWindow(char* window);

      bool open(std::string fileName);
      PICTURE screenShot(int dx, int dy);
      PICTURE twoValue(PICTURE &targetPic, int gate);
      PICTURE GaussBlur(PICTURE &targetPic, double rate);
      PICTURE twist(PICTURE &targetPic, int posx, int posy, double rate);
      PICTURE blackhole(PICTURE &targetPic, int posx, int posy, double gravity, double factor);
      PICTURE negative(PICTURE &targetPic);
      PICTURE RGBtoYUV(PICTURE &targetPic, char type);
      PICTURE GammaTransform(PICTURE &targetPic, double gamma);
      PICTURE jpeg_compress(PICTURE &targetPic, double Q);
      PICTURE jpeg_decompress(PICTURE &targetPic, double Q);
      PICTURE sharp(PICTURE &targetPic, double rate);
      PICTURE imgFloor(PICTURE &targetPic, int flr);
      PICTURE matrixTrans(PICTURE &targetPic, int posx, int posy, double mat[4], RANGE range);
      PICTURE rotateD(PICTURE &targetPic, int posx, int posy, double deg, RANGE range);
      PICTURE rotateQ(PICTURE &targetPic, int qNum);
      PICTURE ripple(PICTURE &targetPic, int posx, int posy, double density);
      PICTURE subPic(PICTURE &targetPic, RANGE range);
      PICTURE copyPaste(PICTURE &targetPic, RANGE oRange, int pX, int pY);
      PICTURE rgbMatrixTrans(PICTURE &targetPic, int mtx[9]);
```

# sorting.h
methods:
- template <typename T> bool increasing(T i0, T i1);
- template <typename T> bool decreasing(T i0, T i1);

- template <typename T> void quick_sort(T* start, int num, bool (*compare)(T i0, T i1));
- template <typename T> void merge_sort(T* start, int num, bool (*compare)(T i0, T i1));
- template <typename T> void heap_sort(T* start, int num, bool (*compare)(T i0, T i1));

# sudoku.h
unfinished

# systemFnc.h
- class SYS
```
  private:
      HWND targetWND;
      struct timeval timestamp;
      bool enable, waiting, check_WHD=1;
      unsigned char switchKey, maximizeKey, minimizeKey, focusKey, setForegroundWindow;
      long int timeCountDown;

  public:
      SYS(const char* window);

      RECT rctScreen, targetWNDsize;

      static std::string GetFocusWindowText();
      static void SaveToClipboard(const char* str);
      static int gettimeofday(struct timeval * tp, struct timezone * tzp);
      static std::string gettimestring();

      SYS setWindow(const char* window);
      SYS setWindow();
      void setKey(unsigned char key, unsigned char value);
      bool isEnable();
      long int getNowtick();
      void keybd(const char* btn, char type);
      void mouseAction(int x, int y, char type, bool abs, DWORD dwFlags);
      // mouse position absolute
      void mouseRC(int x, int y, char type);
      void mouseLC(int x, int y, char type);
      void mouseMC(int x, int y, char type);
      void mouseWH(int x, int y, char type);
      void mouseMV(int x, int y);
      // mouse position relative
      void mouseRCr(int dx, int dy, char type);
      void mouseLCr(int dx, int dy, char type);
      void mouseMCr(int dx, int dy, char type);
      void mouseWHr(int dx, int dy, char type);
      void mouseMVr(int dx, int dy);
      void wait(int dur); // dur ms
      void wait(int minDur, int rangeDur); // dur ms
      void minimize();
      void maximize();
      void focus();
      void pause();
      void setCountdown(long int time);
```
