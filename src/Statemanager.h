#ifndef __STATEMANAGER_H__
#define __STATEMANAGER_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Button.h>
#include <FiniteStateMachine.h>
#include <iterator> // The iterator has to be included if you want to use <vector>
#include <vector>

/**
 * Provides a KISS statemanager for use with Arduino projects. 
 *
 * Requires the FiniteStateMachine arduino library, see:
 * http://playground.arduino.cc/code/FiniteStateMachine
 * and the avr_stl library, see:
 * https://github.com/maniacbug/StandardCplusplus
 * <p>
 * Usage:
 * // Includes
 * #include <FiniteStateMachine.h>
 * #include "Statemanager.h"
 *
 * // Define state functions
 * static const State stateArr[] = {
 *		State(STATE_NAME_ENTER, STATE_NAME_UPDATE, STATE_NAME_EXIT)
 * };
 *
 * // Initialize the stateManager with the states
 * std::vector<State> statesVector (stateArr, stateArr + sizeof(stateArr) / sizeof(stateArr[0]) );
 * StateManager stateMngr = StateManager(statesVector);
 *
 * // In the update loop, update state:
 * void loop() {
 * 		// Check states (check which one is active and run the update function)
 *		stateMngr.checkStates();
 * }
 *
 * // Go to the next state
 * stateMngr.transitionToNextState();
 * </p>
 *
 * @author      Wilbert van de Ridder <l.w.vanderidder @ student.utwente.nl>
 * @version     1.0 
 * @since       2013-07-07
 */
class StateManager {		
	public:	
		/**	
		 * Constructor
		 * 
		 * @param byte stateVector Vector containing all initial states.
		 * @see addState()
		 */
		StateManager(std::vector<State>& stateVector);
		
		/**	
		 * Constructor
		 *  
		 * @param byte stateVector Vector containing all initial states.
		 * @param byte buttonStateSwitchPin; pin number of button that switches the states.
		 * @see addState()
		 */
		StateManager(byte buttonStateSwitchPin, std::vector<State>& stateVector);
		
		/**	
		 * Adds a new state to the system.
		 *
		 * @param newState A new state object.
		 */
		void addState(State& newState);
		
		/**	
		 * Add a vector with led pins to use for state indication.
		 *
		 * @param aLedVector Vector containing pin numbers.
		 */
		void addLedVector(std::vector<byte>& aLedVector);
		
		/**	
		 *	Checks if the current state has to be changed due to a button press.
		 */
		void checkStates();
		
		/**
		 * Forces transition to the next state.
		 * 
		 * Can be used for example when there is no button to switch states. 
		 * Another usage may be to have a state being active only a 
		 * predefined amount of time. E.g. using the blinkLed() function.
		 *
		 * @see blinkLed()
		 */
		void transitionToNextState();
		
		/**	
		 * Blinks the led for predefined amount of time, acts as a delay.
		 * 
		 * ledIndication should be active.
		 * 
		 * @param int switchTime; time between on / off switching.
		 * @param int totalTime; total time the led will blink.
		 * @see addLedVector()
		 * @see ledStateIndication
		 */
		void blinkLed(int switchTime, int totalTime);
		
		/**	
		 * Blinks the led and halts execution until a button has been pressed.
		 * 
		 * ledIndication should be active.
		 * 
		 * @param int switchTime; time between on / off switching.
		 * @see addLedVector()
		 * @see ledStateIndication
		 */
		void blinkLedWaitButtonPress(int aSwitchTime);
	private:
		/** 	
		 * Increments the state number.
		 * 
		 * @see currentState
		 */
		void incrementState();
		
		/**	
		 * Updates all LED's.
		 */
		void updateLedIndication();
		
		/**	
		 * FiniteStateMachine object.
		 */
		FiniteStateMachine arduinoStateMachine;
		
		/**	
		 * Counter variable, holds number of button presses and determines 
		 * the current active state.
		 */
		int currentState;
		
		/**	
		 * Total number of states in the system.
		 */
		byte numberOfStates;
		
		/**	
		 * Button for switching states.
		 */
		Button* buttonState;
		
		/**	
		 * Vector containing all states.
		 */
		std::vector<State> stateVector;
		
		/**	
		 * Boolean to indicate whether to use leds for state indication.
		 */
		bool ledStateIndication;
		
		/**	
		 * Vector containing all pin numbers used for led state indication.
		 */
		std::vector<byte> ledVector;
		
		/**	
		 * Boolean to indicate whether to use a button to switch states.
		 */
		bool useButtonToSwitchStates;
};
#endif
