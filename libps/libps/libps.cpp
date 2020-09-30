// This is the main DLL file.

#include "stdafx.h"

#include "libps.h"


bool libps::PShell::Init(String ^ tenantName, String ^ userName, String ^ password)
{
	TenantShell^ retrievedTenant = nullptr;
	configuredTenants->TryGetValue(tenantName, retrievedTenant);
	if (retrievedTenant == nullptr) 
	{
		retrievedTenant = gcnew TenantShell(userName, password);
		retrievedTenant->InitTenant();
		configuredTenants->Add(tenantName, retrievedTenant);
	} 
	return true;
}

String^ libps::PShell::Call(String ^ tenantName, String ^ commandlet)
{
	return "false";
}

libps::TenantShell^ libps::PShell::FetchTenant(String ^ tenantName)
{
	TenantShell^ retrievedTenant = nullptr;
	configuredTenants->TryGetValue(tenantName, retrievedTenant);
	return retrievedTenant;
}

libps::TenantShell::TenantShell(String ^ username, String ^ password)
{
	SecureString^ ssPassword = gcnew SecureString();
	for(int i = 0 ; i < password->Length ; i++)
	{
		ssPassword->AppendChar(password[i]);
	}
	tenantCredential = gcnew PSCredential(username, ssPassword);
}

bool libps::TenantShell::InitTenant()
{
	try
	{
		WSManConnectionInfo^ wsconnect = gcnew WSManConnectionInfo(
		gcnew Uri("https://outlook.office365.com/powershell-liveid/"),
		gcnew String("http://schemas.microsoft.com/powershell/Microsoft.Exchange"),
		tenantCredential);
		wsconnect->AuthenticationMechanism = AuthenticationMechanism::Basic;
		wsconnect->MaximumConnectionRedirectionCount = 1;
		wsconnect->IdleTimeout = 10000000;
		exchRunspace = RunspaceFactory::CreateRunspace(wsconnect);
		exchRunspace->Open();
		Console::WriteLine("Opened");
	}
	catch (const std::exception& e)
	{
		Console::WriteLine(gcnew String(e.what()));
	}
	finally 
	{

	}
	
	return true;
}

ICollection<PSObject^>^ libps::TenantShell::CallCommandlet(String ^ commandlet)
{
	PowerShell^ pshellInst = PowerShell::Create();
	pshellInst->Commands->AddCommand(commandlet);
	pshellInst->Runspace = exchRunspace;
	Console::WriteLine("Invoking");
	return pshellInst->Invoke();
}

JNIEXPORT void JNICALL Java_com_ps_util_PSUtil_init
		(JNIEnv *env, jobject obj, jstring tenant, jstring username, jstring password)
{
	char* _tenant = (char*)env->GetStringUTFChars(tenant, 0);
	char* _username = (char*)env->GetStringUTFChars(username, 0);
	char* _password = (char*)env->GetStringUTFChars(password, 0);

	String^ M_tenantname = gcnew String(_tenant);
	String^ M_userName = gcnew String(_username);
	String^ M_password = gcnew String(_password);

	libps::PShell::Init(M_tenantname, M_userName, M_password);
	
	//Freeup
	env->ReleaseStringUTFChars(tenant, _tenant);
	env->ReleaseStringUTFChars(username, _username);
	env->ReleaseStringUTFChars(password, _password);

	return;
}

JNIEXPORT jstring JNICALL Java_com_ps_util_PSUtil_invoke(JNIEnv* env, jobject, jstring tenant, jstring command)
{
	char* _tenant = (char*)env->GetStringUTFChars(tenant, 0);
	char* _command = (char*)env->GetStringUTFChars(command, 0);
	jstring jresult;

	String^ M_tenantname = gcnew String(_tenant);//getStringText(env, tenant));
	String^ M_command = gcnew String(_command);
	//M_command = M_command + " | ConvertTo-Json";
	Console::WriteLine(M_command);
	libps::TenantShell^ tenantShell = libps::PShell::FetchTenant(M_tenantname);
	if (tenantShell != nullptr)
	{
		Console::WriteLine("Commandlet Calling!");
		ICollection<PSObject^>^ result = tenantShell->CallCommandlet(M_command);
		Console::WriteLine("Commandlet Called!");
		IEnumerator<PSObject^>^ resultList = result->GetEnumerator();
		if (resultList != nullptr)
		{
			while (resultList->MoveNext())
			{
				Console::WriteLine("Checking result");
				PSObject^ obj = resultList->Current;
				String^ resultJson = obj->ToString();
				Console::WriteLine(resultJson);
			}
			
			//IntPtr ptrToNativeString = Marshal::StringToHGlobalUni(resultJson);
			//jresult = env->NewStringUTF(static_cast<char*>(ptrToNativeString.ToPointer()));
			//Marshal::FreeHGlobal(ptrToNativeString);
			return jresult;
		}
	}
	//Freeup
	env->ReleaseStringUTFChars(tenant, _tenant);
	env->ReleaseStringUTFChars(tenant, _tenant);
}


