// **********************************************************************
//
// Copyright (c) 2003
// ZeroC, Inc.
// Billerica, MA, USA
//
// All Rights Reserved.
//
// Ice is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation.
//
// **********************************************************************

#include <Freeze/StrategyI.h>

using namespace std;
using namespace Ice;
using namespace Freeze;

LocalObjectPtr
Freeze::EvictionStrategyI::activatedObject(const Identity& ident,
                                           const ObjectPtr& servant)
{
    CookiePtr cookie = new Cookie;
    cookie->mutated = false;
    return cookie;
}

void
Freeze::EvictionStrategyI::destroyedObject(const Identity& ident, const LocalObjectPtr& cookie)
{
    // Nothing to do
}

void
Freeze::EvictionStrategyI::evictedObject(const ObjectStorePtr& store,
                                         const Identity& ident,
                                         const ObjectPtr& servant,
                                         const LocalObjectPtr& cookie)
{
    //
    // Only store the object's persistent state if it has been mutated.
    //
    CookiePtr c = CookiePtr::dynamicCast(cookie);
    assert(c);
    if(c->mutated)
    {
        store->save(ident, servant);
        c->mutated = false;
    }
}

void
Freeze::EvictionStrategyI::invokedObject(const ObjectStorePtr& store,
                                         const Identity& ident,
                                         const ObjectPtr& servant,
                                         bool mutating,
                                         bool idle,
                                         const LocalObjectPtr& cookie)
{
    if(mutating)
    {
        CookiePtr c = CookiePtr::dynamicCast(cookie);
        assert(c);
        c->mutated = true;
    }
}

void
Freeze::EvictionStrategyI::destroy()
{
    // Nothing to do
}

LocalObjectPtr
Freeze::IdleStrategyI::activatedObject(const Identity& ident,
                                       const ObjectPtr& servant)
{
    CookiePtr cookie = new Cookie;
    cookie->mutated = false;
    return cookie;
}

void
Freeze::IdleStrategyI::destroyedObject(const Identity& ident, const LocalObjectPtr& cookie)
{
    // Nothing to do
}

void
Freeze::IdleStrategyI::evictedObject(const ObjectStorePtr& store,
                                        const Identity& ident,
                                        const ObjectPtr& servant,
                                        const LocalObjectPtr& cookie)
{
    //
    // The object must reach the idle state in order for it to be
    // evicted, therefore the object should have already been saved
    // by invokedObject.
    //
    CookiePtr c = CookiePtr::dynamicCast(cookie);
    assert(c);
    assert(!c->mutated);
}

void
Freeze::IdleStrategyI::invokedObject(const ObjectStorePtr& store,
                                     const Identity& ident,
                                     const ObjectPtr& servant,
                                     bool mutating,
                                     bool idle,
                                     const LocalObjectPtr& cookie)
{
    CookiePtr c = CookiePtr::dynamicCast(cookie);
    assert(c);
    if(!idle && mutating)
    {
        c->mutated = true;
    }
    else if(idle && (mutating || c->mutated))
    {
        //
        // Only store the object's persistent state if the object is idle
        // and it has been mutated.
        //
        store->save(ident, servant);
        c->mutated = false;
    }
}

void
Freeze::IdleStrategyI::destroy()
{
    // Nothing to do
}
