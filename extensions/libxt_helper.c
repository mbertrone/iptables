/* Shared library add-on to iptables to add related packet matching support. */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <xtables.h>
#include <linux/netfilter/xt_helper.h>

/* Function which prints out usage message. */
static void
help(void)
{
	printf(
"helper match v%s options:\n"
"[!] --helper string        Match helper identified by string\n"
"\n",
IPTABLES_VERSION);
}

static const struct option opts[] = {
	{ "helper", 1, 0, '1' },
	{0}
};

/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const void *entry,
      struct xt_entry_match **match)
{
	struct xt_helper_info *info = (struct xt_helper_info *)(*match)->data;

	switch (c) {
	case '1':
		if (*flags)
			exit_error(PARAMETER_PROBLEM,
					"helper match: Only use --helper ONCE!");
		check_inverse(optarg, &invert, &invert, 0);
		strncpy(info->name, optarg, 29);
		info->name[29] = '\0';
		if (invert)
			info->invert = 1;
		*flags = 1;
		break;

	default:
		return 0;
	}
	return 1;
}

/* Final check; must have specified --helper. */
static void
final_check(unsigned int flags)
{
	if (!flags)
		exit_error(PARAMETER_PROBLEM,
			   "helper match: You must specify `--helper'");
}

/* Prints out the info. */
static void
print(const void *ip,
      const struct xt_entry_match *match,
      int numeric)
{
	struct xt_helper_info *info = (struct xt_helper_info *)match->data;

	printf("helper match %s\"%s\" ", info->invert ? "! " : "", info->name);
}

/* Saves the union ipt_info in parsable form to stdout. */
static void
save(const void *ip, const struct xt_entry_match *match)
{
	struct xt_helper_info *info = (struct xt_helper_info *)match->data;

	printf("%s--helper \"%s\" ",info->invert ? "! " : "", info->name);
}

static struct xtables_match helper = { 
	.family		= AF_INET,
	.name		= "helper",
	.version	= IPTABLES_VERSION,
	.size		= XT_ALIGN(sizeof(struct xt_helper_info)),
	.help		= &help,
	.parse		= &parse,
	.final_check	= &final_check,
	.print		= &print,
	.save		= &save,
	.extra_opts	= opts,
};

static struct xtables_match helper6 = { 
	.family		= AF_INET6,
	.name		= "helper",
	.version	= IPTABLES_VERSION,
	.size		= XT_ALIGN(sizeof(struct xt_helper_info)),
	.help		= &help,
	.parse		= &parse,
	.final_check	= &final_check,
	.print		= &print,
	.save		= &save,
	.extra_opts	= opts,
};

void _init(void)
{
	xtables_register_match(&helper);
	xtables_register_match(&helper6);
}