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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dlog.h>

#include "camsrcjpegenc.h"

#define USE_GMODULE
#ifdef USE_GMODULE
#include <gmodule.h>
#else
#include <dlfcn.h>
#endif

#define PATH_SW_ENCODER_LIB         "/usr/lib/libcamsrcjpegenc-sw.so"
#define PATH_HW_ENCODER_LIB         "/usr/lib/libcamsrcjpegenc-hw.so"
#define MAKE_FOURCC(a,b,c,d)        ((guint32)((a)|(b)<<8|(c)<<16|(d)<<24))


/* Get JPEG encoder information */
typedef gboolean (*GetJPEGEncInfoFunc)  ( jpegenc_internal_info *info );

/* Encode JPEG */
typedef gboolean (*EncodeJPEGFunc)      ( jpegenc_parameter *enc_param );

/* Function name of sub module */
#define GET_JPEGENC_INFO_FUNC_NAME   "camsrcjpegencsub_get_info"
#define ENCODE_JPEG_FUNC_NAME        "camsrcjpegencsub_encode"

/* Debug Log */
#define DLOG_PKG_NAME                              "CAMSRCJPEGENC"
#define camsrc_jpegenc_debug( msg, args... )       SLOG( LOG_DEBUG, DLOG_PKG_NAME, "[INFO] [%05d: %s] "msg"\n", __LINE__, __func__, ##args );
#define camsrc_jpegenc_warning( msg, args... )     SLOG( LOG_WARN,  DLOG_PKG_NAME, "[WARN] [%05d: %s] "msg"\n", __LINE__, __func__, ##args );
#define camsrc_jpegenc_error( msg, args... )       SLOG( LOG_ERROR, DLOG_PKG_NAME, "[ERROR][%05d: %s] "msg"\n", __LINE__, __func__, ##args );


/* Internal Function */
static int _encode_jpeg( char *enc_lib_path, jpegenc_internal_info *enc_detail_info, jpegenc_parameter *enc_param );


static int
_encode_jpeg( char *enc_lib_path, jpegenc_internal_info *info, jpegenc_parameter *enc_param )
{
	int ret = CAMSRC_JPEGENC_ERROR_NONE;
	//unsigned char *src_data = NULL;

#ifdef USE_GMODULE
	GModule *module = NULL;
#else
	void *dl_handle = NULL;
#endif
	EncodeJPEGFunc encode_jpeg_func = NULL;

	if( !enc_lib_path || !info || !enc_param )
	{
		camsrc_jpegenc_error( "enc_lib_path[%p] or info[%p] or enc_param[%p] is NULL",
		                      enc_lib_path, info, enc_param );
		return CAMSRC_JPEGENC_ERROR_NULL_POINTER;
	}

#ifdef USE_GMODULE
	module = g_module_open( enc_lib_path, G_MODULE_BIND_LAZY );
	if( module )
#else
	dl_handle = dlopen( enc_lib_path, RTLD_LAZY );
	if( dl_handle )
#endif
	{
#ifdef USE_GMODULE
		g_module_symbol( module, ENCODE_JPEG_FUNC_NAME, (gpointer*)&encode_jpeg_func );
#else
		encode_jpeg_func = (EncodeJPEGFunc) dlsym( dl_handle, ENCODE_JPEG_FUNC_NAME );
#endif
		if( encode_jpeg_func )
		{
			/*Convert raw data if needed */
			

			if( encode_jpeg_func( enc_param ) )
			{
				camsrc_jpegenc_debug( "JPEG encode OK! - size[%d]", ret, enc_param->result_len );
				ret = CAMSRC_JPEGENC_ERROR_NONE;
			}
			else
			{
				camsrc_jpegenc_error( "JPEG encode Failed" );
				ret = CAMSRC_JPEGENC_ERROR_ENCODE_FAIL;
			}
		}
		else
		{
			camsrc_jpegenc_error( "[%s] find symbol failed", ENCODE_JPEG_FUNC_NAME );
			ret = CAMSRC_JPEGENC_ERROR_FIND_SYMBOL;
		}

#ifdef USE_GMODULE
		g_module_close( module );
		module = NULL;
#else
		dlclose( dl_handle );
		dl_handle = NULL;
#endif
	}
	else
	{
		camsrc_jpegenc_error( "%s module open failed", enc_lib_path );
		ret = CAMSRC_JPEGENC_ERROR_MODULE_OPEN;
	}

	return ret;
}

gboolean
camsrcjpegenc_get_src_fmt( guint32 fourcc, ColorFormatType *src_fmt )
{
	int ret = TRUE;

	if( src_fmt == NULL )
	{
		camsrc_jpegenc_warning( "src_fmt is NULL" );
		return FALSE;
	}

	*src_fmt = COLOR_FORMAT_NOT_SUPPORT;

	switch( fourcc )
	{
		case MAKE_FOURCC('I','4','2','0'):
		case MAKE_FOURCC('S','4','2','0'):
			*src_fmt = COLOR_FORMAT_I420;
			camsrc_jpegenc_debug( "I420" );
			break;

		case MAKE_FOURCC('Y','U','Y','V'):
		case MAKE_FOURCC('S','U','Y','V'):
			*src_fmt = COLOR_FORMAT_YUYV;
			camsrc_jpegenc_debug( "YUYV" );
			break;

		case MAKE_FOURCC('U','Y','V','Y'):
		case MAKE_FOURCC('S','Y','V','Y'):
			*src_fmt = COLOR_FORMAT_UYVY;
			camsrc_jpegenc_debug( "UYVY" );
			break;

		case MAKE_FOURCC('R','G','B','A'):
			*src_fmt = COLOR_FORMAT_RGBA;
			camsrc_jpegenc_debug( "RGBA" );
			break;

		case MAKE_FOURCC('A','R','G','B'):
			*src_fmt = COLOR_FORMAT_ARGB;
			camsrc_jpegenc_debug( "ARGB" );
			break;

		case MAKE_FOURCC('R','G','B',' '):
			*src_fmt = COLOR_FORMAT_RGB;
			camsrc_jpegenc_debug( "RGB" );
			break;

		case MAKE_FOURCC('N','V','1','2'):
		case MAKE_FOURCC('S','N','1','2'):
			*src_fmt = COLOR_FORMAT_NV12;
			camsrc_jpegenc_debug( "NV12" );
			break;

		case MAKE_FOURCC('N','V','2','1'):
			*src_fmt = COLOR_FORMAT_NV21;
			camsrc_jpegenc_debug( "NV21" );
			break;

		case MAKE_FOURCC('N','V','1','6'):
			*src_fmt = COLOR_FORMAT_NV16;
			camsrc_jpegenc_debug( "NV16" );
			break;

		default:
			camsrc_jpegenc_warning( "Can not convert fourcc[%d]", fourcc );
			ret = FALSE;
			break;
	}

	return ret;
}

int
camsrcjpegenc_get_info( jpegenc_info *enc_info )
{
	int i = 0, j = 0, ret = CAMSRC_JPEGENC_ERROR_NONE;
	char *ENCODER_LIB_PATH[JPEG_ENCODER_NUM] = { PATH_SW_ENCODER_LIB, PATH_HW_ENCODER_LIB };

#ifdef USE_GMODULE
	GModule *module = NULL;
#else
	void *dl_handle = NULL;
#endif
	jpegenc_internal_info *info[JPEG_ENCODER_NUM];

	GetJPEGEncInfoFunc get_jpegenc_info_func = NULL;

	camsrc_jpegenc_debug( "ENTER" );

	if( enc_info == NULL )
	{
		camsrc_jpegenc_error( "enc_info pointer is NULL" );
		return CAMSRC_JPEGENC_ERROR_NULL_POINTER;
	}

	info[JPEG_ENCODER_SOFTWARE] = &enc_info->sw_enc;
	info[JPEG_ENCODER_HARDWARE] = &enc_info->hw_enc;

	/* Get SW/HW JPEG encoder information */
	for( i = JPEG_ENCODER_SOFTWARE ; i < JPEG_ENCODER_NUM ; i++ )
	{
		/* Open module */
#ifdef USE_GMODULE
		module = g_module_open( ENCODER_LIB_PATH[i], G_MODULE_BIND_LAZY );
		if( module == NULL )
		{
			/* module open failed */
			camsrc_jpegenc_error( "%s module open failed : %s",
			                      ENCODER_LIB_PATH[i], g_module_error() );
#else
		dl_handle = dlopen( ENCODER_LIB_PATH[i], RTLD_LAZY );
		if( dl_handle == NULL )
		{
			/* module open failed */
			camsrc_jpegenc_error( "%s module open failed : %s",
			                      ENCODER_LIB_PATH[i], dlerror() );
#endif

			if( i == JPEG_ENCODER_SOFTWARE )
				enc_info->sw_support = FALSE;
			else
				enc_info->hw_support = FALSE;

			info[i]->mem_addr_type = MEMORY_ADDRESS_NONE;
			for( j = 0 ; j < COLOR_FORMAT_NUM ; j++ )
			{
				info[i]->input_fmt_list[j] = COLOR_FORMAT_NOT_SUPPORT;
			}
			info[i]->input_fmt_num = 0;
			info[i]->input_fmt_recommend = COLOR_FORMAT_NOT_SUPPORT;
		}
		/* lib open and call the function to get information of JPEG encoder */
		else
		{
#ifdef USE_GMODULE
			g_module_symbol( module, GET_JPEGENC_INFO_FUNC_NAME, (gpointer*)&get_jpegenc_info_func );
#else
			get_jpegenc_info_func = (GetJPEGEncInfoFunc) dlsym( dl_handle, GET_JPEGENC_INFO_FUNC_NAME );
#endif
			if( get_jpegenc_info_func )
			{
				if( i == JPEG_ENCODER_SOFTWARE )
					enc_info->sw_support = TRUE;
				else
					enc_info->hw_support = TRUE;

				get_jpegenc_info_func( info[i] );

				camsrc_jpegenc_debug( "version                  : [%02d]", info[i]->version );
				camsrc_jpegenc_debug( "mem_addr_type            : [%02d]", info[i]->mem_addr_type );
				camsrc_jpegenc_debug( "input_fmt_num            : [%02d]", info[i]->input_fmt_num );
				for( j = 0 ; j < info[i]->input_fmt_num ; j++ )
				{
					camsrc_jpegenc_debug( "- fmt_list[%02d]           : [%02d]", j, info[i]->input_fmt_list[j] );
				}
				camsrc_jpegenc_debug( "input_fmt_recommend      : [%02d]", info[i]->input_fmt_recommend );
				camsrc_jpegenc_debug( "progressive_mode_support : [%02d]", info[i]->progressive_mode_support );
			}
			else
			{
#ifdef USE_GMODULE
				camsrc_jpegenc_error( "%s g_module_symbol failed : %s",
				                      GET_JPEGENC_INFO_FUNC_NAME, g_module_error() );
#else
				camsrc_jpegenc_error( "%s dlsym failed : %s",
				                      GET_JPEGENC_INFO_FUNC_NAME, dlerror() );
#endif

				ret = CAMSRC_JPEGENC_ERROR_FIND_SYMBOL;
			}

			/* Close module */
#ifdef USE_GMODULE
			g_module_close( module );
			module = NULL;
#else
			dlclose( dl_handle );
			dl_handle = NULL;
#endif
		}
	}

	camsrc_jpegenc_debug( "LEAVE" );

	return ret;
}

int
camsrcjpegenc_encode( jpegenc_info *enc_info, JPEGEncoderType enc_type, jpegenc_parameter *enc_param )
{
	int ret = CAMSRC_JPEGENC_ERROR_NONE;

	camsrc_jpegenc_debug( "ENTER" );

	if( enc_info == NULL || enc_param == NULL )
	{
		camsrc_jpegenc_error( "enc_info[%p] or enc_param[%p] is NULL", enc_info, enc_param );
		return CAMSRC_JPEGENC_ERROR_NULL_POINTER;
	}

	/* Check encoder type and that is supported or not */
	switch( enc_type )
	{
		case JPEG_ENCODER_SOFTWARE:
			if( enc_info->sw_support )
			{
				ret = _encode_jpeg( PATH_SW_ENCODER_LIB, &enc_info->sw_enc, enc_param );
			}
			else
			{
				camsrc_jpegenc_error( "JPEG SW encoder is not supported" );
				ret = CAMSRC_JPEGENC_ERROR_NOT_SUPPORT;
			}
			break;

		case JPEG_ENCODER_HARDWARE:
			if( enc_info->hw_support )
			{
				ret = _encode_jpeg( PATH_HW_ENCODER_LIB, &enc_info->hw_enc, enc_param );
			}
			else
			{
				camsrc_jpegenc_error( "JPEG HW encoder is not supported" );
				ret = CAMSRC_JPEGENC_ERROR_NOT_SUPPORT;
			}
			break;

		default:
			camsrc_jpegenc_error( "NOT Supported JPEG encoder Type : %d", enc_type );
			ret = CAMSRC_JPEGENC_ERROR_NOT_SUPPORT;
			break;
	}

	camsrc_jpegenc_debug( "LEAVE" );

	return ret;
}

int main(int argc, char** argv)
{
	return 0;
}
