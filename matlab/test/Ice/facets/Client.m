%{
**********************************************************************

Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.

This copy of Ice is licensed to you under the terms described in the
ICE_LICENSE file included in this distribution.

**********************************************************************
%}

classdef Client < Application
    methods
        function r = run(obj, args)
            g = AllTests.allTests(obj);
            g.shutdown();
            r = 0;
        end
    end
    methods(Access=protected)
        function [r, remArgs] = getInitData(obj, args)
            [initData, remArgs] = getInitData@Application(obj, args);
            initData.properties_.setProperty('Ice.Package.Test', 'test.Ice.facets');
            r = initData;
        end
    end
    methods(Static)
        function status = start(args)
            addpath('generated');
            if ~libisloaded('icematlab')
                loadlibrary('icematlab', 'icematlab_proto')
            end
            c = Client();
            status = c.main('Client', args);
        end
    end
end