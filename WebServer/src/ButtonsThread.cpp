#include <Button.h>
#include <Context.h>
#include <AppThreads.h>



void ButtonsThread(void * pvParameters) {
	Button upButton(2, 0, 250);
	Button downButton(2, 1, 250);
	Button okButton(2, 2, 250);

	Button * buttons[BT_LEN];

	buttons[BT_OK] = &okButton;
	buttons[BT_UP] = &upButton;
	buttons[BT_DW] = &downButton;

	ButtonsState states;

	states.States[BT_OK] = BT_NONE;
	states.States[BT_UP] = BT_NONE;
	states.States[BT_DW] = BT_NONE;

	while (1) {


		for (uint8_t i = 0; i < BT_LEN; ++i) {

			buttons[i]->Evaluate();
			states.States[i] = buttons[i]->state;

			switch (buttons[i]->state) {
			// case BT_PRESSED:
			case BT_RELEASED:
			case BT_LONGPRESS:
				context.SaveButtonState(&states);
				break;
			}

		}

		Scheduler::Yield();
	}

}

