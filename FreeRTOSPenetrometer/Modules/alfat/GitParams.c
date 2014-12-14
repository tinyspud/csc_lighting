#include "GitParams.h"

int BuildPathLength = -1;
int BuildSystemLength = -1;
int BuildUserDomainLength = -1;
int BuildUserNameLength = -1;
int BuildBranchNameLength = -1;
int BuildCommitSHALength = -1;
int LastCommitTimeLength = -1;
int CompileTimeLength = -1;

#define GET_LENGTH(lenparam, macrotext)		for(lenparam = 0; lenparam < LONGEST_STR_USED; lenparam++){\
		if((macrotext[lenparam] == '\0') || (macrotext[lenparam] == '\r') || (macrotext[lenparam] == '\n'))\
		break;\
}


void GatherGitParams(){

	GET_LENGTH(BuildPathLength, BUILD_PATH);

	GET_LENGTH(BuildSystemLength, BUILD_SYSTEM);

	GET_LENGTH(BuildUserDomainLength, BUILD_USER_DOMAIN);

	GET_LENGTH(BuildUserNameLength, BUILD_USER_NAME);

	GET_LENGTH(BuildBranchNameLength, BRANCH_NAME);

	GET_LENGTH(BuildCommitSHALength, COMMIT_SHA);

	GET_LENGTH(LastCommitTimeLength, LAST_COMMIT_TIME_UTC);

	GET_LENGTH(CompileTimeLength, COMPILE_TIME_UTC);

}
