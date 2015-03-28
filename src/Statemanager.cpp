#include "Statemanager.h"

StateManager::StateManager(std::vector<State>& aStateVector)
		: arduinoStateMachine(aStateVector[0]), currentState(0), numberOfStates(0)
{
	// Set member variables
	stateVector = aStateVector;
	numberOfStates = aStateVector.size();
	useButtonToSwitchStates = false;
}

StateManager::StateManager(byte buttonStateSwitchPin, std::vector<State>& aStateVector)
		: arduinoStateMachine(aStateVector[0]), currentState(0), numberOfStates(0)			
{
	// Set member variables
	stateVector = aStateVector;
	numberOfStates = aStateVector.size();
	useButtonToSwitchStates = true;
	
	// Set switch button
	buttonState = new Button((uint8_t)buttonStateSwitchPin,PULLDOWN);
}

void StateManager::addState(State& newState) {
	// Add a new state to the stateArray
	stateVector.push_back(newState);
	numberOfStates++;
}

void StateManager::addLedVector(std::vector<byte>& aLedVector) {
	ledVector = aLedVector;
	
	// Check if the vector is not empty
	if (ledVector.size() > 0) {
		// Set pins in output mode
		for ( int i = 0; i < ledVector.size(); i++ ) {
			pinMode(ledVector[i],OUTPUT);
		}
	
		// Turn on led indication		
		ledStateIndication = true;
		
		// Update the led indication
		updateLedIndication();
	}
}

void StateManager::checkStates() {
	if (useButtonToSwitchStates) {
		// Check if the button has been pressed, if so, change state
		if (buttonState->uniquePress()){
			// Increment currentState and constrain it to the number of states
			transitionToNextState();
		}
	}
	arduinoStateMachine.update();
}

void StateManager::transitionToNextState() {
	// Increment state
	incrementState();
	arduinoStateMachine.transitionTo(stateVector[currentState]);
}

void StateManager::blinkLed(int switchTime, int totalTime) {	
	if (ledStateIndication) {
		int totalBlinks = totalTime / switchTime;
		byte ledStatePin = currentState % ledVector.size();
		
		// Blink the led
		for ( int i = 0; i < totalBlinks; i++ ) {
			digitalWrite(ledVector[ledStatePin], (i % 2) ? HIGH : LOW);
			delay(switchTime);
		}
		
		// Make sure the led is always on when exiting this function
		digitalWrite(ledVector[ledStatePin], HIGH);
	}
}

void StateManager::blinkLedWaitButtonPress(int aSwitchTime) {
	if (ledStateIndication) {
		int ledState = HIGH;
		byte counter = 0;
		int switchTime = aSwitchTime;
		byte ledStatePin = currentState % ledVector.size();
		
		while(true) {
			if (buttonState->uniquePress()){
				// Button pressed, stop halting
				break;
			}
			ledState = (ledState) ? LOW : HIGH;
			digitalWrite(ledVector[ledStatePin], ledState);
			
			// Add pattern to blinking to indicate it's waiting for a key press
			if (counter % 7 == 0) {
				switchTime = aSwitchTime * 2.5;
			}			
			if (counter % 13 == 0) {
				switchTime = aSwitchTime;
			}			
			counter++;
			delay(switchTime);
		}
		// Make sure the led is always on when exiting this function
		digitalWrite(ledVector[ledStatePin], HIGH);
	}
};

void StateManager::incrementState() {
	// Increment the state index
	currentState = ++currentState % numberOfStates;
	
	// Check if we need to update the led indicator
	updateLedIndication();
}

void StateManager::updateLedIndication() {
	if (ledStateIndication) {
		// Make sure all leds are turned off
		for ( int i = 0; i < ledVector.size(); i++ ) {
			// Set designated pin to LOW
			digitalWrite(ledVector[i], LOW);
		}
		
		// Turn on the led indicating the current state
		byte ledStatePin = currentState % ledVector.size();
		digitalWrite(ledVector[ledStatePin], HIGH);
	}
}
