#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "input.h"

#include <inttypes.h>

int
config_create(const char *);

int
config_open_sections(const char *);

// "#" prefix should be implemented
long
config_get_long(const char *);

const char *
config_get_string(const char *);

uint8_t
config_get_bool(const char *);

int
config_close_sections(int);

void
config_destroy(void);

#endif
