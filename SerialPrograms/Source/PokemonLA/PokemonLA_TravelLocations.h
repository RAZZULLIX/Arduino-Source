/*  Travel Locations
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_PokemonLA_TravelLocations_H
#define PokemonAutomation_PokemonLA_TravelLocations_H

#include <functional>
#include <string>
#include <vector>
#include <map>
#include "ClientSource/Connection/BotBase.h"
#include "NintendoSwitch/Commands/NintendoSwitch_Commands_PushButtons.h"
#include "PokemonLA_Locations.h"

class QString;

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonLA{



struct TravelLocation{
    const char* label;

    MapRegion region;
    uint8_t warp_slot;
    uint8_t warp_sub_slot;

    std::function<void(const BotBaseContext& context)> post_arrival_maneuver;

    TravelLocation(
        const char* p_label,
        MapRegion p_region,
        uint8_t p_warp_slot, uint8_t p_warp_sub_slot,
        std::function<void(const BotBaseContext& context)>&& p_post_arrival_maneuver
    );
};


class TravelLocations{
public:
    static const TravelLocations& instance();

    const TravelLocation* get_from_name(const std::string& name) const;

    std::vector<QString> all_location_names() const;


public:
    const TravelLocation Fieldlands_Fieldlands;
    const TravelLocation Fieldlands_Heights;
    const TravelLocation Fieldlands_Arena;

    const TravelLocation Mirelands_Mirelands;
    const TravelLocation Mirelands_Bogbound;
    const TravelLocation Mirelands_DiamondSettlement;
    const TravelLocation Mirelands_Arena;

    const TravelLocation Coastlands_Beachside;
    const TravelLocation Coastlands_Coastlands;
    const TravelLocation Coastlands_Arena;

    const TravelLocation Highlands_Highlands;
    const TravelLocation Highlands_Mountain;
    const TravelLocation Highlands_Summit;
    const TravelLocation Highlands_Arena;

    const TravelLocation Icelands_Snowfields;
    const TravelLocation Icelands_Icepeak;
    const TravelLocation Icelands_PearlSettlement;
    const TravelLocation Icelands_Arena;


private:
    TravelLocations();
    void add_location(const TravelLocation& location);

    std::vector<const TravelLocation*> m_list;
    std::map<std::string, const TravelLocation*> m_map;
};






}
}
}
#endif