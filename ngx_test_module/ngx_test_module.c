#include<ngx_config.h>
#include<ngx_core.h>
#include<ngx_http.h>
typedef struct {
	ngx_str_t hello;
}ngx_http_test_conf_loc_t;
static char * set(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_command_t test_commands[] = {
	{
		ngx_string("test"),
		NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS | NGX_CONF_TAKE1,
		set,
		NGX_HTTP_LOC_CONF_OFFSET,
		offsetof(ngx_http_test_conf_loc_t,hello),
		NULL	
	},
	ngx_null_command
};
static void * ngx_http_test_create_loc_conf(ngx_conf_t *cf)
{
	ngx_http_test_conf_loc_t *conf;
	conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_test_conf_loc_t));
	if(conf == NULL){
		return NGX_CONF_ERROR;	
	}
	return conf;
}
static ngx_http_module_t test_ctx= {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	ngx_http_test_create_loc_conf,
	NULL
};

ngx_module_t ngx_http_test_module = {
	NGX_MODULE_V1,
	&test_ctx,
	test_commands,
	NGX_HTTP_MODULE,
	NULL,NULL,NULL,
	NULL,NULL,NULL,NULL,
	NGX_MODULE_V1_PADDING
};
static ngx_int_t handler(ngx_http_request_t *req){
//	u_char html[100] = "test";
	req->headers_out.status = 200;

	ngx_http_test_conf_loc_t *mycnf;
	mycnf = ngx_http_get_module_loc_conf(req,ngx_http_test_module);
	ngx_buf_t *b;
	b = ngx_pcalloc(req->pool, sizeof(ngx_buf_t));
	ngx_chain_t out;
	out.buf = b;
	out.next = NULL;
	b->pos = mycnf->hello.data;
	b->last = mycnf->hello.data+mycnf->hello.len;
	ngx_log_error(NGX_LOG_EMERG, req->connection->log, 0, "hello data");
	//b->pos = html;
	//b->last =html+len; 
	b->memory =1;
	b->last_buf = 1;

	req->headers_out.content_length_n = mycnf->hello.len;
	ngx_str_set(&req->headers_out.content_type,"text/json");
	ngx_http_send_header(req);
	return ngx_http_output_filter(req,&out);


}
static char * set(ngx_conf_t *cf, ngx_command_t *cmd, void *conf){
	ngx_http_core_loc_conf_t *corecf;
	corecf = ngx_http_conf_get_module_loc_conf(cf,ngx_http_core_module);
	corecf->handler = handler;
	ngx_conf_set_str_slot(cf,cmd,conf);
	return NGX_CONF_OK;
}
