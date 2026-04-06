#ifndef THL_CORE_H
#define THL_CORE_H

#include <cjson/cJSON.h>

/**
 * General usage notes:
 * - A parameter which is converted is one which is guaranteed to be freed by
 *   the function being called, even if the function fails
 * - A parameter must not be NULL unless stated otherwise
 */

// TODO: const-correctness

typedef int thl_bus_t;

typedef struct thl_dev_info_t {
    char *device_id;
    char **type_names;
} thl_dev_info_t;

typedef thl_dev_info_t **thl_dev_list_t;

typedef struct thl_pmsg_t {
    char *data;
    size_t len;
} thl_pmsg_t;

typedef enum thl_dir_t {
    THL_RIGHT, THL_LEFT, THL_UP, THL_DOWN, THL_FRONT, THL_BACK,
} thl_dir_t;


/**
 * Convert a JSON parameter list `params` into a JSON invoke message which can
 * be passed to thl_bus_invoke, or converted to a prepared message with
 * thl_pmsg_convert.
 * - `params` may be NULL, which causes failure
 * - returns NULL on failure
 */
cJSON* thl_invoke_msg_convert(char* device_id, char* method_name, cJSON* params);
/**
 * Convert a direction enum to a string usable in messages.
 * - returns NULL on an invalid direction, or a string on success. The string
 *   MUST NEVER BE FREED; they are string literals
 */
char* thl_dir_to_str(thl_dir_t dir);


/**
 * Open a bus with an hypervisor console device file path.
 * - `path` may be NULL, which defaults to "/dev/hvc0"
 * - returns NULL on failure
 */
thl_bus_t thl_bus_open(char *path);
/**
 * Close a bus.
 * - `bus` may be NULL
 */
void thl_bus_close(thl_bus_t bus);
/**
 * Read an incoming message from a bus. Blocks until a message is received.
 * - returns NULL on failure, or a parsed JSON response
 */
cJSON* thl_bus_read(thl_bus_t bus);
/**
 * Write a JSON message to the bus.
 * - returns 1 on success, 0 on failure
 */
int thl_bus_write(thl_bus_t bus, cJSON *msg);
/**
 * Handle the response of a thl_bus_request call. Blocks until a message is
 * received.
 * - returns NULL on failure, or a parsed JSON response
 */
cJSON* thl_bus_handle_response(thl_bus_t bus, const char *expected_response_type);
/**
 * Send an outgoing message to the bus and parse the response.
 * - `msg` may be NULL. `msg` will also be freed, even if this function fails
 * - returns NULL on failure, or a parsed JSON response
 */
cJSON* thl_bus_request(thl_bus_t bus, cJSON *msg, const char *expected_response_type);
/**
 * Similar to thl_bus_request, but specialised for method invocations.
 * - `msg` may be NULL. `msg` will also be freed, even if this function fails
 * - returns NULL on failure, or a parsed JSON response
 */
cJSON* thl_bus_invoke(thl_bus_t bus, cJSON *msg);
/**
 * Similar to thl_bus_invoke, but specialised for method invocations which
 * don't return data.
 * - `msg` may be NULL. `msg` will also be freed, even if this function fails
 * - returns 0 on failure, or 1 on success
 */
int thl_bus_invoke_nodata(thl_bus_t bus, cJSON *msg);


/**
 * List all devices available in a bus.
 * - `bus` may be NULL
 * - returns NULL on failure, or a device list on success
 */
thl_dev_list_t thl_bus_list(thl_bus_t bus);
/**
 * Free a device list returned from thl_bus_list.
 * - `list` may be NULL
 */
void thl_dev_list_free(thl_dev_list_t list);
/**
 * Find a device by type in a device list.
 * - `list` may be NULL
 * - returns NULL on failure, or the first device ID matching the type on
 *   success
 */
char* thl_dev_list_find(thl_dev_list_t list, const char *wanted_type_name);
/**
 * Find a device by type available in a bus. Prefer using thl_dev_list_find if
 * you need to find multiple devices with different types.
 * - `bus` may be NULL
 * - returns NULl on failure, or the first device ID matching the type on
 *   success
 */
char* thl_bus_find(thl_bus_t bus, const char *wanted_type_name);


/**
 * Create a prepared message from a message. Prepared messages can be sent more
 * efficiently than regular messages, unless they're only sent once. You should
 * prefer using prepared messages if you intend to send a message multiple
 * times.
 * - `msg` may be NULL
 * - returns NULL on failure, or a prepared message on success
 */
thl_pmsg_t* thl_pmsg_create(cJSON *msg);
/**
 * Similar to thl_pmsg_create, but the message is freed.
 * - `msg` may be NULL
 * - returns NULL on failure, or a prepared message on success
 */
thl_pmsg_t* thl_pmsg_convert(cJSON *msg);
/**
 * Free a prepared message.
 * - `pmsg` may be NULL
 */
void thl_pmsg_free(thl_pmsg_t *pmsg);
/**
 * Similar to thl_bus_write, but for prepared messages.
 * - returns 1 on success, 0 on failure
 */
int thl_bus_pwrite(thl_bus_t bus, const thl_pmsg_t *pmsg);
/**
 * Similar to thl_bus_request, but for prepared messages. NULL messages are not
 * allowed, unlike the non-prepared counterpart.
 * - returns NULL on failure, or a parsed JSON response
 */
cJSON* thl_bus_prequest(thl_bus_t bus, const thl_pmsg_t *pmsg, const char *expected_response_type);
/**
 * Similar to thl_bus_invoke, but for prepared messages. NULL messages are not
 * allowed, unlike the non-prepared counterpart.
 * - returns NULL on failure, or a parsed JSON response
 */
cJSON* thl_bus_pinvoke(thl_bus_t bus, thl_pmsg_t *pmsg);
/**
 * Similar to thl_bus_pinvoke, but specialised for method invocations which
 * don't return data.
 * - returns 0 on failure, or 1 on success
 */
int thl_bus_pinvoke_nodata(thl_bus_t bus, thl_pmsg_t *pmsg);

#endif
