enum signalStates { PREPARE, INERT, GO, RESOLVE, CHECK, END };
byte signalState = PREPARE;
enum player {PLAYER1, PLAYER2, PLAYER3, PLAYER4, PLAYER5, PLAYER6};//these modes will simply be different colors
byte player = PLAYER1;//the default mode when the game begins
byte player_num = 0;
byte chosen_player = 6;
Timer warn_timer;
Timer check_timer;
void loop() {
	switch (signalState) {
        case PREPARE:
    		prepareLoop();
        	break;
      	case INERT:
      	case GO:
      	case RESOLVE:
        	playLoop();
        	break;
      	case CHECK:
        	checkLoop();
        	break;
      	case END:
        	endLoop();
        	break;
    }
}

void prepareLoop() {
  	FOREACH_FACE(f) {
  		if (getSignalState(getLastValueReceivedOnFace(f)) == INERT) {
            signalState = INERT;
            player = getPlayer(getLastValueReceivedOnFace(f));
          	player_num = player + 1;
			byte sendData = (signalState << 3) + (player);
        	setValueSentOnAllFaces(sendData);
    	}
  		else {
	        if (player_num == 0) {
    	        setColor(OFF);
        	}
        	else {
            	for ( byte i = 0; i < player_num; i ++) {
               		setColorOnFace(getColor(i+1), i);
            	}
        	}
        	if (buttonSingleClicked()) {
            	player_num = (player_num + 1) % 7;
        	}
      		if (buttonDoubleClicked() && player_num != 0) {
                player = player_num - 1;
                signalState = INERT;
				byte sendData = (signalState << 3) + (player);
        		setValueSentOnAllFaces(sendData);
        	}
          	else {
              	setValueSentOnAllFaces(0);
            }
    	}
    }
}

void playLoop() {
	switch (signalState) {
        case INERT:
            inertLoop();
            break;
        case GO:
            goLoop();
            break;
        case RESOLVE:
            resolveLoop();
            break;
    }
    displaySignalState();
    byte sendData = (signalState << 3) + (player);
    setValueSentOnAllFaces(sendData);
}
  
void inertLoop() {
    //set myself to GO
    if (buttonSingleClicked()) {
      	if (chosen_player == 6) {
          	chosen_player = player;
        }
      	else {
          	warn_timer.set(1000);
        }
        signalState = GO;
        player = (player + 1) % (player_num);//adds one to game mode, but 3+1 becomes 0
    }
    //listen for neighbors in GO
    FOREACH_FACE(f) {
        if (!isValueReceivedOnFaceExpired(f)) { //a neighbor!
            if (getSignalState(getLastValueReceivedOnFace(f)) == GO) { //a neighbor saying GO!
                signalState = GO;
                player = getPlayer(getLastValueReceivedOnFace(f));
            }
        }
    }
}
void goLoop() {
    signalState = RESOLVE; //I default to this at the start of the loop. Only if I see a problem does this not happen
    //look for neighbors who have not heard the GO news
    FOREACH_FACE(f) {
        if (!isValueReceivedOnFaceExpired(f)) { //a neighbor!
            if (getSignalState(getLastValueReceivedOnFace(f)) == INERT) {//This neighbor doesn't know it's GO time. Stay in GO
                signalState = GO;
            }
        }
    }
}

void resolveLoop() {
  	if (chosen_player == 6) {
    	signalState = INERT; //I default to this at the start of the loop. Only if I see a problem does this not happen
    }
  	else {
		check_timer.set(2000);
      	signalState = CHECK;
    }
    //look for neighbors who have not moved to RESOLVE
    FOREACH_FACE(f) {
        if (!isValueReceivedOnFaceExpired(f)) { //a neighbor!
            if (getSignalState(getLastValueReceivedOnFace(f)) == GO) {//This neighbor isn't in RESOLVE. Stay in RESOLVE
                signalState = RESOLVE;
            }
        }
    }
}

void checkLoop() {
	if (check_timer.isExpired()) {
		setColor(OFF);
		signalState = END;
	}
	else {
		setColor(WHITE);
		FOREACH_FACE(f) {
        	if (!isValueReceivedOnFaceExpired(f)) { //a neighbor!
            	if (getSignalState(getLastValueReceivedOnFace(f)) == INERT) {//This neighbor isn't in RESOLVE. Stay in RESOLVE
                	signalState = INERT;
            	}
        	}
    	}
		setValueSentOnAllFaces(6 << 3);
	}
}

void endLoop() {
	setColor(getColor(getRealPlayer(chosen_player)))
	FOREACH_FACE(f) {
    	if (!isValueReceivedOnFaceExpired(f)) { //a neighbor!
            if (getSignalState(getLastValueReceivedOnFace(f)) == PREPARE) {//This neighbor isn't in RESOLVE. Stay in RESOLVE
				signalState = PREPARE;
				player = PLAYER1;
				player_num = 0;
				chosen_player = 6;
              	setColor(OFF);
            }
    	}
	}
	if (buttonDoubleClicked()) {
		signalState = PREPARE;
		player = PLAYER1;
		player_num = 0;
		chosen_player = 6;
		setValueSentOnAllFaces(0);
	}
}

void displaySignalState() {
  	if (warn_timer.isExpired()) {
  		switch (signalState) {
    		case INERT:
      			setColor(getColor(getRealPlayer(player)));
      			break;
    		case GO:
    		case RESOLVE:
	      		setColor(WHITE);
    	  		break;
        }
  	}
  	else {
      	setColor(WHITE);
    }
}

Color getColor(data) {
	switch (data) {
      	case 0:
			return (makeColorRGB(0,0,0));
        	break;
      	case 1:
        	return (makeColorRGB(255,0,0));
	        break;
    	case 2:
        	return (makeColorRGB(0,255,0));
        	break;
      	case 3:
        	return (makeColorRGB(0,0,255));
        	break;
      	case 4:
			return (makeColorRGB(255,0,255));
	        break;
      	case 5:
        	return (makeColorRGB(255,255,0));
        	break;
      	case 6:
        	return (makeColorRGB(0,255,255));
        	break;
    }
}

byte getRealPlayer(byte data) {
	return (data + 1) % player_num + 1;
}

byte getPlayer(byte data) {
  return (data & 7);//returns bits E and F
}

byte getSignalState(byte data) {
  return ((data >> 3) & 7);//returns bits C and D
}