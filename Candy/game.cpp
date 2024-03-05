#include "tools.h"
#include "textRectangle.cpp"
#include "sketchable.cpp"
/*
Date : 22/08/2022
Author : Delaude Julien
*/
/*--------------------------------------------------
Board class
This is our Model 
Takes vector of vector of ClickableCell and a vector of possibles moves
Can call on every method from ClickableCell and modify our Sketchable
Check integrity of the board and implement the logical point of view as crushCandy, setMatch and reSeason
--------------------------------------------------*/
class Board {
  int score=0;
  bool canFill;
  int inaction=0;
  vector<Move> possibles_moves;
  vector< vector< ClickableCell> > board;
 public:
  static const int rows = 9;
  static const int columns = 9;
  Board() {
    newGame();
  }
  ClickableCell getSquare(int row, int column) const {
    return board[row][column];
  }
 
  //To print score
  string getScore(){
    return to_string(score);
  }
  auto getPossibles_moves(){
    searchMoves();
    return possibles_moves;
  }
  
  //Check horizontaly if theres blank between candys
  bool checkIsFalling(){
    for (int x=0; x<Board::rows;x++){
      for (int y=1;y<Board::columns;y++){
        if (board[x][y].getSketchable() == CandyFlavor::Null && board[x][y-1].getSketchable()!=CandyFlavor::Null){
          return true;
        }
      }
    }
    return false;
  }
 
  //Search possibles moves, return impossible if empty
  bool isImpossible(){
    searchMoves();
    if (possibles_moves.empty()){return true;}
    else {return false;}
  }
 

  //increment inactivity counter
  void setInaction(bool inc){
    if (inc){inaction++;}
    else {inaction=0;}
  }
  
  //Return suggested move after inactivity time 
  Move getSuggest(){
    srand(time(0));
    auto v = getPossibles_moves();
    return v[rand()%v.size()];
  }
  
  //Hightlight a random move if inactivity
  void hightlight(Move tosuggest){
    Point source = tosuggest.t;
    Point dest = tosuggest.p;
    ClickableCell c1 = board[source.x][source.y];
    ClickableCell c2 =board[dest.x][dest.y];
    c1.setFrameColor(FL_YELLOW);
    c2.setFrameColor(FL_YELLOW);
    c1.draw();
    c2.draw();
    c1.setFrameColor(FL_BLACK);
    c2.setFrameColor(FL_BLACK);
    setInaction(false);
    }
  
  //Return true if inactivity counter > 10 sec
  bool isInactive(){
    return inaction==refreshPerSecond*10;
  }
  
  
  //Simulate from every position on board every move available et which lead to a 3-match
  void searchMoves(){
    possibles_moves.clear();
    vector<Point> dir= {{1,0},{-1,0},{0,1},{0,-1}};
    for (int x = 0; x<Board::rows;x++){
      for (int y =0; y<Board::columns;y++){
        for(int j = 0;j<4;j++){
          CandyFlavor flav = board[x][y].getSketchable();
          Point alpha = {x,y};Point beta = {x+dir[j].x,y+dir[j].y};
          if (isInBoard(beta)){
            Move move{alpha,beta};
            CandyFlavor otherflav =board[beta.x][beta.y].getSketchable();
            board[x][y].setFlavor(otherflav);
           if(isAlign_x(beta,flav) ==true){possibles_moves.push_back(move);possibles_moves.push_back({beta,alpha});}
           if(isAlign_y(beta,flav) ==true){possibles_moves.push_back(move);possibles_moves.push_back({beta,alpha});}
          }
          board[x][y].setFlavor(flav);
        }
      }
    }
  }
  bool isInBoard(Point p){
    if(p.x>= 0 && p.x <Board::rows){
      if(p.y>=0 && p.y<Board::columns){
        return true;
      }
      return false;
    }
    return false;
  }
 

  //Check if a given move from controllBoard is valid, if isnt dont swap
  bool isValidMove(Move p){
    vector<Move> v = getPossibles_moves();
    for (long unsigned int j=0;j<v.size();j++){
      Move m = v[j];
      Point source = m.t;
      Point dest = m.p;
      if (source.x == p.t.x && source.y == p.t.y && dest.x== p.p.x && dest.y == p.p.y){return true;}
    }
    cout<<"Move not Possible..."<<endl;
    return false;
  }
  
  //Check for 3-match in both direction
  bool isAlign_x(Point p, CandyFlavor flav){
    int x = p.x;
    int y = p.y;
    if (x==0){
      if (board[x+1][y].getSketchable()== flav && board[x+2][y].getSketchable() == flav){return true;}
    }
    if(x==1){
      if (board[x-1][y].getSketchable() == flav && board[x+1][y].getSketchable()==flav){return true;}
      if (board[x+1][y].getSketchable()== flav && board[x+2][y].getSketchable() == flav){return true;}
    }
    if (x==Board::columns-2){
      if (board[x-1][y].getSketchable()== flav && board[x+1][y].getSketchable() == flav){return true;}
      if (board[x-1][y].getSketchable()== flav && board[x-2][y].getSketchable() == flav){return true;}
    }
    if (x==Board::columns-1){
      if(board[x-1][y].getSketchable()==flav && board[x-2][y].getSketchable() == flav){return true;}
    }
    else{
      if(x>1 && x<Board::columns-2){
        if(board[x-1][y].getSketchable()==flav && board[x-2][y].getSketchable() == flav){return true;}
        if (board[x+1][y].getSketchable()== flav && board[x+2][y].getSketchable() == flav){return true;}
        if (board[x-1][y].getSketchable()== flav && board[x+1][y].getSketchable() == flav){return true;}
      }
    }
    return false;
  }
  bool isAlign_y(Point p, CandyFlavor flav){
    int x = p.x;
    int y = p.y;
    if (y==0){
      if (board[x][y+1].getSketchable()== flav && board[x][y+2].getSketchable() == flav){return true;}
    }
    if(y==1){
      if (board[x][y-1].getSketchable() == flav && board[x][y+1].getSketchable()==flav){return true;}
      if (board[x][y+1].getSketchable()== flav && board[x][y+2].getSketchable() == flav){return true;}
    }
    if (y==Board::columns-2){
      if (board[x][y-1].getSketchable()== flav && board[x][y+1].getSketchable() == flav){return true;}
      if (board[x][y-1].getSketchable()== flav && board[x][y-2].getSketchable() == flav){return true;}
    }
    if (y==Board::columns-1){
      if(board[x][y-1].getSketchable()==flav && board[x][y-2].getSketchable() == flav){return true;}
    }
    else{
      if(y>1 && y<Board::columns-2){
        if(board[x][y-1].getSketchable()==flav && board[x][y-2].getSketchable() == flav){return true;}
        if (board[x][y+1].getSketchable()== flav && board[x][y+2].getSketchable() == flav){return true;}
        if (board[x][y-1].getSketchable()== flav && board[x][y+1].getSketchable() == flav){return true;}
      }
    }
    return false;
  }
  
  //Goes twice, one horizontal the other one vertical throught the board to initialize 3-match
  void setMatch(){
    for (int x = 0; x<Board::rows;x++){
      for (int y =0; y<Board::columns-2;y++){
        CandyFlavor flav = board[x][y].getSketchable();
        if (board[x][y+1].getSketchable()== flav && board[x][y+2].getSketchable() == flav){
          for (int n=0;n<=2;n++){board[x][y+n].isMatch(true);}
        }
      }
    }
    for (int x = 0; x<Board::rows-2;x++){
      for (int y =0; y<Board::columns;y++){
        CandyFlavor flav = board[x][y].getSketchable();
        if (board[x+1][y].getSketchable()== flav && board[x+2][y].getSketchable() == flav){
          for (int n=0;n<=2;n++){board[x+n][y].isMatch(true);}
        }
      } 
    }
  }
  
  //Crush if no candy are falling then proceed to destruct every marked candy, if some candy's must fall first then dropCandy()
  void crushCandy(){
    if(!checkIsFalling()){
      canFill=true;
      setMatch();
      for (int x=0; x<Board::rows;x++){
        for (int y=0;y<Board::columns;y++){
          if (board[x][y].getMatch() == true){
            score +=10;
            //Set Logical flavor to Null and undo match
            CandyFlavor flav = CandyFlavor::Null;
            board[x][y].setFlavor(flav);
            board[x][y].isMatch(false);
            //Animate Destruction
            board[x][y].toDestroy(true);
            board[x][y].draw();
          }
          }
        }
    }
    dropCandy();
  }
  

  //Check if Candy is not Animating
  bool canUpdate(){
    for (int x =0; x<Board::rows;x++){
      for (int y =0; y<Board::columns;y++){
        auto cell = getSquare(x,y);
        if (cell.getState() ==1){return false;}
      }
    }
    return true;
  }

  //Drop Candy until out of blank between candys, start at y=max-1 and uses swap to simulate falling
  void dropCandy(){
    for (int x =0; x<Board::columns;x++){
      for (int y = Board::columns-1;y>=0;y--){
        if(board[x][y].isDestroyed()==true){board[x][y].toDestroy(false);}
        if (board[x][y].getSketchable() == CandyFlavor::Null && canUpdate()==true ){
          int tmp = y-1;
          while (tmp>=0 && board[x][tmp].getSketchable() != CandyFlavor::Null){
              if(board[x][tmp].getSketchable() != CandyFlavor::Null ){
                canFill = false;
                auto cell = board[x][tmp];
                auto cell2 = board[x][y];
                cell.mouseClick({x,tmp},{x,y});
                cell2.mouseClick({x,y},{x,tmp});
                swap({x,tmp},{x,y});
                break;
              }
            tmp--;
          }
        }
      }
    }
  }
  
  //If none is Falling & canfillup, pick a random flavor then put it in the 1 row then proceed to fall 
  void reSeason(){
    srand(time(0));
    if (!checkIsFalling() && canFill){
      for (int x =0; x<Board::rows;x++){
        if (board[x][0].getSketchable() == CandyFlavor::Null){
          CandyFlavor nflav = RandomCandy(rand()%6 +1);
          board[x][0].setFlavor(nflav);
        }
      }
    }
  }

  //Simple Swap
  void swap(Point p1,Point p2){
    std::swap(board[p1.x][p1.y], board[p2.x][p2.y]);
  }
  void newGame() {
    srand(time(0));
    score = 0;
    board.clear();
    for (int x = 0; x<rows; x++) {
      board.push_back({});
    for (int y = 0; y<columns; y++){
      CandyFlavor random = RandomCandy((rand()%6)+1);
      board.at(x).emplace_back(make_shared<Swap>(make_shared<Candy>(Point{50*x+50, 50*y+100}, 50, 50,random)));
    }
  }
  }
};


/*--------------------------------------------------
DispalyBoard class.
Main Loop of the interativ project
Draws refreshpersecond times per second and check for GameOver 
We use few booleans to prevent animating and crushing at the same time
--------------------------------------------------*/
class DisplayBoard {
  shared_ptr<Board> board; 
  int textSize=18;
  Fl_PNG_Image *background = new Fl_PNG_Image("images/Background.png");
 public:
  DisplayBoard(shared_ptr<Board> board): board{board} {};
  void draw() const {
    background -> draw(0,50,1200,720);
    fl_draw_box(FL_FLAT_BOX,10,65,Board::rows*50+30,Board::columns*50+30,FL_BLACK);
    bool up = board->canUpdate(); // get Animation State 
    for (int x =0; x<Board::rows;x++){
      for (int y =0; y<Board::columns;y++){
        auto cell = board->getSquare(x,y);
        cell.draw();
    }
    }
    if (up) {board->crushCandy();}
    if (up) {board->reSeason();}
    unique_ptr<Printable> scoreboard = make_unique<TextRectangle>(Point{600,102},220,75, "Actual Score is : "+board->getScore()+" pts",textSize); 
    scoreboard->draw();
    if (stoi(board->getScore())>=Board::rows*Board::columns*20 || board->isImpossible()){ // If no move are available or goal is done
      unique_ptr<Printable> GameEnds = make_unique<TextRectangle>(Point{500,500},400,200, "Game Is Done you've collected : "+board->getScore(),20);
      GameEnds->draw();
      if (stoi(board->getScore())>=350){cout<<"Game ends, You've succeded"<<endl;}
      else{cout<<"Game ends, No moremove available. Please reload"<<endl;}
      sleep(10);
      exit(1);
      //board->newGame();//Causes Bug although this is the exact same as the 1st initialize
    }
    if (up){board->setInaction(true);} //If user can play but dont, increase inactivity timer
    if(!up){board->setInaction(false);} //If user or proccess dropCandy or CrushCandy then reset timer
    if(board->isInactive()){ // If user is not playing for over 10sec, then pop up a random move 
      cout<<"Inactivity"<<endl;
      Move m= board->getSuggest();
      board->hightlight(m);
    }
  }    
};

/*--------------------------------------------------
ControllBoard class.
Process Event, needs 2 clicks to work, next to each other and can proceed without having to match
Responsible for the animation start and uses 2 stored positions to swap
--------------------------------------------------*/

class ControllBoard {
  shared_ptr<Board> board;
  int click=0;
  Point p1;
  Point p2;
 public:
  ControllBoard(shared_ptr<Board> board): board{board} {};
  bool processEvent(const int event) {//User has to click in both candy to perform swap
    switch(event) {
      case FL_PUSH: {
        click++;
        p1.x = static_cast<int>(Fl::event_x()-25)/50;//Convert in int
        p1.y = static_cast<int>(Fl::event_y()-75)/50;
        if(p1.x>=0 && p1.x < Board::columns && click ==1){
          p2.x = p1.x;
          p2.y = p1.y;
          break;
        }
        if (click==2)
          if (abs(p2.x-p1.x)+abs(p2.y-p1.y)==1 && board->canUpdate()==true && board->isValidMove({p2,p1})){ //If the board is actually drawing falling candys then donc proceed to swap
            cout<<p2.x<<","<<p2.y<<"=>"<<p1.x<<","<<p1.y<<endl;
            auto cell1 = board->getSquare(p2.x,p2.y);
            auto cell2 = board->getSquare(p1.x,p1.y);
            cell1.mouseClick(p2,p1);
            cell2.mouseClick(p1,p2);
            std::swap(cell1,cell2);
            board->swap(p2,p1);
            click=0;
            return true;
          }click = 0; break; 
        }
      case FL_KEYDOWN:
        switch(Fl::event_key()){
          case ' ':
           board->newGame();//Causes bug 
           return true;
          case 'q':
            exit(0);
        }
    }
    return false;
  }
};


/*--------------------------------------------------
MainWindow class
Shows windows, start MVC, model-view-controller and reset
Handles the controller
--------------------------------------------------*/
class MainWindow : public Fl_Window {
  shared_ptr<Board> board;
  DisplayBoard displayBoard;
  ControllBoard controllBoard;
 public:
  MainWindow()
      :Fl_Window(500, 500, windowWidth, windowHeight, "CandyCrush"),
       board{make_shared<Board>()},
       displayBoard(board),
       controllBoard(board) {
    Fl::add_timeout(1.0/refreshPerSecond, Timer_CB, this);
    Fl::visual(FL_RGB);
    resizable(this);
  }
  void draw() override {
    Fl_Window::draw();
    displayBoard.draw();
  }
  int handle(int event) override {
    return controllBoard.processEvent(event);
  }
  static void Timer_CB(void *userdata) {
    MainWindow *o = (MainWindow*) userdata;
    o->redraw();
    Fl::repeat_timeout(1.0/refreshPerSecond, Timer_CB, userdata);
  }
};

int main(int argc, char *argv[]) {
  MainWindow window;
  window.show(argc, argv);
  return Fl::run();
}
