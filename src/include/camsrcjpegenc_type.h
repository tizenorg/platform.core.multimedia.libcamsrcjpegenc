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

#ifndef __CAMSRCJPEGENC_TYPE_H__
#define __CAMSRCJPEGENC_TYPE_H__

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Definitions */
/* Error */
#define CAMSRC_JPEGENC_ERROR_NONE                  0x00000000
#define CAMSRC_JPEGENC_ERROR_NULL_POINTER          0x80000001
#define CAMSRC_JPEGENC_ERROR_ALLOCATION            0x80000002
#define CAMSRC_JPEGENC_ERROR_NOT_SUPPORT           0x80000003
#define CAMSRC_JPEGENC_ERROR_INVALID_PARAMETER     0x80000004
#define CAMSRC_JPEGENC_ERROR_MODULE_OPEN           0x80000005
#define CAMSRC_JPEGENC_ERROR_FIND_SYMBOL           0x80000006
#define CAMSRC_JPEGENC_ERROR_ENCODE_FAIL           0x80000007


/* Enumerations */
typedef enum
{
	JPEG_ENCODER_SOFTWARE = 0,
	JPEG_ENCODER_HARDWARE,
	JPEG_ENCODER_NUM,
} JPEGEncoderType;

typedef enum
{
	JPEG_MODE_BASELINE = 0,
	JPEG_MODE_PROGRESSIVE,
	JPEG_MODE_NUM,
} JPEGModeType;

typedef enum
{
	COLOR_FORMAT_NOT_SUPPORT = -1,
	COLOR_FORMAT_I420        = 0,
	COLOR_FORMAT_YUYV,
	COLOR_FORMAT_UYVY,
	COLOR_FORMAT_RGBA,
	COLOR_FORMAT_ARGB,
	COLOR_FORMAT_RGB,
	COLOR_FORMAT_NV12,
	COLOR_FORMAT_NV21,
	COLOR_FORMAT_NV16,
	COLOR_FORMAT_NUM,
} ColorFormatType;

typedef enum
{
	MEMORY_ADDRESS_NONE      = -1,
	MEMORY_ADDRESS_VIRTUAL   = 0,
	MEMORY_ADDRESS_PHYSICAL,
	MEMORY_ADDRESS_NUM,
} MemoryAddressType;

/* Structures */
typedef struct _jpegenc_internal_info jpegenc_internal_info;
struct _jpegenc_internal_info
{
	int                 version;
	MemoryAddressType   mem_addr_type;
	ColorFormatType     input_fmt_list[COLOR_FORMAT_NUM];
	int                 input_fmt_num;
	ColorFormatType     input_fmt_recommend;
	gboolean            progressive_mode_support;
};

typedef struct _jpegenc_info jpegenc_info;
struct _jpegenc_info
{
	gboolean                 sw_support;
	jpegenc_internal_info    sw_enc;
	gboolean                 hw_support;
	jpegenc_internal_info    hw_enc;
};

typedef struct _jpegenc_parameter jpegenc_parameter;
struct _jpegenc_parameter
{
	guint             width;
	guint             height;
	unsigned char    *src_data;
	ColorFormatType   src_fmt;
	guint             src_len;
	unsigned char    *result_data;
	guint             result_len;
	JPEGModeType      jpeg_mode;
	guint             jpeg_quality;
};


#ifdef __cplusplus
}
#endif

#endif /*__CAMSRCJPEGENC_TYPE_H__*/
