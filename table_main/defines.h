// connecting libraries
#include <Wire.h>
#include <SPI.h>
#include <NecDecoder.h>
#include <microLED.h>  
#include <FastLED.h>
#include <EEPROM.h>

#define COLOR_DEBTH 2


#define STRIP_PIN 7     
   
#define TABLE_HEIGHT 8 
#define TABLE_WIDTH 11  

#define code_info 0
#define set_default_settings_EEPROM 0

#define DEBUGMODE 1
#define mode_of_work 0 // 1 -- testing, 0 -- main mode
#define decode_buffer_size 48 
#define input_buffer_size 48 
#define command_number_size 3
#define arguments_str_size 12
#define command_list_size 10
#define command_number_separator '|'
#define arguments_separator '&'
#define double_separator '.'


#define LINE1_CH_1 0xA2
#define LINE1_CH_2 0x62
#define LINE1_CH_3 0xE2
#define LINE2_MOVE_1 0x22
#define LINE2_MOVE_2 0x2
#define LINE2_MOVE_3 0xC2
#define LINE3_1 0xE0
#define LINE3_2 0xA8
#define LINE3_3 0x90
#define LINE4_1 0x68
#define LINE4_2 0x98
#define LINE4_3 0xB0


#define REMOTE_CONTROLL_STEP 5
#define DECREASE false
#define INCREASE true
#define START_STATUS 0
#define WORK_STATUS 1
#define PARAMETERS_CHANGED_STATUS 2


#define MIN_BRIGHTNESS 30
#define MAX_BRIGHTNESS 215

#define START_MODE_OF_TABLE 2


NecDecoder ir;
NecDecoder ir2;
microLED<TABLE_HEIGHT * TABLE_WIDTH, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

#ifdef DEBUGMODE  
    unsigned long count_circles = 0;
    unsigned long count_frames = 0;
    unsigned long lastTimeDebug = 0;
#endif

struct command { 
  char* request_number; 
  byte embedded_command_number;
  char* arguments_str;
};

struct effect_info { 
  byte effect_num; 

  byte color;
  byte saturation;
  byte brightnes;
  byte effect_speed;
  byte effect_scale;
};
effect_info currentMode;
boolean remote_controler_changes = false;
                                     //  --0---1---2---3---4---5---6---7---8--
byte tranzistors_line [TABLE_HEIGHT]    = {47, 46, 49, 48, 51, 50, 53, 52};  // pins for connecting transistors responsible for a specific line

                                     // --0--1--2--3--4--5--6--7--8--9--10--11--12--
byte ports_vertical_lines[TABLE_WIDTH] = {33,30,31,28,29,26,27,24,25,22,23}; // pins for reading vertical values




boolean buttons_matrix_status[TABLE_HEIGHT][TABLE_WIDTH];
byte current_line = 0;

// interrupts for listening port
volatile byte interrupts_count = 0;
volatile boolean is_command_execution_allowed = 1;


// comands
byte command_lenth = 0;
command* command_list[command_list_size];
char input_string_buffer[decode_buffer_size];

char* list_of_all_commands[]={
    "get_status",                       // ----- 0
    "get_second"
};

char error_command[] = "uncorrect_input!";
char normal_work_command[] = ">everything is ok!<";
char overflow_command[] = ">overflow<";
