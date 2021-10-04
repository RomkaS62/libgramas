#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "gramas/cmd_parse.h"
#include "gramas/gr_str.h"

#define buf_append_str_l(buf, str)	buf_append_arr((buf), str, sizeof(str) - 1)
#define buf_append_str(buf, str)	buf_append_arr((buf), (str), strlen(str))

static struct gr_parse_arg *
get_arg_definition(const struct gr_cmd_parser * const parser, const char *arg);

static struct gr_parse_switch *
get_switch_definition(const struct gr_cmd_parser * const parser, const char *sw);

int gr_parse_str(const char *cmd_arg, void *target)
{
	target = (void *)cmd_arg;
	return 0;
}

int gr_parse_int(const char *cmd_arg, void *_target)
{
	int *target = _target;
	long tmp;
	char *endptr;

	tmp = strtol(cmd_arg, &endptr, 10);
	if (tmp > INT_MAX) {
		return 1;
	}
	if (tmp < INT_MIN) {
		return 2;
	}
	if (*endptr != '\0') {
		return 3;
	}
	*target = (int)tmp;
	return 0;
}

int gr_parse_uint(const char *cmd_arg, void *_target)
{
	unsigned int *target = _target;
	unsigned long tmp;
	char *endptr;

	tmp = strtoul(cmd_arg, &endptr, 10);
	if (tmp > UINT_MAX) {
		return 1;
	}
	if (*endptr != '\0') {
		return 2;
	}
	*target = (int)tmp;
	return 0;
}

void gr_parse_int_err(const char *cmd_arg)
{
	fprintf(stderr, "\"%s\" is not a valid integer!\n", cmd_arg);
}

void gr_parse_uint_err(const char *cmd_arg)
{
	fprintf(stderr, "\"%s\" is not a valid unsigned integer!\n", cmd_arg);
}

int gr_parse_cmd(struct gr_cmd_parser *parser, const char **args, size_t count)
{
	int ret = 0;
	int err;
	size_t i;
	struct gr_parse_arg *arg_parser = NULL;
	struct gr_parse_switch *sw_parser = NULL;
	const char *arg;

	for (i = 0; i < count; i++) {
		arg = args[i];
		if (strcmp(arg, parser->arg_terminator) == 0) {
			i++;
			break;
		}
		arg_parser = get_arg_definition(parser, arg);
		sw_parser = get_switch_definition(parser, arg);
		if (arg_parser != NULL) {
			if (i + 1 < count) {
				i++;
			} else {
				fputs("Unexpected end of command line!", stderr);
				ret = GR_PARSE_CMD_UNEXPECTED_EOL;
				goto err;
			}
			arg = args[i];
			if (err = arg_parser->on_arg(arg, arg_parser->on_arg)) {
				arg_parser->on_err(arg, err);
			}
		} else if (sw_parser != NULL) {
			*sw_parser->target |= 1 << sw_parser->bit;
		} else {
			break;
		}
	}
	for (; i < count; i++) {
		arg = args[i];
		if (err = parser->on_default_arg(arg, parser->default_arg_target)) {
			parser->on_default_arg_err(arg, err);
		}
	}
err:
	return ret;
}

char * gr_parse_generate_help(struct gr_cmd_parser *parser)
{
	static const char *fmt = "%s:\t%s\n";

	size_t i;
	struct gr_parse_arg *arg;
	struct gr_parse_switch *sw;
	struct gr_str str;

	gr_str_init(&str);
	gr_sprintf_a(&str, "%s\n", parser->help_text);
	for (i = 0; i < parser->arg_count; i++) {
		arg = &parser->args[i];
		gr_sprintf_a(&str, fmt, arg->arg, arg->help_text);
	}
	for (i = 0; i < parser->sw_count; i++) {
		sw = &parser->switches[i];
		gr_sprintf_a(&str, fmt, sw->sw, sw->help_text);
	}
	return str.str;
}

static struct gr_parse_arg *
get_arg_definition(const struct gr_cmd_parser * const parser, const char *arg)
{
	size_t i;
	struct gr_parse_arg *ret = NULL;
	struct gr_parse_arg *arg_parser = NULL;

	for (i = 0; i < parser->arg_count; i++) {
		arg_parser = &parser->args[i];
		if (strcmp(arg_parser->arg, arg) == 0) {
			ret = arg_parser;
			break;
		}
	}

	return ret;
}

static struct gr_parse_switch *
get_switch_definition(const struct gr_cmd_parser * const parser, const char *sw)
{
	size_t i;
	struct gr_parse_switch *ret = NULL;
	struct gr_parse_switch *sw_parser = NULL;

	for (i = 0; i < parser->sw_count; i++) {
		sw_parser = &parser->switches[i];
		if (strcmp(sw_parser->sw, sw) == 0) {
			ret = sw_parser;
			break;
		}
	}

	return ret;
}
