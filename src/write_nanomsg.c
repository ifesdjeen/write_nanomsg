/**
 *
 */

#include "collectd.h"
#include "plugin.h"

/* Ethernet - (IPv6 + TCP) = 1500 - (40 + 32) = 1428 */
#ifndef NMSG_SEND_BUF_SIZE
# define NMSG_SEND_BUF_SIZE 1428
#endif

struct nanomsg_callback {
  char *connect_uri;
  char send_buf[NMSG_SEND_BUF_SIZE];
  int  nanomsg_sock;
};

static int nanomsg_write(const data_set_t *ds, /* {{{ */
                         const value_list_t *vl,
                         user_data_t *ud)
{
  printf("OHAI\n");
  return 1;
}

static int nanomsg_config_node (oconfig_item_t *cfg) {
  struct nanomsg_callback *node_config;
  node_config = malloc(sizeof(*node_config));

  if (node_config) {
    ERROR("MALLOC FAILED");
    return -1;
  }

  int i;
  for (i = 0; i < cfg->children_num; i++) {
    oconfig_item_t *child = &cfg->children[i];

    if (strcasecmp ("uri", child->key))
      cf_util_get_string(child, &node_config->connect_uri);
  }
  return 0;
}

static int nanomsg_config (oconfig_item_t *ci)
{
  int i;
  for (i = 0; i < ci->children_num; i++) {
    oconfig_item_t *child = &ci->children[i];
    if (strcasecmp ("Server", child->key)) {
      nanomsg_config_node(child);
    } else {
      ERROR("Invalid configuration option: %s", child->key);
    }
  }

  return 0;
}

/* static int nanomsg_shutdown (void) */
/* { */

/* } */


void module_register (void)
{
  plugin_register_complex_config ("write_nanomsg", nanomsg_config);
  /* plugin_register_shutdown ("write_nanomsg", nanomsg_shutdown); */
}
