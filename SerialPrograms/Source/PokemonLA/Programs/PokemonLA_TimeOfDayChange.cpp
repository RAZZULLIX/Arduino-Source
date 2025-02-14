/*  Time of Day Change
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "CommonFramework/Tools/ConsoleHandle.h"
#include "Common/Cpp/Exceptions.h"
#include "CommonFramework/VideoPipeline/VideoFeed.h"
#include "CommonFramework/InferenceInfra/InferenceRoutines.h"
#include "NintendoSwitch/Commands/NintendoSwitch_Commands_PushButtons.h"
#include "PokemonLA/Inference/Objects/PokemonLA_DialogueYellowArrowDetector.h"
#include "PokemonLA_TimeOfDayChange.h"

#include <iostream>
using std::cout;
using std::endl;

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonLA{


void change_time_of_day_at_tent(ConsoleHandle& console, BotBaseContext& context, TimeOfDay target_time, Camp camp){
    // Move to the tent
    switch (camp)
    {
    case Camp::FIELDLANDS_FIELDLANDS:
        pbf_move_left_joystick(context, 105, 0, 220, 20);
        break;
    
    case Camp::FIELDLANDS_HEIGHTS:
        pbf_move_left_joystick(context, 95, 0, 250, 20);
        break;
    
    case Camp::MIRELANDS_MIRELANDS:
        pbf_move_left_joystick(context, 70, 0, 180, 20);
        break;
    
    case Camp::MIRELANDS_BOGBOUND:
        pbf_move_left_joystick(context, 70, 0, 170, 20);
        break;
    
    case Camp::COASTLANDS_BEACHSIDE:
        pbf_move_left_joystick(context, 100, 0, 130, 20);
        break;
    
    case Camp::COASTLANDS_COASTLANDS:
        pbf_move_left_joystick(context, 75, 0, 160, 20);
        break;
    
    case Camp::HIGHLANDS_HIGHLANDS:
        pbf_move_left_joystick(context, 95, 0, 190, 20);
        break;
    
    case Camp::HIGHLANDS_MOUNTAIN:
        pbf_move_left_joystick(context, 60, 0, 190, 20);
        break;
    
    case Camp::HIGHLANDS_SUMMIT:
        pbf_move_left_joystick(context, 100, 0, 220, 20);
        break;
    
    case Camp::ICELANDS_SNOWFIELDS:
        pbf_move_left_joystick(context, 80, 0, 150, 20);
        break;

    case Camp::ICELANDS_ICEPEAK:
        pbf_move_left_joystick(context, 110, 0, 180, 20);
        break;
    }

    // Press A to interact with tent
    pbf_press_button(context, BUTTON_A, 30, 30);
    context.wait_for_all_requests();

    const bool stop_on_detected = true;
    DialogueYellowArrowDetector yellow_arrow_detector(console, console, stop_on_detected);

    context.wait_for_all_requests();
    // Wait for the dialog box to show up
    int ret = wait_until(
        console, context, std::chrono::seconds(5), {{yellow_arrow_detector}}
    );
    if (ret < 0){
        throw OperationFailedException(console, "Did not interact with a tent");
    }

    // Press A to clear the dialog box, and show the time menu
    // pbf_wait(context, 40);
    pbf_press_button(context, BUTTON_A, 30, 80);
    console.log("Change time of day to " + std::string(TIME_OF_DAY_NAMES[int(target_time)]));

    // Move down the menu to find the target time
    for(int i = 0; i < (int)target_time; i++){
        pbf_press_dpad(context, DPAD_DOWN, 30, 70);
    }

    // Press A to start resting
    pbf_press_button(context, BUTTON_A, 30, 100);
    context.wait_for_all_requests();

    // Wait for the dialog box to show up
    ret = wait_until(
        console, context, std::chrono::seconds(30), {{yellow_arrow_detector}}
    );
    if (ret < 0){
        throw OperationFailedException(console, "Failed to stand up after resting in a tent");
    }

    // Press A again to clear the dialog box
    pbf_press_button(context, BUTTON_A, 30, 100);

    context.wait_for_all_requests();
}



}
}
}
