
boolean add_command_to_list(char*,byte, char*);
void commands_dispatcher(byte );
void commands_list_update();//
void commands_list_init();

void action_get_status(command*);

void action_get_second(command*);



int code_data(char* data, int len);
void code_and_print(char*, byte);
byte str_builder_long(char* main_buf, byte start_point, boolean param_symbol, long num);
byte str_builder_double(char* main_buf, byte start_point, boolean param_symbol, double num, byte rest_symbols_num);
