
QtMoc = Rule( "QtMoc", BIND_PHONY );

function QtMoc:load()
    local mmodule = self.module;
    local settings = self.settings;

    if built_for_platform_and_variant(self) then
        local cc = Cc();
        self.cc = cc;
        cc.settings = self.settings;
        cc.module = mmodule;
        mmodule:add_dependency( cc );

        for _, value in ipairs(self) do
            local header = HeaderFile( value );
            header:set_required_to_exist( true );
            
            local source = File( cxx_name("moc_"..value) );
            source.unit = self;
            source.header = value;
            header.source = source;
            source:add_dependency( header );
            self:add_dependency( source );
            
            local object = File( obj_directory(self)..obj_name(source:id()) );
            object.source = source:id();
            source.object = object;
            object:add_dependency( source );
            object:add_dependency( directory );
            cc:add_dependency( object );
        end
    end
end

function QtMoc:build()
    if self:is_outdated() and built_for_platform_and_variant(self) then
        local moc = self.settings.qt.moc;
        for dependency in self:get_dependencies() do
            if dependency:is_outdated() then
                print( dependency.header );
                local arguments = "moc "..dependency.header.." -o "..dependency:id();
                if exec(moc, arguments) ~= 0 then
                    error( arguments.." failed" );
                end
            end
        end
    end
end

function QtMoc:clean()
    if built_for_platform_and_variant(self) then
        for dependency in self:get_dependencies() do
            if dependency:rule() == File then
                rm( dependency:path() );
            end
        end
    end
end
