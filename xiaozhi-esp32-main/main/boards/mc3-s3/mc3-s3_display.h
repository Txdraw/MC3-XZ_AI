#ifndef MC3_S3_DISPLAY_H
#define MC3_S3_DISPLAY_H

#include "display/lcd_display.h"
#include "application.h"
#include "device_state.h"
#include <esp_timer.h>
#include <time.h>
#include <esp_lvgl_port.h>

extern "C" {
#include "boards/mc3-s3/ui/ui.h"
}

class MC3_S3_SpiLcdDisplay : public SpiLcdDisplay {
private:
    bool custom_ui_loaded_ = false;
    lv_obj_t* original_screen_ = nullptr;
    esp_timer_handle_t time_update_timer_ = nullptr;

    static void time_update_callback(void* arg) {
        MC3_S3_SpiLcdDisplay* display = static_cast<MC3_S3_SpiLcdDisplay*>(arg);
        display->UpdateTime();
    }

    void UpdateTime() {
        if (!custom_ui_loaded_) return;
        
        time_t now = time(nullptr);
        if (now == (time_t)-1) return;
        
        struct tm timeinfo;
        if (localtime_r(&now, &timeinfo) == nullptr) return;
        
        lvgl_port_lock(0);
        lv_label_set_text_fmt(ui_Label1, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
        lvgl_port_unlock();
    }

public:
    MC3_S3_SpiLcdDisplay(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_handle_t panel,
                         int width, int height, int offset_x, int offset_y,
                         bool mirror_x, bool mirror_y, bool swap_xy)
        : SpiLcdDisplay(panel_io, panel, width, height, offset_x, offset_y, mirror_x, mirror_y, swap_xy) {}

    virtual void SetStatus(const char* status) override {
        DeviceState state = Application::GetInstance().GetDeviceState();
        if (state == kDeviceStateIdle) {
            if (!custom_ui_loaded_) {
                original_screen_ = lv_screen_active();
                ui_init();
                lv_disp_load_scr(ui_Screen1);
                custom_ui_loaded_ = true;
                
                esp_timer_create_args_t timer_args = {
                    .callback = &time_update_callback,
                    .arg = this,
                    .name = "time_update_timer"
                };
                esp_timer_create(&timer_args, &time_update_timer_);
                esp_timer_start_periodic(time_update_timer_, 1000000); // 1 second
            }
        } else {
            if (custom_ui_loaded_) {
                if (original_screen_) {
                    lv_screen_load(original_screen_);
                }
                if (time_update_timer_) {
                    esp_timer_stop(time_update_timer_);
                    esp_timer_delete(time_update_timer_);
                    time_update_timer_ = nullptr;
                }
                custom_ui_loaded_ = false;
            }
            SpiLcdDisplay::SetStatus(status);
        }
    }
};

#endif // MC3_S3_DISPLAY_H
