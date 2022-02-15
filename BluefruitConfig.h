// Bluefruit configuration header
#define DEVICE_NAME "G-Hand Controller"
#define DEVICE_MANUFACTURER "Professor D Glizzy"
#define DEVICE_MODEL "Hand enhancer v69"

#define PASSCODE_ENABLED true
#define PASSCODE "12345678"

// make Feather pairable as soon as current connection is lost
#define AUTO_RESTART_ADVERTISING true

// start with timeout = 0 will advertise forever (until connected)
#define ADVERTISING_TIMEOUT 0

#define SERIAL_DEBUG true
#define SERIAL_BAUD 115200


// interval * 1.25 = interval in ms
#define CONN_INTERVAL_MIN 9
#define CONN_INTERVAL_MAX 16

// interval * 0.625 = interval in ms
#define ADVERTISING_INTERVAL_MIN 32
#define ADVERTISING_INTERVAL_MAX 244

// in seconds
#define FAST_MODE_TIMEOUT 30

/* Start Advertising
 * - Enable auto advertising if disconnected
 * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
 * - Timeout for fast mode is 30 seconds
 * - Start(timeout) with timeout = 0 will advertise forever (until connected)
*/
