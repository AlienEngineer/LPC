#include <Button.h>
#include <Context.h>



void ButtonsThread(void * pvParameters) {
	Button upButton(2, 0, 500);
	Button downButton(2, 1, 500);
	Button okButton(2, 2, 500);

	Button * buttons[BT_LEN];

	buttons[BT_OK] = &okButton;
	buttons[BT_UP] = &upButton;
	buttons[BT_DW] = &downButton;

	while (1) {
		ButtonsState states;

		for (uint8_t i = 0; i < BT_LEN; ++i) {

			buttons[i]->Evaluate();

			switch (buttons[i]->state) {
			case BT_PRESSED:
				states.States[i] = BT_PRESSED;
				break;
			case BT_RELEASED:
				context.SaveButtonState(&states);
				break;
			case BT_LONGPRESS:
				states.States[i] = BT_LONGPRESS;
				context.SaveButtonState(&states);
				break;
			}

		}

	}

}

