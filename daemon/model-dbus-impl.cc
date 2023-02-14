/* SPDX-License-Identifier: Apache-2.0 */
/**
 * Copyright (c) 2022 Samsung Electronics Co., Ltd. All Rights Reserved.
 *
 * @file model-dbus-impl.cc
 * @date 29 Jul 2022
 * @brief DBus implementation for Model Interface
 * @see	https://github.com/nnstreamer/api
 * @author Sangjung Woo <sangjung.woo@samsung.com>
 * @bug No known bugs except for NYI items
 */

#include <glib.h>
#include <errno.h>

#include "common.h"
#include "modules.h"
#include "gdbus-util.h"
#include "dbus-interface.h"
#include "model-dbus.h"
#include "service-db.hh"

static MachinelearningServiceModel *g_gdbus_instance = NULL;

/**
 * @brief Utility function to get the DBus proxy of Model interface.
 */
static MachinelearningServiceModel *
gdbus_get_model_instance (void)
{
  return machinelearning_service_model_skeleton_new ();
}

/**
 * @brief Utility function to release DBus proxy of Model interface.
 */
static void
gdbus_put_model_instance (MachinelearningServiceModel ** instance)
{
  g_clear_object (instance);
}

/**
 * @brief Event handler list of Model interface
 */
static struct gdbus_signal_info handler_infos[] = { };

/**
 * @brief The callback function for probing Model Interface module.
 */
static int
probe_model_module (void *data)
{
  int ret = 0;
  g_debug ("probe_model_module");

  g_gdbus_instance = gdbus_get_model_instance ();
  if (NULL == g_gdbus_instance) {
    g_critical ("cannot get a dbus instance for the %s interface\n",
        DBUS_MODEL_INTERFACE);
    return -ENOSYS;
  }

  ret = gdbus_connect_signal (g_gdbus_instance,
      ARRAY_SIZE(handler_infos), handler_infos);
  if (ret < 0) {
    g_critical ("cannot register callbacks as the dbus method invocation "
        "handlers\n ret: %d", ret);
    ret = -ENOSYS;
    goto out;
  }

  ret = gdbus_export_interface (g_gdbus_instance, DBUS_MODEL_PATH);
  if (ret < 0) {
    g_critical ("cannot export the dbus interface '%s' "
        "at the object path '%s'\n", DBUS_MODEL_INTERFACE, DBUS_MODEL_PATH);
    ret = -ENOSYS;
    goto out_disconnect;
  }

  return 0;

out_disconnect:
  gdbus_disconnect_signal (g_gdbus_instance,
    ARRAY_SIZE (handler_infos), handler_infos);

out:
  gdbus_put_model_instance (&g_gdbus_instance);

  return ret;
}

/**
 * @brief The callback function for initializing Model Interface module.
 */
static void
init_model_module (void *data) { }

/**
 * @brief The callback function for exiting Model Interface module.
 */
static void
exit_model_module (void *data)
{
  gdbus_disconnect_signal (g_gdbus_instance,
    ARRAY_SIZE (handler_infos), handler_infos);
  gdbus_put_model_instance (&g_gdbus_instance);
}

static const struct module_ops model_ops = {
  .name = "model-interface",
  .probe = probe_model_module,
  .init = init_model_module,
  .exit = exit_model_module,
};

MODULE_OPS_REGISTER (&model_ops)
