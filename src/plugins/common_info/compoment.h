/**
 *
 *  game_config.h
 *
 */

#pragma once
#include <functional>
#include <map>

#include "drogon/utils/coroutine.h"

template <typename TBaseComp>
class CompCenter;

template <typename TBaseComp>
class IComponent
{
public:
    IComponent(CompCenter<TBaseComp>* cen)
        : center(cen)
    {
        
    };

    virtual ~IComponent()
    {
        
    }
    
public:    
    template<typename TComp, typename = std::enable_if_t< std::is_base_of_v<TBaseComp, TComp>>>
    TComp* Get()
    {
        if (nullptr == center)
        {
            return nullptr;
        }

        return center->Get<TComp>();
    }

private:
    CompCenter<TBaseComp>* center = nullptr;
};


template <typename TBaseComp>
class CompCenter
{

public:
    ~CompCenter()
    {
        for (auto& pair: Comps)
        {
            delete pair.second;
        }
        Comps.clear();
    }
    
public:

    template<typename TComp, typename = std::enable_if_t< std::is_base_of_v<TBaseComp, TComp>>,
             typename... Args>
    bool AddComp(Args... args)
    {
        auto& typeAddress = typeid(TComp);
        
        auto it = Comps.find(&typeAddress);
        if (it != Comps.end())
        {
            return false;
        }

        auto newComp = new TComp(args...);
        Comps[&typeAddress] = newComp;
        return true;        
    }
    
    template<typename TComp, typename = std::enable_if_t< std::is_base_of_v<TBaseComp, TComp>>>
    TComp* Get()
    {
        auto& typeAddress = typeid(TComp);
        auto iter = Comps.find(&typeAddress);
        if (iter != Comps.end())
        {
            return (TComp*)iter->second;
        }
        return nullptr;
    }

    void Foreach(std::function<void(TBaseComp*)> func)
    {
        for (auto& pair : Comps)
        {
            func(pair.second);
        }
    }

    drogon::Task<int> ForeachCo(std::function<drogon::Task<int>(TBaseComp*)> func)
    {
        for (auto& pair : Comps)
        {
            int value = co_await func(pair.second);
            if (value != 0)
            {
                co_return value;
            }
        }

        co_return 0;
    }
    
    
private:
    std::map<const type_info*, TBaseComp*> Comps;
};