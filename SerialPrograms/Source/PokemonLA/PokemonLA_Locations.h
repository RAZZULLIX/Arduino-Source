/*  Pokemon Legends Arceus Locations
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonLA_Locations_H
#define PokemonAutomation_PokemonLA_Locations_H

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonLA{

enum class MapRegion{
    NONE,
    JUBILIFE,
    FIELDLANDS,
    MIRELANDS,
    COASTLANDS,
    HIGHLANDS,
    ICELANDS,
    RETREAT,
};

// Map from int(MapRegion) to its region name
extern const char* MAP_REGION_NAMES[];

// from int [0, 1, 2, 3, 4] to short names of the five wild regions:
// { "Fieldlands", "Mirelands", ... }
// This is useful for logging purpose.
extern const char* WILD_REGION_SHORT_NAMES[];


enum class Camp{
    FIELDLANDS_FIELDLANDS,
    FIELDLANDS_HEIGHTS,
    MIRELANDS_MIRELANDS,
    MIRELANDS_BOGBOUND,
    COASTLANDS_BEACHSIDE,
    COASTLANDS_COASTLANDS,
    HIGHLANDS_HIGHLANDS,
    HIGHLANDS_MOUNTAIN,
    HIGHLANDS_SUMMIT,
    ICELANDS_SNOWFIELDS,
    ICELANDS_ICEPEAK,
};

// Return true if the region is fieldlands, mirelands, coastlands, highlands or icelands.
bool is_wild_land(MapRegion region);




}
}
}
#endif
