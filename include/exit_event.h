#include <minwindef.h>
#include <minwinbase.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EXIT_CON,
    EXIT_WM
} exit_type;

typedef struct {
    bool available;
    exit_type type;
    union {
        DWORD fdwCtrlType;
        LPARAM lParam;
    } arg;
} exit_info_t;

bool init_exit_handler(void);

void wait_for_exit(void);

exit_info_t get_exit_info(void);

#ifdef __cplusplus
} // extern "C"
#endif
