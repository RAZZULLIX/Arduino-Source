/*  Hardware Validation
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "Common/Cpp/CpuId/CpuId.h"

#ifdef PA_ARCH_x86
#include "HardwareValidation_x86.tpp"
#else
#warning "No hardware validation for this architecture."
namespace PokemonAutomation{
    bool check_hardware(){
        return true;
    }
}
#endif
