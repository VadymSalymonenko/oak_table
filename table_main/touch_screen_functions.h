void clearStrip();
void readLine(int line, int nextLine);
void printMatrix();
void set_strip_cell(byte x, byte y, mData led_info);
void strip_show();
void clear_mode_switching();

void mods_dispatcher();

void init_start_mode();
void set_default_mods_parameters();
void update_default_mods_parameters();

void change_curr_mode_left();
void change_curr_mode_right();
void set_new_curr_mode(byte mod_num);
void change_value(boolean action,byte min1,byte max1,byte step1, byte* val,boolean border);
