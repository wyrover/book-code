/******************************************************************************
Module:  Job.h
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#pragma once


///////////////////////////////////////////////////////////////////////////////


#include <malloc.h>  // for _alloca


///////////////////////////////////////////////////////////////////////////////


class CJob {
public:
   CJob(HANDLE hJob = NULL);
   ~CJob();

   operator HANDLE() const { return(m_hJob); }

   // Functions to create/open a job object
   BOOL Create(PSECURITY_ATTRIBUTES psa = NULL, PCTSTR pszName = NULL);
   BOOL Open(PCTSTR pszName, DWORD dwDesiredAccess, 
      BOOL fInheritHandle = FALSE);

   // Functions that manipulate a job object
   BOOL AssignProcess(HANDLE hProcess);
   BOOL Terminate(UINT uExitCode = 0);

   // Functions that set limits/restrictions on the job
   BOOL SetExtendedLimitInfo(PJOBOBJECT_EXTENDED_LIMIT_INFORMATION pjoeli, 
      BOOL fPreserveJobTime = FALSE);
   BOOL SetBasicUIRestrictions(DWORD fdwLimits);
   BOOL GrantUserHandleAccess(HANDLE hUserObj, BOOL fGrant = TRUE);
   BOOL SetSecurityLimitInfo(PJOBOBJECT_SECURITY_LIMIT_INFORMATION pjosli);

   // Functions that query job limits/restrictions
   BOOL QueryExtendedLimitInfo(PJOBOBJECT_EXTENDED_LIMIT_INFORMATION pjoeli);
   BOOL QueryBasicUIRestrictions(PDWORD pfdwRestrictions);
   BOOL QuerySecurityLimitInfo(PJOBOBJECT_SECURITY_LIMIT_INFORMATION pjosli);

   // Functions that query job status information
   BOOL QueryBasicAccountingInfo(
      PJOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION pjobai);
   BOOL QueryBasicProcessIdList(DWORD dwMaxProcesses, 
      PDWORD pdwProcessIdList, PDWORD pdwProcessesReturned = NULL);

   // Functions that set/query job event notifications
   BOOL AssociateCompletionPort(HANDLE hIOCP, ULONG_PTR CompKey);
   BOOL QueryAssociatedCompletionPort(
      PJOBOBJECT_ASSOCIATE_COMPLETION_PORT pjoacp);
   BOOL SetEndOfJobInfo(
      DWORD fdwEndOfJobInfo = JOB_OBJECT_TERMINATE_AT_END_OF_JOB);
   BOOL QueryEndOfJobTimeInfo(PDWORD pfdwEndOfJobTimeInfo);

private:
   HANDLE m_hJob;
};


///////////////////////////////////////////////////////////////////////////////


inline CJob::CJob(HANDLE hJob) {

   m_hJob = hJob;
}


///////////////////////////////////////////////////////////////////////////////


inline CJob::~CJob() {

   if (m_hJob != NULL) 
      CloseHandle(m_hJob);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::Create(PSECURITY_ATTRIBUTES psa, PCTSTR pszName) {

   m_hJob = CreateJobObject(psa, pszName);
   return(m_hJob != NULL);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::Open(
   PCTSTR pszName, DWORD dwDesiredAccess, BOOL fInheritHandle) {

   m_hJob = OpenJobObject(dwDesiredAccess, fInheritHandle, pszName);
   return(m_hJob != NULL);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::AssignProcess(HANDLE hProcess) {

   return(AssignProcessToJobObject(m_hJob, hProcess));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::AssociateCompletionPort(HANDLE hIOCP, ULONG_PTR CompKey) {

   JOBOBJECT_ASSOCIATE_COMPLETION_PORT joacp = { (PVOID) CompKey, hIOCP };
   return(SetInformationJobObject(m_hJob, 
      JobObjectAssociateCompletionPortInformation, &joacp, sizeof(joacp)));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::SetExtendedLimitInfo(
   PJOBOBJECT_EXTENDED_LIMIT_INFORMATION pjoeli, BOOL fPreserveJobTime) {

   if (fPreserveJobTime)
      pjoeli->BasicLimitInformation.LimitFlags |= 
         JOB_OBJECT_LIMIT_PRESERVE_JOB_TIME;

   // If we are to preserve the job's time information, 
   // the JOB_OBJECT_LIMIT_JOB_TIME flag must not be on
   const DWORD fdwFlagTest = 
      (JOB_OBJECT_LIMIT_PRESERVE_JOB_TIME | JOB_OBJECT_LIMIT_JOB_TIME);

   if ((pjoeli->BasicLimitInformation.LimitFlags & fdwFlagTest) 
      == fdwFlagTest) {
      // These flags are mutually exclusive but both are on, error
      DebugBreak();
   }

   return(SetInformationJobObject(m_hJob, 
      JobObjectExtendedLimitInformation, pjoeli, sizeof(*pjoeli)));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::SetBasicUIRestrictions(DWORD fdwLimits) {

   JOBOBJECT_BASIC_UI_RESTRICTIONS jobuir = { fdwLimits };
   return(SetInformationJobObject(m_hJob, 
      JobObjectBasicUIRestrictions, &jobuir, sizeof(jobuir)));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::SetEndOfJobInfo(DWORD fdwEndOfJobInfo) {

   JOBOBJECT_END_OF_JOB_TIME_INFORMATION joeojti = { fdwEndOfJobInfo };
   joeojti.EndOfJobTimeAction = fdwEndOfJobInfo;
   return(SetInformationJobObject(m_hJob, 
      JobObjectEndOfJobTimeInformation, &joeojti, sizeof(joeojti)));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::SetSecurityLimitInfo(
   PJOBOBJECT_SECURITY_LIMIT_INFORMATION pjosli) {

   return(SetInformationJobObject(m_hJob, 
      JobObjectSecurityLimitInformation, pjosli, sizeof(*pjosli)));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QueryAssociatedCompletionPort(
   PJOBOBJECT_ASSOCIATE_COMPLETION_PORT pjoacp) {

   return(QueryInformationJobObject(m_hJob, 
      JobObjectAssociateCompletionPortInformation, pjoacp, sizeof(*pjoacp), 
      NULL));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QueryBasicAccountingInfo(
   PJOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION pjobai) {

   return(QueryInformationJobObject(m_hJob, 
      JobObjectBasicAndIoAccountingInformation, pjobai, sizeof(*pjobai), 
      NULL));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QueryExtendedLimitInfo(
   PJOBOBJECT_EXTENDED_LIMIT_INFORMATION pjoeli) {

   return(QueryInformationJobObject(m_hJob, JobObjectExtendedLimitInformation, 
      pjoeli, sizeof(*pjoeli), NULL));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QueryBasicProcessIdList(DWORD dwMaxProcesses, 
   PDWORD pdwProcessIdList, PDWORD pdwProcessesReturned) {

   // Calculate the # of bytes necessary
   DWORD cb = sizeof(JOBOBJECT_BASIC_PROCESS_ID_LIST) + 
      (sizeof(DWORD) * (dwMaxProcesses - 1));

   // Allocate those bytes from the stack
   PJOBOBJECT_BASIC_PROCESS_ID_LIST pjobpil = 
      (PJOBOBJECT_BASIC_PROCESS_ID_LIST) _alloca(cb);

   // Were those bytes allocated OK? If so, keep going
   BOOL fOk = (pjobpil != NULL);

   if (fOk) {
      pjobpil->NumberOfProcessIdsInList = dwMaxProcesses;
      fOk = ::QueryInformationJobObject(m_hJob, JobObjectBasicProcessIdList, 
         pjobpil, cb, NULL);

      if (fOk) {
         // We got the information, return it to the caller
         if (pdwProcessesReturned != NULL) 
            *pdwProcessesReturned = pjobpil->NumberOfProcessIdsInList;

         CopyMemory(pdwProcessIdList, pjobpil->ProcessIdList, 
            sizeof(DWORD) * pjobpil->NumberOfProcessIdsInList);
      }
   }
   return(fOk);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QueryBasicUIRestrictions(PDWORD pfdwRestrictions) {

   JOBOBJECT_BASIC_UI_RESTRICTIONS jobuir;
   BOOL fOk = QueryInformationJobObject(m_hJob, JobObjectBasicUIRestrictions, 
      &jobuir, sizeof(jobuir), NULL);
   if (fOk)
      *pfdwRestrictions = jobuir.UIRestrictionsClass;
   return(fOk);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QueryEndOfJobTimeInfo(PDWORD pfdwEndOfJobTimeInfo) {

   JOBOBJECT_END_OF_JOB_TIME_INFORMATION joeojti;
   BOOL fOk = QueryInformationJobObject(m_hJob, JobObjectBasicUIRestrictions, 
      &joeojti, sizeof(joeojti), NULL);
   if (fOk)
      *pfdwEndOfJobTimeInfo = joeojti.EndOfJobTimeAction;
   return(fOk);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::QuerySecurityLimitInfo(
   PJOBOBJECT_SECURITY_LIMIT_INFORMATION pjosli) {

   return(QueryInformationJobObject(m_hJob, JobObjectSecurityLimitInformation, 
      pjosli, sizeof(*pjosli), NULL));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::Terminate(UINT uExitCode) {

   return(TerminateJobObject(m_hJob, uExitCode));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CJob::GrantUserHandleAccess(HANDLE hUserObj, BOOL fGrant) {

   return(UserHandleGrantAccess(hUserObj, m_hJob, fGrant));
}


//////////////////////////////// End of File //////////////////////////////////
