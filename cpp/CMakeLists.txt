
target_sources(rpg
    PRIVATE
        Dungeon.cpp
        DungeonMaster.cpp
        Room.cpp
        SampleBuilder.cpp
)

target_include_directories(rpg PRIVATE ${CPP})
