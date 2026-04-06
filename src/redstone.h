#ifndef THL_REDSTONE_H
#define THL_REDSTONE_H

#include "core.h"

typedef enum thl_bundle_color_t {
    THL_WHITE,      THL_ORANGE, THL_MAGENTA, THL_LIGHT_BLUE,
    THL_YELLOW,     THL_LIME,   THL_PINK,    THL_GRAY,
    THL_LIGHT_GRAY, THL_CYAN,   THL_PURPLE,  THL_BLUE,
    THL_BROWN,      THL_GREEN,  THL_RED,     THL_BLACK
} thl_bundle_color_t;


/**
 * Create a message for the redstone:getRedstoneInput bus method. Use this if
 * you intend to invoke the method repeatedly, by converting the message to a
 * prepared message.
 * - returns NULL on failure, or a JSON message on success
 */
cJSON* thl_redstone_msg_create_getRedstoneInput(char *device_id, thl_dir_t direction);
/**
 * Invoke the redstone:getRedstoneInput bus method with a prepared message.
 * - returns the level on success, or -1 on failure
 */
int thl_redstone_pinvoke_getRedstoneInput(thl_bus_t bus, thl_pmsg_t *pmsg);
/**
 * Invoke the redstone:getRedstoneInput bus method. Use this for one-off
 * invocations.
 * - returns the level on success, or -1 on failure
 */
int thl_redstone_getRedstoneInput(thl_bus_t bus, char *device_id, thl_dir_t direction);


/**
 * Create a message for the redstone:getBundledOutput bus method. Use this if
 * you intend to invoke the method repeatedly, by converting the message to a
 * prepared message.
 * - returns NULL on failure, or a JSON message on success
 */
cJSON* thl_redstone_msg_create_getBundledOutput(char *device_id, thl_dir_t direction);
/**
 * Invoke the redstone:getBundledOutput bus method with a prepared message.
 * - `outputs` is the 16 element buffer used to store the outputs, on success.
 *   Partial data may be written on failure
 * - returns the level on success, or -1 on failure
 */
int thl_redstone_pinvoke_getBundledOutput(thl_bus_t bus, unsigned char outputs[16], thl_pmsg_t *pmsg);
/**
 * Invoke the redstone:getBundledOutput bus method. Use this for one-off
 * invocations.
 * - `outputs` is the 16 element buffer used to store the outputs, on success.
 *   Partial data may be written on failure
 * - returns the level on success, or -1 on failure
 */
int thl_redstone_getBundledOutput(thl_bus_t bus, unsigned char outputs[16], char *device_id, thl_dir_t direction);


/**
 * Create a message for the redstone:getBundledInput bus method. Use this if
 * you intend to invoke the method repeatedly, by converting the message to a
 * prepared message.
 * - returns NULL on failure, or a JSON message on success
 */
cJSON* thl_redstone_msg_create_getBundledInput(char *device_id, thl_dir_t direction);
/**
 * Invoke the redstone:getBundledInput bus method with a prepared message.
 * - `inputs` is the 16 element buffer used to store the inputs, on success.
 *   Partial data may be written on failure
 * - returns the level on success, or -1 on failure
 */
int thl_redstone_pinvoke_getBundledInput(thl_bus_t bus, unsigned char inputs[16], thl_pmsg_t *pmsg);
/**
 * Invoke the redstone:getBundledInput bus method. Use this for one-off
 * invocations.
 * - `inputs` is the 16 element buffer used to store the inputs, on success.
 *   Partial data may be written on failure
 * - returns the level on success, or -1 on failure
 */
int thl_redstone_getBundledInput(thl_bus_t bus, unsigned char inputs[16], char *device_id, thl_dir_t direction);


/**
 * Create a message for the redstone:getRedstoneOutput bus method. Use this if
 * you intend to invoke the method repeatedly, by converting the message to a
 * prepared message.
 * - returns NULL on failure, or a JSON message on success
 */
cJSON* thl_redstone_msg_create_getRedstoneOutput(char *device_id, thl_dir_t direction);
/**
 * Invoke the redstone:getRedstoneOutput bus method with a prepared message.
 * - returns the level on success, or -1 on failure
 */
int thl_redstone_pinvoke_getRedstoneOutput(thl_bus_t bus, thl_pmsg_t *pmsg);
/**
 * Invoke the redstone:getRedstoneOutput bus method. Use this for one-off
 * invocations.
 * - returns the level on success, or -1 on failure
 */
int thl_redstone_getRedstoneOutput(thl_bus_t bus, char *device_id, thl_dir_t direction);


/**
 * Create a message for the redstone:setRedstoneOutput bus method. Use this if
 * you intend to invoke the method repeatedly, by converting the message to a
 * prepared message.
 * - returns NULL on failure, or a JSON message on success
 */
cJSON* thl_redstone_msg_create_setRedstoneOutput(char *device_id, thl_dir_t direction, unsigned char level);
/**
 * Invoke the redstone:setRedstoneOutput bus method. Use this for one-off
 * invocations.
 * - returns 1 on success, or 0 on failure
 */
int thl_redstone_setRedstoneOutput(thl_bus_t bus, char *device_id, thl_dir_t direction, unsigned char level);


/**
 * Create a message for the redstone:setBundledOutput bus method. Use this if
 * you intend to invoke the method repeatedly, by converting the message to a
 * prepared message.
 * - returns NULL on failure, or a JSON message on success
 */
cJSON* thl_redstone_msg_create_setBundledOutput(char *device_id, thl_dir_t direction, unsigned char level, thl_bundle_color_t color);
/**
 * Invoke the redstone:setBundledOutput bus method. Use this for one-off
 * invocations.
 * - returns 1 on success, or 0 on failure
 */
int thl_redstone_setBundledOutput(thl_bus_t bus, char *device_id, thl_dir_t direction, unsigned char level, thl_bundle_color_t color);


/**
 * Create a message for the redstone:setBundledOutputs bus method. Use this if
 * you intend to invoke the method repeatedly, by converting the message to a
 * prepared message.
 * - returns NULL on failure, or a JSON message on success
 */
cJSON* thl_redstone_msg_create_setBundledOutputs(char *device_id, thl_dir_t direction, unsigned char levels[16]);
/**
 * Invoke the redstone:setBundledOutputs bus method. Use this for one-off
 * invocations.
 * - returns 1 on success, or 0 on failure
 */
int thl_redstone_setBundledOutputs(thl_bus_t bus, char *device_id, thl_dir_t direction, unsigned char levels[16]);


#endif
