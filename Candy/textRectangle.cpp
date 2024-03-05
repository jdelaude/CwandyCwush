/*
Date : 22/08/2022
Author : Delaude Julien
*/
class Printable {
 public:
  virtual void draw ()=0;
  virtual ~Printable()=default;
};

/*--------------------------------------------------
Rectangle class
Construct a rectangle and draws it, can be modified
--------------------------------------------------*/
class Rectangle: public virtual Printable {
  Point center;
  int w, h;
  Fl_Color fillColor, frameColor;
 public:
  Rectangle(Point center, int w, int h,
            Fl_Color frameColor = FL_BLACK,
            Fl_Color fillColor = FL_BLACK);
  void draw() override;
  bool contains(Point p) const;
  void setFillColor(Fl_Color newFillColor);
  Fl_Color getFillColor() {
    return fillColor;
  }
  void setFrameColor(Fl_Color newFrameColor);
  Fl_Color getFrameColor() {
    return frameColor;
  }
  void setWidth(int neww) {
    w = neww;
  }
  void setHeight(int newh) {
    h = newh;
  }
  int getWidth() const {
    return w;
  }
  int getHeight() const {
    return h;
  }
};

Rectangle::Rectangle(Point center, int w, int h,
                     Fl_Color frameColor,
                     Fl_Color fillColor):
  center{center}, w{w}, h{h}, fillColor{fillColor}, frameColor{frameColor} {}
void Rectangle::draw() {
  array<Point, 5> points{
    Point{center.x-w/2, center.y-h/2},
    Point{center.x-w/2, center.y+h/2},
    Point{center.x+w/2, center.y+h/2},
    Point{center.x+w/2, center.y-h/2},
    Point{center.x-w/2, center.y-h/2}};
  fl_color(fillColor);
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
}
void Rectangle::setFillColor(Fl_Color newFillColor) {
  fillColor = newFillColor;
}
void Rectangle::setFrameColor(Fl_Color newFrameColor) {
  frameColor = newFrameColor;
}
bool Rectangle::contains(Point p) const  {
  return p.x>=center.x-w/2 &&
         p.x<center.x+w/2 &&
         p.y>=center.y-h/2 &&
         p.y<center.y+h/2;
}



/*--------------------------------------------------
Text class
Takes a string, position and draws it, can be modified
--------------------------------------------------*/
class Text: public virtual Printable {
  string s;
  Point center;
  int fontSize;
  Fl_Color color;
 public:
  //Constructor
  Text(string s, Point center, int fontSize = 10, Fl_Color color = FL_RED):
    s{s}, center{center}, fontSize{fontSize}, color{color} {}

  //Draw
  void draw();

  //Setters and getters
  string getString() {
    return s;
  }
  void setString(const string &newString) {
    s = newString;
  }
  int getFontSize() {
    return fontSize;
  }
  void setFontSize(int newFontSize) {
    fontSize = newFontSize;
  }
  Point getCenter() {
    return center;
  }
  void setCenter(Point newCenter) {
    center = newCenter;
  }
};
void Text::draw() {
  fl_color(color);
  fl_font(FL_HELVETICA, fontSize);
  int width{0}, height{0};
  fl_measure(s.c_str(), width, height, false);
  float x = fl_transform_x(center.x-width/2, center.y-fl_descent()+height/2);
  float y = fl_transform_y(center.x-width/2, center.y-fl_descent()+height/2);
  fl_draw(s.c_str(), x, y);
}

/*--------------------------------------------------
TextRectangle class
Inherits of Text and Rectangle to draws a text into a Rectangle
--------------------------------------------------*/
class TextRectangle: public Text, public Rectangle {
 public:
  TextRectangle(Point center, int w, int h, string text, int fontSize = 10)
      : Text{text, center, fontSize}, Rectangle{center, w, h} {}
  void draw() override {
    Rectangle::draw();
    Text::draw();
  }
  ~TextRectangle() override{}
};
