/*  Den Sprite Identifier
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "CommonFramework/ImageTools/SolidColorTest.h"
#include "CommonFramework/ImageMatch/FilterToAlpha.h"
#include "CommonFramework/ImageMatch/ImageCropper.h"
#include "CommonFramework/ImageMatch/ImageDiff.h"
#include "PokemonSwSh/Resources/PokemonSwSh_PokemonSprites.h"
#include "PokemonSwSh_DenMonReader.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSwSh{





class DenSpriteMatcher : public ImageMatch::SilhouetteDictionaryMatcher{
public:
    DenSpriteMatcher(DenSpriteMatcher&&) = default;
    DenSpriteMatcher& operator=(DenSpriteMatcher&&) = default;
public:
    DenSpriteMatcher() = default;
    virtual ImageRGB32 process_image(const ImageViewRGB32& image, Color& background) const override{
        // Find the bounding box of the silhouette.
        ImagePixelBox box = ImageMatch::enclosing_rectangle_with_pixel_filter(
            image,
            // The filter is a lambda function that returns true on dark pixels.
            [](Color pixel){
                return (uint32_t)pixel.red() + pixel.green() + pixel.blue() < 100;
            }
        );
        background = Color();
        ImageRGB32 ret = extract_box_reference(image, box).copy();
        ImageMatch::set_alpha_black(ret);
        return ret;
    }
};


DenSpriteMatcher make_DEN_SPRITE_MATCHER(){
    DenSpriteMatcher matcher;
    for (const auto& item : ALL_POKEMON_SILHOUETTES()){
        matcher.add(item.first, item.second.sprite);
    }
    return matcher;
}
const DenSpriteMatcher& DEN_SPRITE_MATCHER(){
    static DenSpriteMatcher matcher = make_DEN_SPRITE_MATCHER();
    return matcher;
}




DenMonReader::DenMonReader(Logger& logger, VideoOverlay& overlay)
    : m_matcher(DEN_SPRITE_MATCHER())
    , m_logger(logger)
    , m_white(overlay, 0.800, 0.200, 0.150, 0.100)
    , m_den_color(overlay, 0.400, 0.050, 0.200, 0.100)
    , m_lair_pink(overlay, 0.575, 0.035, 0.050, 0.100)
    , m_sprite(overlay, 0.098, 0.23, 0.285, 0.41)
{}


DenMonReadResults DenMonReader::read(const ImageViewRGB32& screen) const{
    DenMonReadResults results;
    if (!screen){
        return results;
    }

    ImageStats white = image_stats(extract_box_reference(screen, m_white));
    if (!is_solid(white, {0.303079, 0.356564, 0.340357})){
        return results;
    }
    do{
        ImageStats den_color = image_stats(extract_box_reference(screen, m_den_color));

        if (is_solid(den_color, {0.593023, 0.204651, 0.202326})){
            results.type = DenMonReadResults::RED_BEAM;
            m_logger.log("Den Type: Red Beam", COLOR_BLUE);
            break;
        }
        if (is_solid(den_color, {0.580866, 0.378132, 0.0410021})){
            results.type = DenMonReadResults::PURPLE_BEAM;
            m_logger.log("Den Type: Purple Beam", COLOR_BLUE);
            break;
        }

        ImageStats lair_pink = image_stats(extract_box_reference(screen, m_lair_pink));
//        cout << lair_pink.average << lair_pink.stddev << endl;

        if (is_solid(lair_pink, {0.448155, 0.177504, 0.374341})){
            results.type = DenMonReadResults::MAX_LAIR;
            m_logger.log("Den Type: Max Lair", COLOR_BLUE);
            break;
        }

        return results;
    }while (false);


    ImageViewRGB32 processed = extract_box_reference(screen, m_sprite);
//    processed.save("processed.png");
//    processed = ImageMatch::black_filter_to_alpha(processed);
    results.slugs = m_matcher.match(processed, ALPHA_SPREAD);
    results.slugs.log(m_logger, MAX_ALPHA);

    results.slugs.clear_beyond_alpha(MAX_ALPHA);

    return results;
}



DenMonSelectData::DenMonSelectData(){
    m_database.add_entry(StringSelectEntry("", "(none)"));
    for (const auto& item : ALL_POKEMON_SPRITES()){
        m_database.add_entry(StringSelectEntry(item.first, item.first, item.second.icon));
    }
}
DenMonSelectOption::DenMonSelectOption(std::string label)
    : StringSelectOption(std::move(label), DenMonSelectData::m_database, "")
{}


}
}
}
