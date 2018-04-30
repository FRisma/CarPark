#ifndef OPTIONSHANDLING_H_
	#define OPTIONSHANDLING_H_

	#define SLOT_ID_LENGHT 15

  	// Structure for the arguments parsing. Will be filled by optionsHandler()
	typedef struct {
		char opt;
		char slotId[SLOT_ID_LENGHT];
	}configuration;

	// Statup arguments handling
    void optionsHandler (int argc, char* const argv[], configuration *conf);

#endif
