#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <MUIU8g2.h>
#include <Wire.h>
#include <Versatile_RotaryEncoder.h>
#include <time.h>

//Pins for OLED Display
#define SCL_PIN 22
#define SDA_PIN 21

//Pins for Rotary Encoder
#define ROT_CLK_PIN 5
#define ROT_DT_PIN 4
#define ROT_SW_PIN 2

//Data Structures
struct SensorReading 
{
  float humidity;
  float temperature;
  uint16_t light;
  uint8_t soilValue;
  time_t timestamp;
};

struct historical
{
  /* data */
};


Versatile_RotaryEncoder versatile_encoder(ROT_CLK_PIN, ROT_DT_PIN, ROT_SW_PIN);

U8G2_SSD1306_128X64_VCOMH0_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL_PIN, SDA_PIN);
MUIU8G2 mui;

//Device info
const char *wifi_network = "Wifi Network"; //Wifi Network
const char *device_name = "Test";
const char *serial_number = "123456789";
const char *version = "v0.1.0";
const uint8_t update_rate_list[] = {1, 2, 5, 10, 20, 30, 60};

//Device options
uint8_t units = 0; // 0 -> METRIC or 1-> IMPERIAL
uint8_t update_rate = 0;


uint8_t mui_hrule(mui_t *ui, uint8_t msg) {
  if ( msg == MUIF_MSG_DRAW ) {
      u8g2.drawHLine(0, mui_get_y(ui), u8g2.getDisplayWidth());
  }
  return 0;
}

uint8_t cycle_pages(mui_t *ui, uint8_t msg) {
  if ( msg == MUIF_MSG_DRAW ) {

  }
  return 0;
}

//Function to list current data
uint8_t show_current_data(mui_t *ui, uint8_t msg) {
  //checks if the display needs to be refreshed
  if ( msg == MUIF_MSG_DRAW ) {
    Serial.println(units);
    //Gets starting positon from fds_t
    u8g2_uint_t x = mui_get_x(ui);
    u8g2_uint_t y = mui_get_y(ui);
    u8g2.setCursor(x+5, y);
    u8g2.print("Humidity:");
    u8g2.setCursor(x+80, y);
    u8g2.print(humidity);
    u8g2.print("%");
    u8g2.setCursor(x+5, y+10);
    u8g2.print("Temperature:");
    u8g2.setCursor(x+80, y+10);
    if (units == 0) {
      u8g2.print(temperature);
      u8g2.print("C");
    } else if (units == 1){
      u8g2.print(roundf((temperature*1.8 + 32) * 100) / 100);
      u8g2.print("F");
    }
    u8g2.setCursor(x+5, y+20);
    u8g2.print("Light:");
    u8g2.setCursor(x+80, y+20);
    u8g2.print(light);
    u8g2.setCursor(x+5, y+30);
    u8g2.print("Soil:");
    u8g2.setCursor(x+80, y+30);
    u8g2.print(soilValue);
  }
  return 0;
}

uint8_t show_current_temp_data(mui_t *ui, uint8_t msg) {
  //checks if the display needs to be refreshed
  if ( msg == MUIF_MSG_DRAW ) {
    Serial.println(units);
    //Gets starting positon from fds_t
    u8g2_uint_t x = mui_get_x(ui);
    u8g2_uint_t y = mui_get_y(ui);
    u8g2.setCursor(x+5, y);
    u8g2.print("Humidity:");
    u8g2.setCursor(x+80, y);
    u8g2.print(humidity);
    u8g2.print("%");
    u8g2.setCursor(x+5, y+10);
    u8g2.print("Temperature:");
    u8g2.setCursor(x+80, y+10);
    if (units == 0) {
      u8g2.print(temperature);
      u8g2.print("C");
    } else if (units == 1){
      u8g2.print(roundf((temperature*1.8 + 32) * 100) / 100);
      u8g2.print("F");
    }
    u8g2.setCursor(x+5, y+20);
    u8g2.print("Light:");
    u8g2.setCursor(x+80, y+20);
    u8g2.print(light);
    u8g2.setCursor(x+5, y+30);
    u8g2.print("Soil:");
    u8g2.setCursor(x+80, y+30);
    u8g2.print(soilValue);
  }
  return 0;
}

//Function to list device info
uint8_t show_device_info(mui_t *ui, uint8_t msg) {
  //checks if the display needs to be refreshed
  if ( msg == MUIF_MSG_DRAW ) {
    //Gets starting positon from fds_t
    u8g2_uint_t x = mui_get_x(ui);
    u8g2_uint_t y = mui_get_y(ui);
    u8g2.setCursor(x+5, y);
    u8g2.print("Wifi:");
    u8g2.setCursor(x+50, y);
    u8g2.print(wifi_network);
    u8g2.setCursor(x+5, y+10);
    u8g2.print("Name:");
    u8g2.setCursor(x+50, y+10);
    u8g2.print(device_name);
    u8g2.setCursor(x+5, y+20);
    u8g2.print("SN:");
    u8g2.setCursor(x+50, y+20);
    u8g2.print(serial_number);
    u8g2.setCursor(x+5, y+30);
    u8g2.print("Ver:");
    u8g2.setCursor(x+50, y+30);
    u8g2.print(version);
  }
  return 0;
}

uint8_t show_my_data(mui_t *ui, uint8_t msg) {
  if ( msg == MUIF_MSG_DRAW ) {
    u8g2_uint_t x = mui_get_x(ui);
    u8g2_uint_t y = mui_get_y(ui);
    u8g2.setCursor(x+5, y);
    u8g2.print("Wifi:");
    u8g2.setCursor(x+50, y);
    u8g2.print(wifi_network);
  }
  return 0;
}

muif_t muif_list[] = {
  MUIF_U8G2_FONT_STYLE(0, u8g2_font_resoledmedium_tr),        /* regular font */
  MUIF_U8G2_FONT_STYLE(1, u8g2_font_resoledbold_tr),        /* bold font */
  MUIF_U8G2_FONT_STYLE(2, u8g2_font_8x13_t_symbols),

  MUIF_RO("HR", mui_hrule),
  MUIF_U8G2_LABEL(),
  
  MUIF_RO("GP",mui_u8g2_goto_data),  
  MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),

  MUIF_VARIABLE("UN",&units,mui_u8g2_u8_opt_line_wa_mud_pi),
  MUIF_VARIABLE("UR",&update_rate,mui_u8g2_u8_opt_line_wa_mud_pi),
  
  /* register custom function to show the data */
  MUIF_RO("SH", show_my_data), 
  MUIF_RO("SD", show_device_info),
  MUIF_RO("CD", show_current_data),

  /* a button for the menu... */
  //MUIF_BUTTON("GO", mui_u8g2_btn_goto_wm_fi)  
  MUIF_EXECUTE_ON_SELECT_BUTTON("GO", mui_u8g2_btn_goto_wm_fi)  
};


fds_t fds_data[] = 

//Main Menu
MUI_FORM(1)
MUI_STYLE(1)
MUI_LABEL(5, 7, "Garden Monitor")
MUI_STYLE(0)
MUI_XY("HR", 0,11)
MUI_DATA("GP", 
    MUI_10 "Enter Data|"
    MUI_20 "Current Data|"
    MUI_30 "Settings|"
    MUI_40 "History|"
    MUI_50 "Device Info"
    )
MUI_XYA("GC", 5, 20, 0) 
MUI_XYA("GC", 5, 30, 1) 
MUI_XYA("GC", 5, 40, 2) 
MUI_XYA("GC", 5, 50, 3) 
MUI_XYA("GC", 5, 60, 4) 

//Current Data
MUI_FORM(20)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Current Data")
MUI_XY("HR", 0,11)
MUI_STYLE(0)
MUI_XY("CD", 0, 23)
MUI_XYAT("GO", 114, 60, 1, " Ok ") 

//ENTER DATA
MUI_FORM(10)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Show Data")
MUI_XY("HR", 0,11)
MUI_STYLE(0)
MUI_XY("SH", 0, 23)
MUI_XYAT("GO", 114, 60, 1, " Ok ") 

//Settings
MUI_FORM(30)
MUI_STYLE(1)
MUI_LABEL(5, 8, "Settings")
MUI_XY("HR", 0,11)
MUI_STYLE(0)
MUI_LABEL(5, 22, "Units:")
MUI_XYAT("UN", 70, 22, 50, "Metric|Imperial")
MUI_LABEL(5, 33, "Refresh:")
MUI_XYAT("UR", 70, 33, 50, "1min|2min|5min|10min|20min|30min|60min")

MUI_XYAT("GO", 114, 60, 1, " Ok ") 

//History
MUI_FORM(40)
MUI_STYLE(1)
MUI_LABEL(5, 8, "History")
MUI_XY("HR", 0,11)
MUI_STYLE(0)
MUI_XY("SH", 0, 23)
MUI_STYLE(2)
MUI_XYAT("GO", 21, 60, 1, " ↰ ") 
MUI_XYAT("GO", 59, 60, 1, "  ←  ") 
MUI_XYAT("GO", 105, 60, 1, "  →  ") 

//Device info
MUI_FORM(50)
MUI_STYLE(1)
MUI_LABEL(5, 7, "Device Info")
MUI_XY("HR", 0,10)
MUI_STYLE(0)
MUI_XY("SD", 0, 23)
MUI_XYAT("GO", 114, 60, 1, " Ok ") 



;

// global variables for menu redraw and input event handling
uint8_t is_redraw = 1;
uint8_t rotate_event = 0; // 0 = not turning, 1 = CW, 2 = CCW
uint8_t press_event = 0; // 0 = not pushed, 1 = pushed
uint8_t long_press_event = 0; // 0 = not pushed, 1 = pushed


// Functions prototyping to be handled on each Encoder Event
void handleRotate(int8_t rotation) {
  if ( rotation > 0 )
    rotate_event = 2; // CW
  else
    rotate_event = 1; // CCW
}

void handlePressRelease() {
  press_event = 1;
}

void handleLongPressRelease() {
  long_press_event = 1;
}


void setup(void) {

  // Load to the encoder all nedded handle functions here (up to 9 functions)
  versatile_encoder.setHandleRotate(handleRotate);
  versatile_encoder.setHandlePressRelease(handlePressRelease);
  versatile_encoder.setHandleLongPressRelease(handleLongPressRelease);
  Serial.begin(9600);
  u8g2.begin();
  mui.begin(u8g2, fds_data, muif_list, sizeof(muif_list)/sizeof(muif_t));
  mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);
}

void read_sensors() {
}

void handle_events(void) {
  // 0 = not pushed, 1 = pushed  
  if ( press_event == 1 ) {
    mui.sendSelect();
    is_redraw = 1;
    press_event = 0;
  }

  // 0 = not pushed, 1 = pushed  
  if ( long_press_event == 1 ) {
    mui.sendSelectWithExecuteOnSelectFieldSearch();
    is_redraw = 1;
    long_press_event = 0;
  }
  
  // 0 = not turning, 1 = CW, 2 = CCW
  if ( rotate_event == 1 ) {
    mui.nextField();
    is_redraw = 1;
    rotate_event = 0;
  }
  
  if ( rotate_event == 2 ) {
    mui.prevField();
    is_redraw = 1;
    rotate_event = 0;
  }    
}



void loop(void) {

  /* check whether the menu is active */
  if ( mui.isFormActive() ) {

    /* update the display content, if the redraw flag is set */
    if ( is_redraw ) {
      u8g2.firstPage();
      do {
          versatile_encoder.ReadEncoder(); // Do the encoder reading and processing
          mui.draw();
          versatile_encoder.ReadEncoder(); // Do the encoder reading and processing
      } while( u8g2.nextPage() );
      is_redraw = 0;                    /* clear the redraw flag */
    }

    versatile_encoder.ReadEncoder(); // Do the encoder reading and processing
    handle_events();
    read_sensors();
      
  } else {
      /* the menu should never become inactive, but if so, then restart the menu system */
      mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);
  }
}