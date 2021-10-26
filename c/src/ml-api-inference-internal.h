/* SPDX-License-Identifier: Apache-2.0 */
/**
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved.
 *
 * @file ml-api-internal-nnstreamer.h
 * @date 20 October 2021
 * @brief ML C-API internal header with NNStreamer deps.
 *        This file should NOT be exported to SDK or devel package.
 * @see	https://github.com/nnstreamer/api
 * @author MyungJoo Ham <myungjoo.ham@samsung.com>
 * @bug No known bugs except for NYI items
 */

#ifndef __ML_API_INTERNAL_NNSTREAMER_H__
#define __ML_API_INTERNAL_NNSTREAMER_H__

#include <glib.h>
#include <gst/gst.h>
#include <tensor_typedef.h>

#include <nnstreamer.h>
#include <nnstreamer-single.h>
#include <nnstreamer_plugin_api_filter.h>

#include "ml-api-internal.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/***** Wrappers of tizen-api-internal.h for pipelines *****/
#if defined (__TIZEN__)
#if defined (__PRIVILEGE_CHECK_SUPPORT__)

#define convert_tizen_element(...) _ml_tizen_convert_element(__VA_ARGS__)

#if (TIZENVERSION >= 5) && (TIZENVERSION < 9999)
#define get_tizen_resource(...) _ml_tizen_get_resource(__VA_ARGS__)
#define release_tizen_resource(...) _ml_tizen_release_resource(__VA_ARGS__)

#elif (TIZENVERSION < 5)
#define get_tizen_resource(...) (0)
#define release_tizen_resource(...) do { } while (0)
typedef void * mm_resource_manager_h;
typedef enum { MM_RESOURCE_MANAGER_RES_TYPE_MAX } mm_resource_manager_res_type_e;

#else /* TIZENVERSION */
#error Tizen version is not defined.
#endif /* TIZENVERSION */

#else /* __PRIVILEGE_CHECK_SUPPORT__ */

#define convert_tizen_element(...) ML_ERROR_NONE
#define get_tizen_resource(...) ML_ERROR_NONE
#define release_tizen_resource(...)

#endif  /* __PRIVILEGE_CHECK_SUPPORT__ */

#else /* __TIZEN */

#define convert_tizen_element(...) ML_ERROR_NONE
#define get_tizen_resource(...) ML_ERROR_NONE
#define release_tizen_resource(...)

#endif  /* __TIZEN__ */
/**
 * @brief Internal private representation of custom filter handle.
 */
typedef struct {
  char *name;
  unsigned int ref_count;
  GMutex lock;
  ml_tensors_info_h in_info;
  ml_tensors_info_h out_info;
  ml_custom_easy_invoke_cb cb;
  void *pdata;
} ml_custom_filter_s;

/**
 * @brief Internal private representation of tensor_if custom conditon.
 * @since_tizen 6.5
 */
typedef struct {
  char *name;
  unsigned int ref_count;
  GMutex lock;
  ml_pipeline_if_custom_cb cb;
  void *pdata;
} ml_if_custom_s;

/**
 * @brief Possible controls on elements of a pipeline.
 */
typedef enum {
  ML_PIPELINE_ELEMENT_UNKNOWN = 0x0,
  ML_PIPELINE_ELEMENT_SINK = 0x1,
  ML_PIPELINE_ELEMENT_APP_SRC = 0x2,
  ML_PIPELINE_ELEMENT_APP_SINK = 0x3,
  ML_PIPELINE_ELEMENT_VALVE = 0x4,
  ML_PIPELINE_ELEMENT_SWITCH_INPUT = 0x8,
  ML_PIPELINE_ELEMENT_SWITCH_OUTPUT = 0x9,
  ML_PIPELINE_ELEMENT_COMMON = 0xB,
} ml_pipeline_element_e;

/**
 * @brief Internal private representation of pipeline handle.
 */
typedef struct _ml_pipeline ml_pipeline;

/**
 * @brief An element that may be controlled individually in a pipeline.
 */
typedef struct _ml_pipeline_element {
  GstElement *element; /**< The Sink/Src/Valve/Switch element */
  ml_pipeline *pipe; /**< The main pipeline */
  char *name;
  ml_pipeline_element_e type;
  GstPad *src;
  GstPad *sink; /**< Unref this at destroy */
  ml_tensors_info_s tensors_info;
  size_t size;

  GList *handles;
  int maxid; /**< to allocate id for each handle */
  gulong handle_id;

  GMutex lock; /**< Lock for internal values */
  gboolean is_media_stream;
  gboolean is_flexible_tensor;

  ml_handle_destroy_cb custom_destroy;
  gpointer custom_data;
} ml_pipeline_element;

/**
 * @brief Internal data structure for the pipeline state callback.
 */
typedef struct {
  ml_pipeline_state_cb cb; /**< Callback to notify the change of pipeline state */
  void *user_data; /**< The user data passed when calling the state change callback */
} pipeline_state_cb_s;

/**
 * @brief Internal data structure for the resource.
 */
typedef struct {
  gchar *type; /**< resource type */
  gpointer handle; /**< pointer to resource handle */
} pipeline_resource_s;

/**
 * @brief Internal private representation of pipeline handle.
 * @details This should not be exposed to applications
 */
struct _ml_pipeline {
  GstElement *element;            /**< The pipeline itself (GstPipeline) */
  GstBus *bus;                    /**< The bus of the pipeline */
  gulong signal_msg;              /**< The message signal (connected to bus) */
  GMutex lock;                    /**< Lock for pipeline operations */
  gboolean isEOS;                 /**< The pipeline is EOS state */
  ml_pipeline_state_e pipe_state; /**< The state of pipeline */
  GHashTable *namednodes;         /**< hash table of "element"s. */
  GHashTable *resources;          /**< hash table of resources to construct the pipeline */
  pipeline_state_cb_s state_cb;   /**< Callback to notify the change of pipeline state */
};

/**
 * @brief Internal private representation sink callback function for GstTensorSink and GstAppSink
 * @details This represents a single instance of callback registration. This should not be exposed to applications.
 */
typedef struct {
  ml_pipeline_sink_cb sink_cb;
  ml_pipeline_src_callbacks_s src_cb;
  void *pdata;
} callback_info_s;

/**
 * @brief Internal private representation of common element handle (All GstElement except AppSink and TensorSink)
 * @details This represents a single instance of registration. This should not be exposed to applications.
 */
typedef struct _ml_pipeline_common_elem {
  ml_pipeline *pipe;
  ml_pipeline_element *element;
  guint32 id;
  callback_info_s *callback_info;   /**< Callback function information. If element is not GstTensorSink or GstAppSink, then it should be NULL. */
} ml_pipeline_common_elem;


/**
 * @brief Macro to check the availability of given NNFW.
 */
#define _ml_nnfw_is_available(f,h) ({bool a; (ml_check_nnfw_availability ((f), (h), &a) == ML_ERROR_NONE && a);})

/**
 * @brief Macro to check the availability of given element.
 */
#define _ml_element_is_available(e) ({bool a; (ml_check_element_availability ((e), &a) == ML_ERROR_NONE && a);})

/**
 * @brief Allocates a tensors information handle from gst info.
 */
int _ml_tensors_info_create_from_gst (ml_tensors_info_h *ml_info, GstTensorsInfo *gst_info);

/**
 * @brief Copies tensor metadata from gst tensors info.
 */
void _ml_tensors_info_copy_from_gst (ml_tensors_info_s *ml_info, const GstTensorsInfo *gst_info);

/**
 * @brief Copies tensor metadata from ml tensors info.
 */
void _ml_tensors_info_copy_from_ml (GstTensorsInfo *gst_info, const ml_tensors_info_s *ml_info);

/**
 * @brief Internal function to get the sub-plugin name.
 */
const char * _ml_get_nnfw_subplugin_name (ml_nnfw_type_e nnfw);

/**
 * @brief Convert c-api based hw to internal representation
 */
accl_hw _ml_nnfw_to_accl_hw (const ml_nnfw_hw_e hw);

/**
 * @brief Internal function to get the nnfw type.
 */
ml_nnfw_type_e _ml_get_nnfw_type_by_subplugin_name (const char *name);

/**
 * @brief Initializes the GStreamer library. This is internal function.
 */
int _ml_initialize_gstreamer (void);

/**
 * @brief Validates the nnfw model file. (Internal only)
 * @since_tizen 5.5
 * @param[in] model List of model file paths.
 * @param[in] num_models The number of model files. There are a few frameworks that require multiple model files for a single model.
 * @param[in/out] nnfw The type of NNFW.
 * @return @c 0 on success. Otherwise a negative error value.
 * @retval #ML_ERROR_NONE Successful
 * @retval #ML_ERROR_NOT_SUPPORTED Not supported.
 * @retval #ML_ERROR_INVALID_PARAMETER Given parameter is invalid.
 */
int _ml_validate_model_file (const char * const *model, const unsigned int num_models, ml_nnfw_type_e * nnfw);

/**
 * @brief Checks the availability of the plugin.
 */
int _ml_check_plugin_availability (const char *plugin_name, const char *element_name);

/**
 * @brief Internal function to convert accelerator as tensor_filter property format.
 * @note returned value must be freed by the caller
 */
char* _ml_nnfw_to_str_prop (ml_nnfw_hw_e hw);

/**
 * @brief Gets the element of pipeline itself (GstElement).
 * @details With the returned reference, you can use GStreamer functions to handle the element in pipeline.
 *          Note that caller should release the returned reference using gst_object_unref().
 * @return The reference of pipeline itself. Null if the pipeline is not constructed or closed.
 */
GstElement* _ml_pipeline_get_gst_element (ml_pipeline_h pipe);

#if defined (__TIZEN__)
/****** TIZEN PRIVILEGE CHECK BEGINS ******/
/**
 * @brief Releases the resource handle of Tizen.
 */
void _ml_tizen_release_resource (gpointer handle, const gchar * res_type);

/**
 * @brief Gets the resource handle of Tizen.
 */
int _ml_tizen_get_resource (ml_pipeline_h pipe, const gchar * res_type);

/**
 * @brief Converts predefined element for Tizen.
 */
int _ml_tizen_convert_element (ml_pipeline_h pipe, gchar ** result, gboolean is_internal);
/****** TIZEN PRIVILEGE CHECK ENDS ******/
#endif /* __TIZEN */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ML_API_INTERNAL_NNSTREAMER_H__ */
