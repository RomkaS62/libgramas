#ifndef GRAMAS_CMD_PARSE_H
#define GRAMAS_CMD_PARSE_H

#include <stdlib.h>
#include "gr_export.h"

#define GR_PARSE_CMD_UNEXPECTED_EOL	(1)
#define GR_PARSE_CMD_UNRECOGNISED_ARG	(2)

GR_CDECL_BEGIN

struct GR_EXPORT gr_parse_arg {
	const char *arg;	/* Command line argument string */
	void *target;		/* Parsed argument will be stored here */
	/* Parses argument cmd_arg and stores result in targer. */
	int (*on_arg)(const char *cmd_arg, void *target);
	/* Called if on_arg fails. cmd_arg is the next argument after command
	 * line argument string and error code.
	 * */
	void (*on_err)(const char *cmd_arg, int err_code);
	const char *help_text;	/* Explanation for the user */
};

struct GR_EXPORT gr_parse_switch {
	const char *sw;		/* Command line argument switch */
	int *target;		/* Switch bit stored here. */
	int bit;		/* Bit in the given int. */
	const char *help_text;	/* Explanation for the user. */
};

struct GR_EXPORT gr_cmd_parser {
	size_t arg_count;
	struct gr_parse_arg *args;
	size_t sw_count;
	struct gr_parse_switch *switches;
	const char *arg_terminator;
	void *default_arg_target;
	int (*on_default_arg)(const char *arg, void *target);
	void (*on_default_arg_err)(const char *arg, int err);
	const char *help_text;
};

/* Default command line argument parsers. */
GR_EXPORT int gr_parse_str(const char *cmd_arg, void *target);
GR_EXPORT int gr_parse_int(const char *cmd_arg, void *target);
GR_EXPORT int gr_parse_uint(const char *cmd_arg, void *target);

/* Default error handlers. */
GR_EXPORT void gr_parse_int_err(const char *cmd_arg);
GR_EXPORT void gr_parse_uint_err(const char *cmd_arg);

/* Parses command line arguments according to given argument and switch
 * definitions.
 * */
GR_EXPORT int gr_parse_cmd(struct gr_cmd_parser *parser, const char **args, size_t count);
/* Generates help string from argument and switch definitions. */
GR_EXPORT char * gr_parse_generate_help(struct gr_cmd_parser *parser);

GR_CDECL_END

#endif /* GRAMAS_CMD_PARSE_H */