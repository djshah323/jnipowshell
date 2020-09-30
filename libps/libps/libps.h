// libps.h

#pragma once

#include <string>

#include "pshell.h"

using namespace System;
using namespace System::Text;
using namespace System::Security;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;
using namespace System::Management::Automation;
using namespace System::Management::Automation::Runspaces;

//typedef wchar_t	unicode;

namespace libps {

	public ref class TenantShell
	{
	private:
		Runspace^ exchRunspace;
		PSCredential^ tenantCredential;

	public:
		TenantShell(String^ username, String^ password);
		bool InitTenant();
		ICollection<PSObject^>^ CallCommandlet(String^ commandlet);
	};

	public ref class PShell
	{
		// TODO: Add your methods for this class here.
		private:  
			static Dictionary<String^, TenantShell^> ^configuredTenants
				= gcnew Dictionary<System::String^, TenantShell^>();
		public:
			static bool Init(String^ tenantName, String^ userName, String^ password);
			static String^ Call(String^ tenantName, String^ commandlet);
			static TenantShell^ FetchTenant(String^ tenantName);
	};

}
