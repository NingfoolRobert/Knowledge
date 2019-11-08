#include <zmq.hpp>
//#include "zhelper.h"
#include "czmq.h"

using namespace zmq;
static void subscriber_thread(void* args, zctx_t *ctx, void* pipe)
{
	void* subscriber = zsocket_new(ctx, ZMQ_SUB);
	zsocket_connect(subscriber, "tcp://localhost:6001");
	zsocket_set_subscribe(subscriber, "A");
	zsocket_set_subscribe(subscriber, "B");

	int nCount = 0;
	while(nCount < 5)
	{
		char* str = zstr_recv(subscriber);
		if(!str)
			break;
		free(str);
		nCount++;
	}
	zsocket_destroy(ctx,subscriber);
}

static void publisher_thread(void* args, zctx_t *ctx, void* pipe)
{
	void* publisher = zsocket_new(ctx, ZMQ_PUB);
	zsocket_bind(ctx, "tcp://*:6000");
	
	while(!zctx_interrupted)
	{
		char str[10];
		sprintf(str, "%c-%05d",randof(10) + 'A', randof(100000));
		if(zstr_send(publisher, str) == -1)
			break;
		zclock_sleep(100);
	}
}

static void listener_thread(void* args, zctx_t* ctx, void* pipe)
{
	while(true)
	{
		zframe_t *frame = zframe_recv(pipe);
		if(!frame)
			break;
		zframe_print(frame, NULL);
		zframe_destroy(&frame);
	}
}


int main()
{
	zctx_t *ctx = ztx_new();
	zthread_fork(ctx, publisher_thread, NULL);
	zthread_fork(ctx,subscriber_thread, NULL);
	void * subscriber = zsocket_new(ctx, ZMQ_XSUB);
	zsocket_connect(subscriber, "tcp://localhost:6000");

	void* publisher = zsocket_new(ctx, ZMQ_XPUB);
	zsocket_connect(publisher, "tcp://*:6001");


	void * listener = zthread_fork(ctx, listener_thread, NULL);
	zmq_proxy(subscriber, publisher, listener);

	puts("interrupted");
	zctx_destroyed(&ctx);
	return 0;
}
