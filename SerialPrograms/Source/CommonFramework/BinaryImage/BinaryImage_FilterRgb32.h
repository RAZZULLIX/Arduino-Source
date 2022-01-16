/*  Binary Image
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#ifndef PokemonAutomation_CommonFramework_BinaryImage_FilterRgb32_H
#define PokemonAutomation_CommonFramework_BinaryImage_FilterRgb32_H

#include "Common/Cpp/Color.h"
#include "Kernels/BinaryMatrix/Kernels_BinaryMatrix.h"

class QImage;

namespace PokemonAutomation{


Kernels::PackedBinaryMatrix compress_rgb32_to_binary_min(
    const QImage& image,
    uint8_t min_red,
    uint8_t min_green,
    uint8_t min_blue
);
Kernels::PackedBinaryMatrix compress_rgb32_to_binary_max(
    const QImage& image,
    uint8_t max_red,
    uint8_t max_green,
    uint8_t max_blue
);
Kernels::PackedBinaryMatrix compress_rgb32_to_binary_range(
    const QImage& image,
    uint8_t min_red, uint8_t max_red,
    uint8_t min_green, uint8_t max_green,
    uint8_t min_blue, uint8_t max_blue
);
Kernels::PackedBinaryMatrix compress_rgb32_to_binary_range(
    const QImage& image,
    uint8_t min_alpha, uint8_t max_alpha,
    uint8_t min_red, uint8_t max_red,
    uint8_t min_green, uint8_t max_green,
    uint8_t min_blue, uint8_t max_blue
);


void filter_rgb32(
    const Kernels::PackedBinaryMatrix& matrix,
    QImage& image,
    Color replace_with,
    bool replace_if_zero    //  If false, replace if one.
);




}
#endif