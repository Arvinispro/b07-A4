module validate;
void getstate(){
  return this -> state;
}
void validate(){
  if (legal){
    this->state = Point{"e1", "e3"} //the validated move
    notifyobs(); //run each obs' notify(), which asks for Validate::getstate() to apply moves
  }
  else{
    state = Point{"z0", "z0"}  //represent error msg
    Observer *sys = getSys(); //only notify system to redo the move for the same color
    sys -> notify();
  }
}