void heli_setup();
void heli_loop();

void read_button();
void set_game_mode(int mode);
void game_home(boolean update);
void game_play(boolean update);
void game_over(boolean update);
unsigned long get_highscore();
void set_highscore(unsigned long score);