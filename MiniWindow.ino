#include <lvgl.h>
#include <TFT_eSPI.h>
/*If you want to use the LVGL examples,
  make sure to install the lv_examples Arduino library
  and uncomment the following line.
#include <lv_examples.h>
*/
 
#include <demos/lv_demos.h>
 
/*Change to your screen resolution*/
static const uint16_t screenWidth  = 160;
static const uint16_t screenHeight = 128;
 
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * 10 ];
 
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
 
#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif
 
/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );
 
    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();
 
    lv_disp_flush_ready( disp );
}
 
 
void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
 
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
 
    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );
 
    lv_init();
 
#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif
 
    tft.begin();          /* TFT init */
    tft.setRotation( 3 ); /* Landscape orientation, flipped */
 
 
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );
 
    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );
 
 
#if 0
    /* Create simple label */
    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, LVGL_Arduino.c_str() );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
#else
    /* Try an example from the lv_examples Arduino library
       make sure to include it as written above.
    lv_example_btn_1();
   */
 
    // uncomment one of these demos
    // lv_demo_widgets();            // OK
     lv_demo_benchmark();          // OK
    // lv_demo_keypad_encoder();     // works, but I haven't an encoder
    // lv_demo_music();              // NOK
    // lv_demo_printer();
    // lv_demo_stress();             // seems to be OK
#endif
    Serial.println( "Setup done" );
}
 
void loop()
{
    lv_timer_handler(); /* let the GUI do its work */
    delay( 5 );
}