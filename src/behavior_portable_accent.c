/*
 * Portable Portuguese accents.
 *
 * HID does not have portable Unicode characters. Profiles 0/1 use Linux's
 * Ctrl+Shift+U entry; profiles 3/4 use Windows' legacy Alt+numpad entry;
 * profile 2 deliberately uses the Linux sequence as best-effort Android
 * support, because Android has no reliable standard HID Unicode protocol.
 */
#define DT_DRV_COMPAT zmk_behavior_portable_accent

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <dt-bindings/zmk/keys.h>
#include <dt-bindings/zmk/modifiers.h>
#include <zmk/behavior.h>
#include <zmk/behavior_queue.h>
#include <zmk/ble.h>
#include <zmk/hid.h>

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

enum accent { ACUTE_A = 1, ACUTE_E, ACUTE_I, ACUTE_O, ACUTE_U, TILDE_A, TILDE_O,
              CEDILLA_C, GRAVE_A, CIRC_A, CIRC_E, CIRC_O };

static const uint16_t lower[] = {0, 0x00e1, 0x00e9, 0x00ed, 0x00f3, 0x00fa, 0x00e3,
                                 0x00f5, 0x00e7, 0x00e0, 0x00e2, 0x00ea, 0x00f4};
static const uint16_t upper[] = {0, 0x00c1, 0x00c9, 0x00cd, 0x00d3, 0x00da, 0x00c3,
                                 0x00d5, 0x00c7, 0x00c0, 0x00c2, 0x00ca, 0x00d4};
static const struct zmk_behavior_binding kp = {.behavior_dev = DEVICE_DT_NAME(DT_NODELABEL(kp))};
static const struct zmk_behavior_binding self = {.behavior_dev = DEVICE_DT_NAME(DT_INST(0, DT_DRV_COMPAT))};

static void q(struct zmk_behavior_binding_event *e, uint32_t key, bool down, uint32_t wait) {
    struct zmk_behavior_binding b = kp; b.param1 = key;
    zmk_behavior_queue_add(e, b, down, wait);
}
static void tap(struct zmk_behavior_binding_event *e, uint32_t key) { q(e, key, true, 22); q(e, key, false, 28); }
/* HID usage order is 1..9, 0; arithmetic from NUMBER_0 is therefore wrong. */
static uint32_t hex_key(uint8_t n) {
    static const uint32_t number_keys[] = {NUMBER_0, NUMBER_1, NUMBER_2, NUMBER_3, NUMBER_4,
                                            NUMBER_5, NUMBER_6, NUMBER_7, NUMBER_8, NUMBER_9};
    return n < 10 ? number_keys[n] : A + n - 10;
}
static uint32_t pad_key(uint8_t n) { static const uint32_t k[] = {KP_N0, KP_N1, KP_N2, KP_N3, KP_N4, KP_N5, KP_N6, KP_N7, KP_N8, KP_N9}; return k[n]; }

static void linux_unicode(struct zmk_behavior_binding_event *e, uint16_t cp) {
    q(e, LCTRL, true, 25); q(e, LSHFT, true, 25); tap(e, U); q(e, LSHFT, false, 25); q(e, LCTRL, false, 35);
    /* Mask a physical Shift only after Ctrl+Shift+U was emitted. This keeps
     * the introducer intact while ensuring hexadecimal digits are unshifted. */
    struct zmk_behavior_binding mask = self; mask.param1 = CIRC_O + 1;
    zmk_behavior_queue_add(e, mask, true, 0);
    for (int s = 12; s >= 0; s -= 4) tap(e, hex_key((cp >> s) & 0xf));
    tap(e, SPACE);
    zmk_behavior_queue_add(e, self, true, 0);
}
static void windows_alt(struct zmk_behavior_binding_event *e, uint16_t cp) {
    /* Mask a physically held Shift so Shift+ACCENT still emits an Alt code. */
    zmk_hid_masked_modifiers_set(MOD_LSFT | MOD_RSFT);
    q(e, LALT, true, 30);
    uint16_t d = cp; uint16_t div = 1000;
    for (; div; div /= 10) { tap(e, pad_key(d / div)); d %= div; }
    q(e, LALT, false, 80);
    zmk_behavior_queue_add(e, self, true, 0);
}
static int pressed(struct zmk_behavior_binding *b, struct zmk_behavior_binding_event e) {
    if (b->param1 == 0) { zmk_hid_masked_modifiers_clear(); return ZMK_BEHAVIOR_OPAQUE; }
    if (b->param1 == CIRC_O + 1) { zmk_hid_masked_modifiers_set(MOD_LSFT | MOD_RSFT); return ZMK_BEHAVIOR_OPAQUE; }
    if (b->param1 > CIRC_O) return -EINVAL;
    bool shifted = zmk_hid_get_explicit_mods() & (MOD_LSFT | MOD_RSFT);
    uint16_t cp = shifted ? upper[b->param1] : lower[b->param1];
    int profile = zmk_ble_active_profile_index();
    if (profile == 3 || profile == 4) windows_alt(&e, cp); else linux_unicode(&e, cp);
    return ZMK_BEHAVIOR_OPAQUE;
}
static int released(struct zmk_behavior_binding *b, struct zmk_behavior_binding_event e) { return ZMK_BEHAVIOR_OPAQUE; }
static const struct behavior_driver_api api = {.binding_pressed = pressed, .binding_released = released};
BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &api);
#endif
