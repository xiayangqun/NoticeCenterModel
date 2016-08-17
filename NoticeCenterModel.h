/*
//  NoticeCenterModel.hpp
//  Puzzle
//
//  Created by xiayangqun on 16/8/17.
//
//
//
    a universal notification center , you can use it to post any type data . like int ,float or std::string.
    but you add Oberser for a std::string type ,when it post other type data ,you will not recive any notification.
    every type noticfication center are Independent of each other.
*/

#ifndef NoticeCenterModel_hpp
#define NoticeCenterModel_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

template <class T>
class NoticeCenterModel
{
public:
    
    static NoticeCenterModel<T> * getInstance();
 
    ~NoticeCenterModel();
    
    void addOberserver(const std::string& key , Ref * target,  const std::function<void(T)>& oberserver);
    void removeOberserver(const std::string& key , Ref * target);
    void postNoticate(const std::string& key,  T data );
    
protected:
    
    struct NoticeOberser
    {
        Ref * _target;
        std::function<void(T)> _callBack;
        
        void active(T data);
        NoticeOberser(const Ref *& target, const std::function<void(T)>& callBack);
    };
    
protected:
    NoticeCenterModel();
    static NoticeCenterModel<T> * instance;
    std::map<std::string , std::vector<NoticeOberser *> > OberserverMap;
    
};



/*impl */

template <class T>
void NoticeCenterModel<T>::NoticeOberser::active(T data)
{
    _callBack(data);
}

template <class T>
NoticeCenterModel<T>::NoticeOberser::NoticeOberser(const Ref *& target, const std::function<void(T)>& callBack)
{
    _target=target;
    _callBack=callBack;
}


template <class T>
NoticeCenterModel<T>::NoticeCenterModel(){}

template <class T>
NoticeCenterModel<T>::~NoticeCenterModel(){}

template <class T>
NoticeCenterModel<T> * NoticeCenterModel<T>::instance=nullptr;

template <class T>
NoticeCenterModel<T> * NoticeCenterModel<T>::getInstance()
{
    if(instance==nullptr)
        instance=new NoticeCenterModel<T>();
    
    return instance;

}


template <class T>
void NoticeCenterModel<T>::addOberserver(const std::string& key , Ref * target,  const std::function<void(T)>& oberserver)
{
    auto ober=new NoticeOberser(target, oberserver);
    auto& oberserverVec=OberserverMap[key];
    oberserverVec.push_back(ober);
}

template <class T>
void NoticeCenterModel<T>::removeOberserver(const std::string& key , Ref * target)
{
    if(OberserverMap.find(key)==OberserverMap.end())
        return;
    
    
    auto & oberservers=OberserverMap.at(key);
    for(auto itor=oberservers.begin  ;  itor!=oberservers.end() ; )
    {
        if(  (*itor)->_target==target )
        {
            delete (*itor);
            itor=oberservers.erase(itor);
        }
        else
        {
            itor++;
        }
    }
}


template <class T>
void  NoticeCenterModel<T>::postNoticate(const std::string& key,  T data)
{
    if(OberserverMap.find(key)==OberserverMap.end())
        return;
    
    for(auto& oberserver: OberserverMap.at(key))
        oberserver->active(data);
}

typedef NoticeCenterModel<int>     NoticeCenterModelInt;
typedef NoticeCenterModel<float>  NoticeCenterModelFloat;
typedef NoticeCenterModel<const std::string& > NoticeCenterModelString;


#endif /* NoticeCenterModel_hpp */
