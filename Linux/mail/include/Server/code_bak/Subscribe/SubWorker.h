//
// Created by aurora on 6/18/19.
//

#ifndef MDS_SUBWORKER_H
#define MDS_SUBWORKER_H

#include "Def.h"
#include "BaseHandler.h"
#include <string>
#include <map>
#include "czmq.h"

using namespace std;

class SubWorker {

public:
    SubWorker(string);

    ~SubWorker();

    void Init(string group, string handler, string type);

    void AddNewSource(string id, string uri, string filter);

    void Start();

    bool IsStartSub();

    long GetSubCount();

    string GetGroupName();

public:
    static void SubThread(SubWorker *);

private:

    void StartHandler();

    BaseHandler *p_handler;

    string str_group;

    string str_handler;

    string str_type;

    map<string, MDS_R_Sub_Source*> map_subSource;

    zsock_t* m_sub= nullptr;

    bool b_startSub;

    long l_subCount;

};


#endif //MDS_SUBWORKER_H
