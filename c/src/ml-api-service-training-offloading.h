/* SPDX-License-Identifier: Apache-2.0 */
/**
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved.
 *
 * @file ml-api-service-training-offloading.h
 * @date 30 Apr 2024
 * @brief ml-service extension internal header.
 *        This file should NOT be exported to SDK or devel package.
 * @brief ML training offloading service of NNStreamer/Service C-API
 * @see https://github.com/nnstreamer/nnstreamer
 * @author Hyunil Park <hyunil46.park@samsung.com>
 * @bug No known bugs except for NYI items
 */

#ifndef __ML_SERVICE_TRAINING_OFFLOADING_H__
#define __ML_SERVICE_TRAINING_OFFLOADING_H__

#include <ml-api-service.h>
#include "nnstreamer-tizen-internal.h"
#include "ml-api-service-offloading.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @brief Creates a training offloading handle for ml-service training offloading service.
 * @param[in] mls ml-service handle created by ml_service_new().
 * @param[in] object The Json object containing the service option.
 * @return @c 0 on success. Otherwise a negative error value.
 * @retval #ML_ERROR_NONE Successful.
 * @retval #ML_ERROR_NOT_SUPPORTED Not supported.
 * @retval #ML_ERROR_PERMISSION_DENIED The application does not have the privilege to access to the media storage or external storage.
 * @retval #ML_ERROR_INVALID_PARAMETER The parameter is invalid.
 * @retval #ML_ERROR_IO_ERROR Failed to parse the configuration file.
 * @retval #ML_ERROR_STREAMS_PIPE Failed to open the model.
 * @retval #ML_ERROR_OUT_OF_MEMORY Failed to allocate required memory.
 */
  int ml_service_training_offloading_create (ml_service_s * mls, JsonObject * object);

/**
 * @brief Set path in ml-service training offloading handle.
 * @note This is not official and public API but experimental API.
 * @param[in] mls ml-service handle created by ml_service_new().
 * @param[in] path Readable and writable path set by the app.
 * @return @c 0 on success. Otherwise a negative error value.
 * @retval #ML_ERROR_NONE Successful.
 * @retval #ML_ERROR_NOT_SUPPORTED Not supported.
 * @retval #ML_ERROR_INVALID_PARAMETER Given parameter is invalid.
 */
  int ml_service_training_offloading_set_path (ml_service_s *mls, const gchar * path);

/**
 * @brief Start ml training offloading service.
 * @remarks The @a handle should be destroyed using ml_service_destroy().
 * @param[in] mls ml-service handle created by ml_service_new().
 * @return @c 0 on Success. Otherwise a negative error value.
 * @retval #ML_ERROR_NONE Successful.
 * @retval #ML_ERROR_NOT_SUPPORTED Not supported.
 * @retval #ML_ERROR_INVALID_PARAMETER Fail. The parameter is invalid.
 * @retval #ML_ERROR_OUT_OF_MEMORY Failed to allocate required memory.
 * @retval #ML_ERROR_STREAMS_PIPE Failed to launch the pipeline.
 * @retval #ML_ERROR_TRY_AGAIN The pipeline is not ready yet.
 * @retval #ML_ERROR_PERMISSION_DENIED The application does not have the privilege to access to the storage.
 */
  int ml_service_training_offloading_start (ml_service_s * mls);

/**
 * @brief Stop ml training offloading service.
 * @remarks The @a handle should be destroyed using ml_service_destroy().
 * @param[in] mls ml-service handle created by ml_service_new().
 * @return @c 0 on Success. Otherwise a negative error value.
 * @retval #ML_ERROR_NONE Successful.
 * @retval #ML_ERROR_NOT_SUPPORTED Not supported.
 * @retval #ML_ERROR_INVALID_PARAMETER Fail. The parameter is invalid.
 * @retval #ML_ERROR_OUT_OF_MEMORY Failed to allocate required memory.
 * @retval #ML_ERROR_STREAMS_PIPE Failed to launch the pipeline.
 * @retval #ML_ERROR_TRY_AGAIN The pipeline is not ready yet.
 * @retval #ML_ERROR_PERMISSION_DENIED The application does not have the privilege to access to the storage.
 */
  int ml_service_training_offloading_stop (ml_service_s * mls);

/**
 * @brief Request all services to ml-service offloading.
 * @param[in] mls ml-service handle created by ml_service_new().
 * @return @c 0 on success. Otherwise a negative error value.
 * @retval #ML_ERROR_NONE Successful.
 * @retval #ML_ERROR_NOT_SUPPORTED Not supported.
 * @retval #ML_ERROR_INVALID_PARAMETER Given parameter is invalid.
 * @return @c 0 on success. Otherwise a negative error value.
 * @retval #ML_ERROR_NONE Successful.
 * @retval #ML_ERROR_NOT_SUPPORTED Not supported.
 * @retval #ML_ERROR_INVALID_PARAMETER Given parameter is invalid.
 */
  int ml_service_training_offloading_all_services_request (ml_service_s * mls);

/**
 * @brief Process received data
 * @param[in] mls ml-service handle created by ml_service_new().
 * @param[in] data_h handle nns_edge_data_h
 * @param[in] data data of received file
 * @param[in] dir_path dir path
 * @param[in] service_type received service type from remote edge
 */
  void ml_service_training_offloading_process_received_data (ml_service_s * mls, void * data_h, const gchar *dir_path, const gchar * data, int service_type);

/**
 * @brief Internal function to destroy ml-service training offloading data.
 * @param[in] mls ml-service handle created by ml_service_new().
 * @return @c 0 on success. Otherwise a negative error value.
 * @retval #ML_ERROR_NONE Successful.
 * @retval #ML_ERROR_NOT_SUPPORTED Not supported.
 * @retval #ML_ERROR_INVALID_PARAMETER Given parameter is invalid.
 * @return @c 0 on success. Otherwise a negative error value.
 * @retval #ML_ERROR_NONE Successful.
 * @retval #ML_ERROR_NOT_SUPPORTED Not supported.
 * @retval #ML_ERROR_INVALID_PARAMETER Given parameter is invalid.
 */
  int ml_service_training_offloading_destroy (ml_service_s * mls);
#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __ML_SERVICE_TRAINING_OFFLOADING_H__ */
