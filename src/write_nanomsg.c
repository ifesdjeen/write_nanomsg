/**
 *
 */

#include "collectd.h"
#include "plugin.h"

/* Ethernet - (IPv6 + TCP) = 1500 - (40 + 32) = 1428 */
#ifndef WG_SEND_BUF_SIZE
# define WG_SEND_BUF_SIZE 1428
#endif

struct nanomsg_callback {
  char *connect_uri;
  char send_buf[]
  int  nanomsg_sock;
};

static int nanomsg_write(const data_set_t *ds, /* {{{ */
                         const value_list_t *vl,
                         user_data_t *ud)
{
  printf("OHAI\n");
  return 1;
}

static int nanomsg_config_node (oconfig_item_t *node_config) {
  // ..
}

static int nanomsg_config (oconfig_item_t *ci)
{
  int i;
  for (i = 0; i < ci->children_num; i++) {

  }
}

static int nanomsg_shutdown (void)
{

}


void module_register (void)
{
  plugin_register_complex_config ("write_nanomsg", nanomsg_config);
  plugin_register_shutdown ("write_nanomsg", nanomsg_shutdown);
}
