
target_sources(rpg
    PUBLIC
        Dungeon.hh
        DungeonMaster.hh
        Room.hh      
        SampleBuilder.hh
)

target_include_directories(rpg PUBLIC ${HEADERS})
