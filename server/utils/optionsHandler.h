#ifndef OPTIONSHANDLING_H_
	#define OPTIONSHANDLING_H_

	#define DEFAULT_PARK_CONFIGFILE "conf/prk.cfg"
	#define FILE_PATH_LENGHT 64

  	// Structure for the arguments parsing. Will be filled by optionsHandler()
	typedef struct {
		char parkingServerCfgFile[FILE_PATH_LENGHT];
	}configuration;

	// Statup arguments handling
    void optionsHandler (int argc, char* const argv[], configuration *conf);

#endif
