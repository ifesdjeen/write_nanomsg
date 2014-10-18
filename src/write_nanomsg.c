/**
 *
 */

#include "collectd.h"
#include "plugin.h"
#include "common.h"
#include "network.h"
#include "utils_format_json.h"

#include <nanomsg/nn.h>
#include <nanomsg/pair.h>

/* Ethernet - (IPv6 + TCP) = 1500 - (40 + 32) = 1428 */
#ifndef NMSG_SEND_BUF_SIZE
# define NMSG_SEND_BUF_SIZE 1428
#endif

struct nanomsg_callback {
  int  nanomsg_sock;
};

static int
nanomsg_write(const data_set_t *ds, /* {{{ */
              const value_list_t *value_list,
              user_data_t *user_data)
{
  struct nanomsg_callback *cb;
  void *buf = nn_allocmsg (4096, 0);
  size_t bfree = 4096;
  size_t bfill = 0;

  cb = user_data->data;
  int sock = cb->nanomsg_sock;

  format_json_initialize (buf, &bfill, &bfree);
  format_json_value_list (buf, &bfill, &bfree, ds, value_list, 0);
  format_json_finalize (buf, &bfill, &bfree);

  nn_send(sock, &buf, NN_MSG, 0);

  return (0);
}

static int nanomsg_flush (cdtime_t timeout,
                          const char *identifier __attribute__((unused)),
                          user_data_t *user_data)
{
  WARNING("%s", "FLUSHING");
  return (0);
}

static void
nanomsg_callback_free (void *data)
{

}

static int
nanomsg_config_node (oconfig_item_t *cfg)
{
  struct nanomsg_callback *node_config;
  user_data_t user_data;

  node_config = malloc(sizeof(*node_config));

  node_config->nanomsg_sock = nn_socket (AF_SP, NN_PAIR);

  if (node_config == NULL) {
    ERROR("write_nanomsg plugin: malloc failed");
    return (-1);
  }
  memset(node_config, 0, sizeof(*node_config));

  node_config->nanomsg_sock = -1;

  memset (&user_data, 0, sizeof (user_data));
  user_data.data = node_config;
  user_data.free_func = nanomsg_callback_free;

  int i;
  for (i = 0; i < cfg->children_num; i++) {
    oconfig_item_t *child = &cfg->children[i];

    if (strcasecmp ("Uri", child->key) == 0) {
      /* char* uri; */
      /* cf_util_get_string(child, &uri); */
      nn_bind (node_config->nanomsg_sock, "tcp://127.0.0.1:5555");

      /* char callback_name[DATA_MAX_NAME_LEN]; */

      /* ssnprintf(callback_name, sizeof(callback_name), "write_nanomsg/%s", */
      /*           uri); */

      char* cb_name = "nanomsg_write";
      WARNING("%s", cb_name);

      plugin_register_write (cb_name, nanomsg_write, &user_data);

      plugin_register_flush (cb_name, nanomsg_flush, &user_data);

    } else {
      ERROR("nanomsg_config Error while configuring node: "
            "option: %s.", child->key);
    }
  }

  return (0);
}

static int
nanomsg_config (oconfig_item_t *ci)
{
  int i;
  for (i = 0; i < ci->children_num; i++) {
    oconfig_item_t *child = &ci->children[i];

    if (strcasecmp ("Server", child->key) == 0) {
      nanomsg_config_node(child);
    } else {
      ERROR("nanomsg_config: invalid configuration option: %s", child->key);
    }
  }

  return (0);
}

/* static int nanomsg_shutdown (void) */
/* { */

/* } */



void module_register (void)
{
  plugin_register_complex_config ("write_nanomsg", nanomsg_config);

  /* plugin_register_shutdown ("write_nanomsg", nanomsg_shutdown); */
}
