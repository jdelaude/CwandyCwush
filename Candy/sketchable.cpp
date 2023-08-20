/*
Date : 22/08/2022
Author : Delaude Julien
N°mat : 000487533
*/
class Sketchable {
 public:
  virtual void draw() =0;
  virtual bool contains(Point p) const =0;
  virtual Point getCenter() const =0;
  virtual CandyFlavor getFlavor() const=0;
  virtual void setFlavor(CandyFlavor &other)=0;
  virtual void setFrameColor(Fl_Color newFrameColor)=0;
  virtual bool getState()=0;
  virtual bool isDestroyed()=0;
  virtual void toDestroy(bool up)=0;
  virtual ~Sketchable() {};
};
/*--------------------------------------------------
Candy class
Inhérits from Sketchable, to acces/modify private data
Takes position, flavor, size and bool to draw rectangle of PNG images
--------------------------------------------------*/
class Candy: public Sketchable {
  Fl_PNG_Image *kyandiA = new Fl_PNG_Image("images/A.png"); //FL_PNG_IMAGE needs -lfltk-images to compile without err
  Fl_PNG_Image *kyandiB = new Fl_PNG_Image("images/B.png");
  Fl_PNG_Image *kyandiC = new Fl_PNG_Image("images/C.png");
  Fl_PNG_Image *kyandiD = new Fl_PNG_Image("images/D.png");
  Fl_PNG_Image *kyandiE = new Fl_PNG_Image("images/E.png");
  Fl_PNG_Image *kyandiF = new Fl_PNG_Image("images/F.png");
  Fl_Image *SupprAni = new Fl_PNG_Image("images/explosion.png");
  Point center;
  int w, h;
  CandyFlavor flavor;
  Fl_Color frameColor;
  bool destroy;
 public:
  ~Candy(){};
  Candy(Point center, int w, int h, CandyFlavor &flavor, Fl_Color frameColor = FL_BLACK): center{center}, w{w}, h{h}, flavor{flavor}, frameColor{frameColor} {};
  bool getState(){};
  void setFrameColor(Fl_Color newFrameColor) override{
    frameColor=newFrameColor;
  }
  void setFlavor(CandyFlavor &newflavor) override{
    flavor = newflavor;
  }
  Fl_Color getFrameColor() {
    return frameColor;
  }
  bool isDestroyed(){
    return destroy;
  }
  void toDestroy(bool lever){
    destroy=lever;
  }
  void setWidth(int neww) {
    w = neww;
  }
  void setHeight(int newh) {
    h = newh;
  }
  int getWidth() {
    return w;
  }
  int getHeight() {
    return h;
  }
  CandyFlavor getFlavor()const override{
    return flavor;
  }
  Point getCenter() const override {
    return center;
  }
  void draw() {
    center.x = fl_transform_x(center.x,center.y);
    center.y = fl_transform_y(center.x,center.y);
    array<Point, 5> points{
    Point{center.x-w/2, center.y-h/2},
    Point{center.x-w/2, center.y+h/2},
    Point{center.x+w/2, center.y+h/2},
    Point{center.x+w/2, center.y-h/2},
    Point{center.x-w/2, center.y-h/2}};
    fl_begin_polygon();
    for (auto& point : points) {
      fl_vertex(point.x, point.y);

    }
    fl_end_polygon();
    fl_color(frameColor);
    fl_begin_line();
    for (auto& point : points) {
      fl_vertex(point.x, point.y);
    }
    
    fl_end_line();
    if(destroy){
      SupprAni->draw(center.x-w/2, center.y-h/2, w, h);
    }
    switch(flavor){
      case CandyFlavor::A:
          kyandiA -> draw(center.x-w/2, center.y-h/2, w, h);
          break;
      case CandyFlavor::B:
          kyandiB -> draw(center.x-w/2, center.y-h/2, w, h);
          break;
      case CandyFlavor::C:
          kyandiC -> draw(center.x-w/2, center.y-h/2, w, h);
          break;
      case CandyFlavor::D:
          kyandiD -> draw(center.x-w/2, center.y-h/2, w, h);
          break;
      case CandyFlavor::E:
          kyandiE -> draw(center.x-w/2, center.y-h/2, w, h);
          break;
      case CandyFlavor::F:
          kyandiF -> draw(center.x-w/2, center.y-h/2, w, h);
          break;
      default:
        break;
    }
    }
  bool contains(Point p) const {
    //cout <<50*p.x+50<<">"<<center.x-w/2<<endl;
    //cout <<50*p.x+50<<"<"<<center.x+w/2<<endl;
    //cout <<50*p.y+100<<">"<<center.y-h/2<<endl;
    //cout <<50*p.y+100<<"<"<<center.y+h/2<<endl;
    return 50*p.x+50>=center.x-w/2 &&
          50*p.x+50<center.x+w/2 &&
          50*p.y+100>=center.y-h/2 &&
          50*p.y+100<center.y+h/2;
  }
};

/*--------------------------------------------------
Animation class
Inhérit from Sketchable and can modify Candy through setter/getter
Takes a point when called to convert Sketchable into Animation and then start pushing into the translation matrix
--------------------------------------------------*/
class Animation: public Sketchable{
  protected:
  shared_ptr<Sketchable> toAnimate;
  public:
    Animation(shared_ptr<Sketchable> toAnimate): toAnimate{toAnimate} {}
    ~Animation(){};
    virtual bool isComplete()=0;
    virtual bool getState()=0;
    virtual void start(Point p) {
      shared_ptr<Animation> a = dynamic_pointer_cast<Animation>(toAnimate);
      if (a) a->start(p);
    };
    bool contains(Point p) const override{
      return toAnimate -> contains(p);
    }
    CandyFlavor getFlavor() const override{
      return toAnimate->getFlavor();
    }
    void setFlavor(CandyFlavor &other)override{
      return toAnimate->setFlavor(other);
    }
    void setFrameColor(Fl_Color newFrameColor) override{
      return toAnimate->setFrameColor(newFrameColor);
    }
    Point getCenter() const override{
      return toAnimate->getCenter();
    }
    bool isDestroyed()override{
    return toAnimate -> isDestroyed();
    }
    void toDestroy(bool lever)override{
      return toAnimate->toDestroy(lever);
    }
};

/*--------------------------------------------------
Swap class
Inhérits from Animation
Takes a int, a bool and proceed to build Swapping, at start we store a position to swap with
--------------------------------------------------*/

class Swap: public Animation {
  int duration;
  bool canswap;
  int time{0};
  Point currentTranslation();
  Point s{s.x=-1,s.y=-1};
  float signe_x,signe_y;
  public:
   Swap(shared_ptr<Sketchable>toAnimate,int duration=100): Animation{toAnimate}, duration{duration} {}
   ~Swap(){};
   void draw() override;
   bool getState() override;
   bool isComplete() override;
   void buildTranslation();
   void start(Point p) override{
    //cout<<"Redirect in Swap"<<endl;
    s.x = p.x;
    s.y = p.y;
    buildTranslation();
    Animation::start(p);
    canswap = true;
    time=0;
   }
};
void Swap::buildTranslation(){//Proceed to know which direction to translate to
    if (s.x >=0 && s.x <9 &&  s.y >=0 && s.y <9){
      Point tmp = toAnimate->getCenter();
      if ((tmp.x-25)/50<s.x){signe_x=1.4;}
      if ((tmp.x-25)/50>s.x){signe_x=-1.4;}
      if ((tmp.x-25)/50==s.x){signe_x=0;}
      if ((tmp.y-75)/50<s.y){signe_y=1.4;}
      if ((tmp.y-75)/50>s.y){signe_y=-1.4;}
      if ((tmp.y-75)/50==s.y){signe_y=0;}
    } else
    {signe_x = 0;
    signe_y = 0;}
}
void Swap::draw() {
  if (canswap){
    time++;
  }
  Translation r{currentTranslation()};
  toAnimate->draw();
  if(isComplete()){
    canswap=false;
    s.x =-1;
    s.y =-1;
  }
}
bool Swap::getState(){
  return canswap;
}
Point Swap::currentTranslation(){
  if(isComplete()){
    return {0,0};
  } 
  else {
    //cout<<tmp.x/50<<","<<tmp.y/ 50<<" to "<<s.x<<","<<s.y<<endl;
    return {static_cast<int>(signe_x*sin(pi*time/duration)),static_cast<int>(signe_y*sin(pi*time/duration))};
  }  
}
bool Swap::isComplete() {
  return time > duration;
}

/*--------------------------------------------------
Clickable class
Takes a shared_ptr of animation which leads to Sketchable so as Candy
Each one can be clicked to start the animation they're made for
--------------------------------------------------*/
class ClickableCell {
  shared_ptr<Animation> animation;
  bool match;
  public:
  ClickableCell(shared_ptr<Animation> animation): animation{animation}{}
  ~ClickableCell(){};
  void draw();
  void mouseClick(Point mouseLoc,Point mouseDest);
  bool contains(Point p){
    return animation -> contains(p);
  }
  Point getCenter() {
    return animation ->getCenter();
  }
  bool isComplete(){
    return animation ->isComplete();
  }
  void setFlavor(CandyFlavor &other){
      return animation->setFlavor(other);
    }
  CandyFlavor getSketchable(){
    return animation->getFlavor();
  }
  void isMatch(bool bin){
    match=bin;
  }
  bool getMatch(){
    return match;
  }
  bool isDestroyed(){
    return animation -> isDestroyed();
  }
  void toDestroy(bool lever){
    return animation->toDestroy(lever);
  }
  void setFrameColor(Fl_Color newFrameColor){
    return animation->setFrameColor(newFrameColor);
  }
  bool getState(){
    return animation->getState();
  }
};
void ClickableCell::draw() {
  animation -> draw();
  //Point tmp = animation->getCenter();
  //cout<<(tmp.x-25)/50<<","<<(tmp.y-75)/50<<" état : "<<animation-> isComplete()<<animation->getState()<<endl;
}
void ClickableCell::mouseClick(Point mouseLoc, Point mouseDest) {//takes MouseDest to be able to compare source from dest to determine which way the candy must go
  if (animation -> contains(mouseLoc)) {
    animation ->start(mouseDest);
  }
}
 