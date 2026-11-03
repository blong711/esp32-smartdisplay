#include <Arduino.h>
#include <esp32_smartdisplay.h>

static int click_count = 0;
static lv_obj_t *label_count = nullptr;

static void btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        click_count++;
        if (label_count)
        {
            char buf[32];
            snprintf(buf, sizeof(buf), "Clicks: %d", click_count);
            lv_label_set_text(label_count, buf);
        }

        // Đổi màu LED RGB khi nhấn nút (Xanh lá -> Xanh dương -> Đỏ)
        bool r = (click_count % 3 == 0);
        bool g = (click_count % 3 == 1);
        bool b = (click_count % 3 == 2);
        smartdisplay_led_set_rgb(r, g, b);
        Serial.printf("Button clicked! Count: %d\n", click_count);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Starting ESP32-3248S035C Demo...");

    // Khởi tạo SmartDisplay (Màn hình ST7796 + Cảm ứng GT911)
    smartdisplay_init();

    // Đặt độ sáng màn hình tối đa
    smartdisplay_lcd_set_backlight(1.0f);

    // Bật đèn LED ban đầu (Xanh lá)
    smartdisplay_led_set_rgb(false, true, false);

    // Màn hình chính
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x181825), LV_PART_MAIN);

    // Tiêu đề
    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "ESP32-3248S035C");
    lv_obj_set_style_text_color(title, lv_color_hex(0x89B4FA), LV_PART_MAIN);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);

    // Thông tin phụ
    lv_obj_t *sub = lv_label_create(scr);
    lv_label_set_text(sub, "ST7796 320x480 | GT911 Capacitive");
    lv_obj_set_style_text_color(sub, lv_color_hex(0xA6ADC8), LV_PART_MAIN);
    lv_obj_align(sub, LV_ALIGN_TOP_MID, 0, 65);

    // Nút bấm kiểm tra cảm ứng
    lv_obj_t *btn = lv_button_create(scr);
    lv_obj_set_size(btn, 220, 60);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 10);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xA6E3A1), LV_PART_MAIN);

    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Touch Me!");
    lv_obj_set_style_text_color(btn_label, lv_color_hex(0x11111B), LV_PART_MAIN);
    lv_obj_center(btn_label);

    // Nhãn đếm số lượt click
    label_count = lv_label_create(scr);
    lv_label_set_text(label_count, "Clicks: 0");
    lv_obj_set_style_text_color(label_count, lv_color_hex(0xF9E2AF), LV_PART_MAIN);
    lv_obj_align(label_count, LV_ALIGN_CENTER, 0, 90);

    Serial.println("UI setup complete.");
}

void loop()
{
    lv_timer_handler();
    delay(5);
}
