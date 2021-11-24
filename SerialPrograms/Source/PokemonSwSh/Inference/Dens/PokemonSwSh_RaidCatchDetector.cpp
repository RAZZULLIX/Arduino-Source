/*  Raid Catch Detector
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "CommonFramework/ImageTools/SolidColorTest.h"
#include "CommonFramework/ImageTools/ColorClustering.h"
#include "CommonFramework/Inference/ImageTools.h"
#include "PokemonSwSh_RaidCatchDetector.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonSwSh{


RaidCatchDetector::RaidCatchDetector(VideoOverlay& overlay)
    : m_left0 (0.82, 0.85 + 0 * 0.078, 0.01, 0.04)
    , m_right0(0.96, 0.85 + 0 * 0.078, 0.01, 0.04)
//    , m_left1 (0.82, 0.85 + 1 * 0.078, 0.01, 0.04)
//    , m_right1(0.96, 0.85 + 1 * 0.078, 0.01, 0.04)
    , m_text0 (0.82, 0.84 + 0 * 0.078, 0.15, 0.06)
    , m_text1 (0.82, 0.84 + 1 * 0.078, 0.15, 0.06)
    , m_arrow(overlay, ImageFloatBox(0.75, 0.82, 0.10, 0.10))
{
    add_box(m_left0);
    add_box(m_right0);
//    add_box(m_left1);
//    add_box(m_right1);
    add_box(m_text0);
    add_box(m_text1);
    add_boxes(m_arrow);
}
bool RaidCatchDetector::detect(const QImage& screen){
    ImageStats left0 = image_stats(extract_box(screen, m_left0));
    if (!is_black(left0)){
        return false;
    }
    ImageStats right0 = image_stats(extract_box(screen, m_right0));
    if (!is_black(right0)){
        return false;
    }
    if (euclidean_distance(left0.average, right0.average) > 10) return false;
#if 0
    ImageStats left1 = image_stats(extract_box(screen, m_left1));
    if (!is_white(left1)){
        return false;
    }
    ImageStats right1 = image_stats(extract_box(screen, m_right1));
    if (!is_white(right1)){
        return false;
    }
    if (euclidean_distance(left1.average, right1.average) > 10) return false;
#endif

//    cout << "==================" << endl;
    if (!cluster_fit_2(
        extract_box(screen, m_text0),
        qRgb(0, 0, 0), 0.90,
        qRgb(255, 255, 255), 0.10
    )){
        return false;
    }
//    cout << "------------------" << endl;
    if (!cluster_fit_2(
        extract_box(screen, m_text1),
        qRgb(255, 255, 255), 0.90,
        qRgb(0, 0, 0), 0.10
    )){
        return false;
    }

    if (!m_arrow.detect(screen)){
        return false;
    }

    return true;
}
bool RaidCatchDetector::process_frame(
    const QImage& frame,
    std::chrono::system_clock::time_point timestamp
){
    //  Need 5 consecutive successful detections.
    if (!detect(frame)){
        m_trigger_count = 0;
        return false;
    }
    m_trigger_count++;
    return m_trigger_count >= 5;
}




}
}
}
