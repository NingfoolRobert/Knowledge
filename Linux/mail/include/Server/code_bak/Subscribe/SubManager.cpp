//
// Created by aurora on 6/18/19.
//

#include "SubManager.h"
#include "MDSRConfigHelper.h"
#include "CICCLog.h"

using namespace CICCTools;

SubManager::SubManager() {}

SubManager::~SubManager() {}

void SubManager::LoadConfig() {
    GrpConfigMap *grpConfigMap = MDSRCONFIG.GetGrpConfigMap();
    if(grpConfigMap!= nullptr){
        for(auto itr = grpConfigMap->begin(); itr!= grpConfigMap->end(); itr++){
            SubWorker* worker;
            if(!CheckWorkerExist(itr->second->grpName)){
                worker = new SubWorker(itr->second->grpName);
                map_Worker.insert(make_pair(itr->second->grpName, worker));
            }else{
                worker = map_Worker.find(itr->second->grpName)->second;
            }
            worker->Init(itr->second->grpName, itr->second->handle, itr->second->type);
            for(auto v = itr->second->items.begin(); v!= itr->second->items.end(); v++){
                worker->AddNewSource(v->sourceId, v->uri, v->filter);
            }
        }
        b_isLoadConfig = true;
    }else{
        MYLOG.Error("[E] grpConfigMap is null");
    }
}

bool SubManager::CheckWorkerExist(string group) {
  if(map_Worker.find(group) == map_Worker.end()){
      return false;
  }else{
      return true;
  }
}

int SubManager::GetWorkerSize() {
    return map_Worker.size();
}

bool SubManager::IsLoadConfig() {
    return b_isLoadConfig;
}

void SubManager::StartSubscribe() {
    try{
        for (auto itr = map_Worker.begin(); itr != map_Worker.end(); itr++) {
            SubWorker *worker = itr->second;
            worker->Start();
        }
        b_isStartSub = true;
    }catch(exception ex){
        MYLOG.Error("[E] %s", ex.what());
        b_isStartSub = false;
    }
}

bool SubManager::IsStartSubscribe() {
    return b_isStartSub;
}

void SubManager::CountSubscribe() {
    for (auto itr = map_Worker.begin(); itr != map_Worker.end(); itr++) {
        SubWorker *worker = itr->second;
        MYLOG.Info("group: %s, count %lld", worker->GetGroupName().c_str(), worker->GetSubCount());;
    }
}
