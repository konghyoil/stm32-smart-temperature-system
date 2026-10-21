#include "State.h"

Temperature_data System_Data = {0};

void Set_State(Temperature_data *Set_Data)
{
    switch(Set_Data->State)
    {
        case OFF_STATE:

            if(Set_Data->button_flag == button_on)
            {
                if(Set_Data->temp_current >= MaxTemp ||Set_Data->temp_current <= MinTemp ||Set_Data->temp_target >= MaxTemp)
                {
                    Set_Data->State = ERROR_STATE;
                }
                else if(Set_Data->temp_target > Set_Data->temp_current)
                {
                    Set_Data->State = HEATING_STATE;
                }
                else
                {
                    Set_Data->State = IDLE_STATE;
                }
            }

            break;

        case IDLE_STATE:

            if(Set_Data->temp_current >= MaxTemp ||Set_Data->temp_current <= MinTemp ||Set_Data->temp_target >= MaxTemp)
            {
                Set_Data->State = ERROR_STATE;
            }
            else if(Set_Data->button_flag != button_on)
            {
                Set_Data->State = OFF_STATE;
            }
            else if(Set_Data->temp_current <= (Set_Data->temp_target))
            {
                Set_Data->State = HEATING_STATE;
            }

            break;

        case HEATING_STATE:

            if(Set_Data->temp_current >= MaxTemp ||Set_Data->temp_current <= MinTemp ||Set_Data->temp_target >= MaxTemp)
            {
                Set_Data->State = ERROR_STATE;
            }
            else if(Set_Data->button_flag != button_on)
            {
                Set_Data->State = OFF_STATE;
            }
            else if(Set_Data->temp_current - 1 >= Set_Data->temp_target)
            {
                Set_Data->State = IDLE_STATE;
            }

            break;

        case ERROR_STATE:

            if(Set_Data->button_flag != button_on)
            {
            	Set_Data->State = OFF_STATE;
            }
            else if(Set_Data->temp_current < MaxTemp &&Set_Data->temp_current > MinTemp &&Set_Data->temp_target < MaxTemp)
            {
                if(Set_Data->temp_target > Set_Data->temp_current)
                {
                	Set_Data->State = HEATING_STATE;
                }
                else
                {
                    Set_Data->State = IDLE_STATE;
                }
            }

            break;

        default:
            Set_Data->State = ERROR_STATE;
            break;
    }
}

void State_Work(Temperature_data *Work_Data)
{
	switch(Work_Data->State)
	{
		case  HEATING_STATE :

			Relay_on();

			break;

		case OFF_STATE :
		case IDLE_STATE :
		case ERROR_STATE :

			Relay_off();

			break;

	}
}
