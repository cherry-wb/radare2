/* radare - LGPL - Copyright 2009 pancake<nopcode.org> */

#include <r_debug.h>
#include "../config.h"

static struct r_debug_handle_t *debug_static_plugins[] = 
	{ R_DEBUG_STATIC_PLUGINS };

R_API int r_debug_handle_init(struct r_debug_t *dbg)
{
	int i;
	INIT_LIST_HEAD(&dbg->handlers);
	for(i=0;debug_static_plugins[i];i++)
		r_debug_handle_add (dbg, debug_static_plugins[i]);
	return R_TRUE;
}

R_API int r_debug_use(struct r_debug_t *dbg, const char *str)
{
	struct list_head *pos;
	list_for_each_prev(pos, &dbg->handlers) {
		struct r_debug_handle_t *h = list_entry(pos, struct r_debug_handle_t, list);
		if (!strcmp(str, h->name)) {
			dbg->h = h;
			if (h->reg_profile) {
				char *profile = dbg->h->reg_profile();
				r_reg_set_profile_string(dbg->reg, profile);
				free(profile);
			}
			return R_TRUE;
		}
	}
	return R_FALSE;
}

R_API int r_debug_handle_list(struct r_debug_t *dbg)
{
	int count = 0;
	struct list_head *pos;
	list_for_each_prev(pos, &dbg->handlers) {
		struct r_debug_handle_t *h = list_entry(pos, struct r_debug_handle_t, list);
		printf("%d %s %s\n", count, h->name, ((h==dbg->h)?"*":""));
		count++;
	}
	return R_FALSE;
}

R_API int r_debug_handle_add(struct r_debug_t *dbg, struct r_debug_handle_t *foo)
{
	list_add_tail(&(foo->list), &(dbg->handlers));
	return R_TRUE;
}
