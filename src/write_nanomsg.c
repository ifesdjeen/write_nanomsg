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
};

static int
nanomsg_write(const data_set_t *ds, /* {{{ */
              const value_list_t *value_list,
              user_data_t *user_data)
{
  struct nanomsg_callback *cb;
  char   send_buffer[4096];
  size_t send_buffer_free = sizeof (send_buffer);
  size_t send_buffer_fill = 0;

  memset (send_buffer, 0, sizeof (send_buffer));

  format_json_initialize (send_buffer, &send_buffer_fill, &send_buffer_free);
  format_json_value_list (send_buffer,
                          &send_buffer_fill,
                          &send_buffer_free,
                          ds,
                          value_list,
                          0);
  format_json_finalize (send_buffer, &send_buffer_fill, &send_buffer_free);

  cb = user_data->data;
  int sock = cb->nanomsg_sock;
  nn_send(sock, &send_buffer, send_buffer_fill, 0);

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
      /* char* uri; */
      /* cf_util_get_string(child, &uri); */
      nn_connect (node_config->nanomsg_sock, "tcp://127.0.0.1:5555");
      WARNING("%s", "CONNECTING");
      /* char callback_name[DATA_MAX_NAME_LEN]; */

      /* ssnprintf(callback_name, sizeof(callback_name), "write_nanomsg/%s", */
      /*           uri); */

      char* cb_name = "nanomsg_write";
      WARNING("%s", cb_name);

      plugin_register_write (cb_name, nanomsg_write, &user_data);
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
