//
// Created by aurora on 6/18/19.
//

#include "SubWorker.h"
#include "CICCLog.h"
#include <set>
//#include <MDS_R/Test/TestHelper.h>
#include "HandlerFactory.h"

using namespace CICCTools;

SubWorker::SubWorker(string g) : l_subCount(0), str_group(g) {

}

SubWorker::~SubWorker() {

}

void SubWorker::Init(string group, string handler, string type) {
    str_group = group;
    str_handler = handler;
    str_type = type;

    m_sub=zsock_new(ZMQ_SUB);
    if(!m_sub)
    {
        MYLOG.Error("[E]create sub failed");
        return;
    }
    zsock_set_tcp_keepalive(m_sub,1);
    zsock_set_tcp_keepalive_cnt(m_sub, 3);
    zsock_set_tcp_keepalive_idle(m_sub, 60);
    zsock_set_tcp_keepalive_intvl(m_sub, 10);

    zsock_set_rcvbuf(m_sub, 64 * 1024 * 1024);
    zsock_set_sndbuf(m_sub, 64 * 1024 * 1024);
    zsock_set_rcvhwm(m_sub, 0);
    zsock_set_sndhwm(m_sub, 0);

    p_handler = HandlerFactory::GetInstance()->CreateHandler(str_group, str_handler, str_type);
}

void SubWorker::AddNewSource(string id, string uri, string filter) {
    string key = uri + filter;
    if(map_subSource.find(key)==map_subSource.end()){
        MDS_R_Sub_Source *source = new MDS_R_Sub_Source();
        source->id = id;
        source->uri = uri;
        source->filter = filter;
        map_subSource.insert(make_pair(key, source));
    }else{
        MYLOG.Error("sub key is already exist");
    }
}

void SubWorker::Start() {
    set<string> uris;
    for (auto item = map_subSource.begin(); item != map_subSource.end(); item++) {
        if(uris.count(item->second->uri) > 0){
            MYLOG.Info("[C] %s already connected", item->second->uri.c_str());
            zsock_set_subscribe(m_sub, item->second->filter.c_str());
        }else{
            int ret=zsock_connect(m_sub,item->second->uri.c_str());
            if(ret!=0)
            {
                MYLOG.Error("[E]connect %s error",item->second->uri.c_str());
            }
            zsock_set_subscribe(m_sub,item->second->filter.c_str());
        }
    }

    p_handler->Start();

    thread t1(SubThread,this);
    t1.detach();
}

bool SubWorker::IsStartSub() {
    return b_startSub;
}

void SubWorker::SubThread(SubWorker *worker) {
    while (true)
    {
        zmsg_t *msg=zmsg_recv(worker->m_sub);
        ++(worker->l_subCount);

        if(msg!= nullptr)
        {
//            MYTEST.LogDownTopic(msg);
            worker->p_handler->Push(msg);
        }
        else
        {
            continue;
        }
    }
}

long SubWorker::GetSubCount() {
    return l_subCount;
}

string SubWorker::GetGroupName() {
    return str_group;
}


