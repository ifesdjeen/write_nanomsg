/**
 *
 */

#include "collectd.h"
#include "plugin.h"
#include "common.h"
#include "network.h"
#include "utils_format_json.h"

#include <nanomsg/nn.h>
// #include <nanomsg/pair.h>
#include <nanomsg/bus.h>

/* Ethernet - (IPv6 + TCP) = 1500 - (40 + 32) = 1428 */
#ifndef NMSG_SEND_BUF_SIZE
# define NMSG_SEND_BUF_SIZE 1428
#endif

struct nanomsg_callback {
  int  nanomsg_sock;
  int  nanomsg_endpoint_id;
};

static int
nanomsg_write(const data_set_t *ds, /* {{{ */
              const value_list_t *value_list,
              user_data_t *user_data)
{
  struct nanomsg_callback *cb;
  char   ser_buf[4096];
  size_t ser_buf_free = sizeof (ser_buf);
  size_t ser_buf_fill = 0;
  int    nbytes;

  memset (ser_buf, 0, sizeof (ser_buf));

  format_json_initialize (ser_buf, &ser_buf_fill, &ser_buf_free);
  format_json_value_list (ser_buf,
                          &ser_buf_fill,
                          &ser_buf_free,
                          ds,
                          value_list,
                          0);
  format_json_finalize (ser_buf, &ser_buf_fill, &ser_buf_free);

  cb = user_data->data;
  int sock = cb->nanomsg_sock;

  nbytes = nn_send(sock, &ser_buf, ser_buf_fill, 0);

  if (nbytes == ser_buf_fill) {
    return (0);
  } else {
    return (1);
  }
}

static void
nanomsg_callback_free (void *data)
{
  WARNING("%s", "CALLING FREE");
}

static int
nanomsg_config_node (oconfig_item_t *cfg)
{
  struct nanomsg_callback *node_config;
  user_data_t user_data;

  node_config = malloc(sizeof(*node_config));

  node_config->nanomsg_sock = nn_socket (AF_SP, NN_BUS);

  if (node_config->nanomsg_sock < 0) {
    ERROR("write_nanomsg plugin: can't initialize socket");
  }

  if (node_config == NULL) {
    ERROR("write_nanomsg plugin: malloc failed");
    return (-1);
  }
  memset(node_config, 0, sizeof(*node_config));

  memset (&user_data, 0, sizeof (user_data));
  user_data.data = node_config;
  user_data.free_func = nanomsg_callback_free;

  int i;
  for (i = 0; i < cfg->children_num; i++) {
    oconfig_item_t *child = &cfg->children[i];

    if (strcasecmp ("Uri", child->key) == 0) {
      char *uri = NULL;
      cf_util_get_string(child, &uri);
      nn_connect (node_config->nanomsg_sock, uri);

      plugin_register_write ("nanomsg_write", nanomsg_write, &user_data);
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
