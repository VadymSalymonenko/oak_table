
// ------------FADING_TOUCH---------------
#define FT_MAX_BRIGHTNESS 200
#define FT_MAX_BRIGHTNESS_LOW 120
#define FT_MIN_BRIGHTNESS 30
#define FT_DECREASING_MAX_STEP 10
#define FT_MIN_SPEED 0
#define TOTAL_SHINE_TIME 65000
#define LUKASHENKO_LOOSE_LIM 10
byte decreasing_step = FT_DECREASING_MAX_STEP;


enum {
  ONE_COLOR_1,
  MULTICOLOR_1,
  SEPARATE_COLORS_1
};
// ------------FULL_SCREEN_NOISE---------------
#define COLOR_PALETTE_SIZE 50  // 1 element = 3 bite;

#define FSN_COUNTER_STEP_MAX 10
#define FSN_COUNTER_STEP_MIN 1
#define FSN_CHANGED_DELAY 1500

// ------------ONE_COLOR_NOISE---------------
#define OCN_COLOR_RANGE 10
#define OCN_MAX_BRIGHTNESS 215
#define OCN_MAX_BRIGHTNESS_LOW 100
#define OCN_MIN_BRIGHTNESS 80
// ------------MULTICOLOR_NOISE---------------
#define MN_MAX_BRIGHTNESS 215
#define MN_MAX_BRIGHTNESS_LOW 100
#define MN_MIN_BRIGHTNESS 80
#define MN_MIN_COLOR_RANGE 10
// ------------separate_colors_noise---------------
#define SCN_MAX_BRIGHTNESS 215
#define SCN_MAX_BRIGHTNESS_LOW 100
#define SCN_COLORS_NUM 4

mData* color_palette;
int counter = 0;



int effect_general_status = 0;

int cell_light_status[TABLE_HEIGHT][TABLE_WIDTH];
long newcell_timestamp, add_colour_timestamp = 0;

void fading_touch();
void drawing_mode();
void one_color_noise();
void multicolor_noise();
void separate_colors_noise();
void tap_game_1_mode();
