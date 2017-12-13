#include "SubProtocol/SBD.h"
#include "SubProtocol/SBOR.h"
#include "SubProtocol/SM.h"
#include "SubProtocol/SSED.h"
#include "SubProtocol/SMIN.h"
#include "SubProtocol/SMINn.h"

int main (int argc, char *argv[])
{
	if (argc>1) {
		if(strcmp(argv[1], "SBD")==0) { 
			SBD_tester();
		}
		else if(strcmp(argv[1], "SBOR")==0) {
			SBOR_tester();
		}
		else if(strcmp(argv[1],"SM")==0) {
			SM_tester();
		}
		else if(strcmp(argv[1], "SSED")==0) {
			SSED_tester();
		}
		else if(strcmp(argv[1], "SMIN")==0) {
			SMIN_tester();
		}
		else if(strcmp(argv[1], "SMINn")==0) {
			SMINn_tester();
		}
		else {
		printf("Please choose any of the following argument\n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n"
		,"SBD", "SBOR", "SM", "SSED", "SMIN", "SMINn");
		}
	}
	else {
		printf("Please choose any of the following argument\n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n\t%s\n"
		,"SBD", "SBOR", "SM", "SSED", "SMIN", "SMINn");
	}

	return 0;
}