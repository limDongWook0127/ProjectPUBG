#include "BaseLibrary/BaseStructType.h"

bool FPlayerAbilitySet::IsValid() const
{	
	return InputTag.IsValid() && AbilityToGrant;
}