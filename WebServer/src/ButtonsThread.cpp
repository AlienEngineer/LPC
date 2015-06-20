#include <Button.h>
#include <Context.h>
#include <AppThreads.h>

ButtonsState states;

void ButtonsThread(void * pvParameters) {
	Button upButton(2, 0, 250);
	Button downButton(2, 1, 250);
	Button okButton(2, 2, 250);

	Button * buttons[BT_LEN];

	buttons[BT_OK] = &okButton;
	buttons[BT_UP] = &upButton;
	buttons[BT_DW] = &downButton;



	states.States[BT_OK] = BT_NONE;
	states.States[BT_UP] = BT_NONE;
	states.States[BT_DW] = BT_NONE;

	while (1) {

		for (uint8_t i = 0; i < BT_LEN; ++i) {
			// Eval button {i}
			buttons[i]->Evaluate();

			// Collect state of button {i}
			states.States[i] = buttons[i]->state;
		}

		ButtonsState * temp = context.GetLastButtonState();

		if (states.HasChanged(temp)) {
			context.SaveButtonState(&states);
		}

		Scheduler::Yield();
	}

}

