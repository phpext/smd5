/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/json/php_json.h"
#include "php_smd5.h"

ZEND_DECLARE_MODULE_GLOBALS(smd5)

/* True global resources - no need for thread safety here */
static int le_smd5;

/* {{{ smd5_functions[]
 *
 * Every user visible function must have an entry in smd5_functions[].
 */
const zend_function_entry smd5_functions[] = {
	PHP_FE(md5_encrypt,	NULL)
	PHP_FE(md5_decrypt,	NULL)
	PHP_FE_END	/* Must be the last line in smd5_functions[] */
};
/* }}} */


#ifdef COMPILE_DL_SMD5
ZEND_GET_MODULE(smd5)
#endif

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    //STD_PHP_INI_ENTRY("smd5.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_smd5_globals, smd5_globals)
    STD_PHP_INI_ENTRY("smd5.encrypt_url", "http://tool.zzblo.com/api/md5/encrypt", PHP_INI_ALL, OnUpdateString, encrypt_url, zend_smd5_globals, smd5_globals)
    STD_PHP_INI_ENTRY("smd5.decrypt_url", "http://tool.zzblo.com/api/md5/decrypt", PHP_INI_ALL, OnUpdateString, decrypt_url, zend_smd5_globals, smd5_globals)
PHP_INI_END()
/* }}} */


/* {{{ php_smd5_init_globals
 *
static void php_smd5_init_globals(zend_smd5_globals *smd5_globals)
{
    //初始化 init变量其实也可以 在GINIT中设置
}
 }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(smd5)
{
    //ZEND_INIT_MODULE_GLOBALS(smd5, php_smd5_init_globals, NULL);
    REGISTER_INI_ENTRIES();
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(smd5)
{
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(smd5)
{
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(smd5)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(smd5)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "smd5 support", "enabled");
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}
/* }}} */

/**
 * {{{ PHP_GINIT_FUNCTION
 */
PHP_GINIT_FUNCTION(smd5){
//    smd5_globals->global_value=1;
//    smd5_globals->global_string="string...";
};
/**
 * }}}
 */

size_t memory_callback(void *data, size_t size, size_t nmemb, void *user)
{
    size_t realsize = size * nmemb;
    DownloadContent *content = (DownloadContent *)user;

    content->text = erealloc(content->text, content->size + realsize + 1);
    if (content->text == NULL) 
    {
#if defined DEBUG_MODE
        /* out of memory! */ 
        fprintf(stderr, "not enough memory (realloc returned NULL)\n");
#endif
        //exit(EXIT_FAILURE);
        return -1;
    }

    memcpy(&(content->text[content->size]), data, realsize);
    content->size += realsize;
    content->text[content->size] = '\0';

    return realsize;
}


int post_content(char *url,char *postfields, DownloadContent *content)
{
    CURL *curl_handle;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2)");
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postfields);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, memory_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)content);

    res = curl_easy_perform(curl_handle);

    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    return res;
}


/* {{{ proto string md5_encrypt(string string)
   ; */
PHP_FUNCTION(md5_encrypt)
{
	char *param = NULL;
	int argc = ZEND_NUM_ARGS();
	int param_len;
    int ret_code;    
	char *post_string=NULL;
    zend_bool assoc=1;    
    zval *ret_array; 
    zval **ppstatus;

    if (zend_parse_parameters(argc TSRMLS_CC, "s", &param, &param_len) == FAILURE) 
		return;
       
    DownloadContent content;
    content.text=NULL;
    content.size=0;
 
    //连接请求参数 text=123123
    int text_len=5;//strlen("text=");
    int post_len=text_len+param_len;
    post_string=emalloc(post_len+1);
    memcpy(post_string,"text=",text_len);
    memcpy(post_string+text_len,param,param_len);
    post_string[post_len]='\0';
    
    ret_code=post_content(SMD5_G(encrypt_url),post_string,&content);
    if(ret_code!=0){
        efree(content.text);
	    efree(post_string);
        php_error(E_WARNING, "md5_encrypt: can not post the encrypt_url");
        return ; 
    }
    //php_printf(content.text);
    MAKE_STD_ZVAL(ret_array);
    #if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 3))
        php_json_decode(ret_array,content.text,content.size,assoc TSRMLS_CC);    
    #else
        php_json_decode(ret_array,content.text,content.size,assoc,512 TSRMLS_CC);    
    #endif
    if(zend_hash_find(Z_ARRVAL_P(ret_array),ZEND_STRS("secret32"),(void**)&ppstatus)!=FAILURE){
        //php_printf(Z_STRVAL_PP(ppstatus));
        RETVAL_STRING(Z_STRVAL_PP(ppstatus),1);
    }else{
        php_error(E_WARNING, "md5_encrypt: can not get the secret 32");
    }
    //RETVAL_ZVAL(ret_array,1,1);  //注意后面的参数  copy dtoc 
    zval_ptr_dtor(&ret_array);
    efree(content.text);
    efree(post_string);
    post_string=NULL;
    content.text=NULL;
}
/* }}} */

/* {{{ proto string md5_decrypt(string secret)
   ; */
PHP_FUNCTION(md5_decrypt)
{
	char *param = NULL;
	int argc = ZEND_NUM_ARGS();
	int param_len;
    int ret_code;    
	char *post_string=NULL;
    zend_bool assoc=1;    
    zval *ret_array; 
    zval **pptext;

    if (zend_parse_parameters(argc TSRMLS_CC, "s", &param, &param_len) == FAILURE) 
		return;
       
    DownloadContent content;
    content.text=NULL;
    content.size=0;
 
    //连接请求参数 secret=bc138a5d21ffd54ce525033053a927e6
    int text_len=7;//strlen("secret=");
    int post_len=text_len+param_len;
    post_string=emalloc(post_len+1);
    memcpy(post_string,"secret=",text_len);
    memcpy(post_string+text_len,param,param_len);
    post_string[post_len]='\0';
    
    ret_code=post_content(SMD5_G(decrypt_url),post_string,&content);
    if(ret_code!=0){
        efree(content.text);
	    efree(post_string);
        php_error(E_WARNING, "md5_decrypt: can not post the decrypt_url");
        return ; 
    }
    //php_printf(content.text);
    MAKE_STD_ZVAL(ret_array);
    #if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION < 3))
        php_json_decode(ret_array,content.text,content.size,assoc TSRMLS_CC);    
    #else
        php_json_decode(ret_array,content.text,content.size,assoc,512 TSRMLS_CC);    
    #endif
    if(zend_hash_find(Z_ARRVAL_P(ret_array),ZEND_STRS("text"),(void**)&pptext)!=FAILURE){
        RETVAL_STRING(Z_STRVAL_PP(pptext),1);
    }else{
        php_error(E_WARNING, "md5_decrypt: can not decrypt");
    }
    zval_ptr_dtor(&ret_array);
    efree(content.text);
    efree(post_string);
    post_string=NULL;
    content.text=NULL;
};
/* }}} */

/* {{{ smd5_module_entry
*/
zend_module_entry smd5_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "smd5",
    smd5_functions,
    PHP_MINIT(smd5),
    PHP_MSHUTDOWN(smd5),
    PHP_RINIT(smd5),		/* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(smd5),	/* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(smd5),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_SMD5_VERSION,
#endif
    PHP_MODULE_GLOBALS(smd5),
    PHP_GINIT(smd5),
    NULL,
    NULL,
    STANDARD_MODULE_PROPERTIES_EX //注意这里 不是之前的STANDARD_MODULE_PROPERTIES 具体可查看宏
};
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
