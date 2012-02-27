/*
 * libcamsrcjpegenc
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Jeongmo Yang <jm80.yang@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __CAMSRCJPEGENC_H__
#define __CAMSRCJPEGENC_H__

#include <camsrcjpegenc_type.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Get source color format using fourcc
 *
 * @param[in]  fourcc  ::guint32           fourcc of video source
 * @param[out] src_fmt ::ColorFormatType   color format type enum value
 *
 * @return Success on TRUE or returns FALSE
 *
 */
gboolean camsrcjpegenc_get_src_fmt( guint32 fourcc, ColorFormatType *src_fmt );

/**
 * Get information of supported JPEG encoder
 *
 * @param[out] enc_info ::JPEGENC_INFO      information of supported JPEG encoder
 *
 * @return Success on CAMSRC_JPEGENC_ERROR_NONE or returns with ::CAMSRC_JPEGENC_ERROR code
 *
 */
int camsrcjpegenc_get_info( jpegenc_info *enc_info );

/**
 * Encode JPEG image
 *
 * @param[in] enc_info  ::JPEGENC_INFO         structure from camsrcjpegenc_get_info
 * @param[in] enc_param ::JPEGENC_PARAMETER    encoding parameter
 *
 * @return Success on CAMSRC_JPEGENC_ERROR_NONE or returns with ::CAMSRC_JPEGENC_ERROR code
 *
 */
int camsrcjpegenc_encode( jpegenc_info *enc_info, JPEGEncoderType enc_type, jpegenc_parameter *enc_param );

#ifdef __cplusplus
}
#endif

#endif /*__CAMSRCJPEGENC_H__*/
