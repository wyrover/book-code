#include "FuncAddrValid.h"

__inline ULONG CR4()
{
	// mov eax, cr4
	__asm _emit 0x0F __asm _emit 0x20 __asm _emit 0xE0
}
BOOLEAN
	MmIsSystemAddressAccessable(
	PVOID VirtualAddress
	)

	/*++

	Routine Description

	For a given virtual address this function returns TRUE if the address
	is accessable without an access violation (it may incur a page fault).

	Arguments

	VirtualAddress

	Address to be checked

	Return value

	TRUE if address may be safely accessed, FALSE otherwise

	--*/

{
	if (CR4() & PAE_ON)
	{
		PMMPTE_PAE PointerPte;

		PointerPte = MiGetPdeAddressPae (VirtualAddress);
		if ((PointerPte->u.Long.QuadPart == MM_ZERO_PTE) ||
			(PointerPte->u.Long.QuadPart == MM_ZERO_KERNEL_PTE) ||
			(PointerPte->u.Soft.Protection == MM_ZERO_ACCESS)) 
		{
			return FALSE;
		}

		PointerPte = MiGetPteAddressPae (VirtualAddress);
		if ((PointerPte->u.Long.QuadPart == MM_ZERO_PTE) ||
			(PointerPte->u.Long.QuadPart == MM_ZERO_KERNEL_PTE) ||
			(PointerPte->u.Soft.Protection == MM_ZERO_ACCESS)) 
		{
			return FALSE;
		}
	}
	else
	{
		PMMPTE PointerPte;

		PointerPte = MiGetPdeAddress (VirtualAddress);
		if ((PointerPte->u.Long == MM_ZERO_PTE) ||
			(PointerPte->u.Long == MM_ZERO_KERNEL_PTE) ||
			(PointerPte->u.Soft.Protection == MM_ZERO_ACCESS)) 
		{
			return FALSE;
		}

		PointerPte = MiGetPteAddress (VirtualAddress);
		if ((PointerPte->u.Long == MM_ZERO_PTE) ||
			(PointerPte->u.Long == MM_ZERO_KERNEL_PTE) ||
			(PointerPte->u.Soft.Protection == MM_ZERO_ACCESS)) 
		{
			return FALSE;
		}
	}

	return TRUE;
}

#if 0
extern "C" extern ULONG KiBugCheckData;

VOID MmAllowPageFaultsAtRaisedIrql ()
	/**
	Allow page faults at raised IRQL
	*/
{
	//
	// KiTrap0E performs check if KiBugCheckData == 0.
	// If so, KiTrap0E calls KeBugCheckEx(IRQL_NOT_LESS_OR_EQUAL,...)
	//  if not, it calls MmAccessFault and resolves fault.
	//

	KiBugCheckData = 0xFFFFFFFF;
}

VOID MmForbidPageFaultsAtRaisedIrql ()
{
	KiBugCheckData = 0;
}
#endif

// Irql < DISPATCH_LEVEL
VOID MmMakeAddressWritable (PVOID VirtualAddress, BOOLEAN Write)
{
	PMMPTE_PAE PointerPte_Pae;
	PMMPTE PointerPte;
	ULONG t;

	ASSERT (MmIsSystemAddressAccessable(VirtualAddress));

	// load page
	t = *(ULONG*)VirtualAddress;

	// make it writable
	if (CR4() & PAE_ON)
	{
		PointerPte_Pae = MiGetPteAddressPae (VirtualAddress);
		PointerPte_Pae->u.Hard.Write = Write;
	}
	else
	{
		PointerPte = MiGetPteAddress (VirtualAddress);
		PointerPte->u.Hard.Valid = Write;
	}
}

PMDL LockMem(PVOID Buffer, ULONG Size)
{
	PMDL Mdl;
	ULONG i;

	Mdl = IoAllocateMdl (Buffer, Size, FALSE, FALSE, NULL);
	if (Mdl)
	{
		//KdPrint(("LOCKMEM: Mdl allocated at %X\n", Mdl));

		__try
		{
			for (i=0; i<Size; i++)
			{
				((UCHAR*)Buffer)[i] = ((UCHAR*)Buffer)[i];
			}

			KdPrint (("LOCKMEM: Loaded pages\n"));

			MmProbeAndLockPages (Mdl, KernelMode, IoWriteAccess);

			KdPrint (("LOCKMEM: Mdl probed and locked\n"));
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			IoFreeMdl (Mdl);
			//KdPrint(("LockMem: MmProbeAndLock pages failed with exception code %X\n", GetExceptionCode()));
			return NULL;
		}

		return Mdl;
	}

	//KdPrint(("LockMem: IoAllocateMdl failed\n"));
	return NULL;
}


VOID UnlockMem (PMDL Mdl)
{
	MmUnlockPages (Mdl);
	IoFreeMdl (Mdl);
}
VALIDITY_CHECK_STATUS MmIsAddressValidExNotPae(
	IN PVOID Pointer
	)
{
	VALIDITY_CHECK_STATUS  Return = VCS_INVALID;
	MMPTE* Pde;
	MMPTE* Pte;
	MMPTE pte;

	Pde = MiGetPdeAddress(Pointer);

	//KdPrint(("PDE is 0x%08x\n", Pde));
	if( Pde->u.Hard.Valid )
	{
		//KdPrint(("PDE entry is valid, PTE PFN=%08x\n", Pde->u.Hard.PageFrameNumber));

		Pte = MiGetPteAddress(Pointer);

		//KdPrint(("PTE is 0x%08x\n", Pte));
		if( Pte->u.Hard.Valid )
		{
			//KdPrint(("PTE entry is valid, PFN=%08x\n", Pte->u.Hard.PageFrameNumber));
			Return = VCS_VALID;
		}
		else
		{
			//
			// PTE is not valid
			//

			pte = *Pte;

			//KdPrint(("Got invalid PTE [%08x]: Proto=%d,Transition=%d,Protection=0x%x,PageFilePFN=0x%x\n",
			//	pte.u.Long,
			//	pte.u.Soft.Prototype,
			//	pte.u.Soft.Transition,
			//	pte.u.Soft.Protection,
			//	pte.u.Soft.PageFileHigh));

			if( pte.u.Long )
			{
				if( pte.u.Soft.Prototype == 1 )
				{
					//KdPrint(("PTE entry is not valid, points to prototype PTE.\n"));

					// more accurate check should be performed here for pointed prototype PTE!

					Return = VCS_PROTOTYPE;
				}
				else  // not a prototype PTE
				{
					if( pte.u.Soft.Transition != 0 )
					{
						//
						// This is a transition page. Consider it invalid.
						//

						//KdPrint(("PTE entry is not valid, points to transition page.\n"));

						Return = VCS_TRANSITION;
					}
					else if (pte.u.Soft.PageFileHigh == 0)
					{
						//
						// Demand zero page
						//

						//KdPrint(("PTE entry is not valid, points to demand-zero page.\n"));

						Return = VCS_DEMANDZERO;
					}
					else
					{
						//
						// Pagefile PTE
						//

						if( pte.u.Soft.Transition == 0 )
						{
							//KdPrint(("PTE entry is not valid, VA is paged out (PageFile offset=%08x)\n",
							//	pte.u.Soft.PageFileHigh));

							Return = VCS_PAGEDOUT;
						}
						else
						{
							//KdPrint(("PTE entry is not valid, Refault\n"));
						}
					}
				}
			}
			else
			{
				//KdPrint(("PTE entry is completely invalid\n"));
			}
		}
	}
	else
	{
		//KdPrint(("PDE entry is not valid\n"));
	}

	return Return;
}
VALIDITY_CHECK_STATUS MmIsAddressValidExPae(
	IN PVOID Pointer
	)
{
	VALIDITY_CHECK_STATUS Return = VCS_INVALID;
	MMPTE_PAE* Pde;
	MMPTE_PAE* Pte;
	MMPTE_PAE pte;

	Pde = MiGetPdeAddressPae(Pointer);

	//KdPrint(("PDE is at 0x%08x\n", Pde));
	if( Pde->u.Hard.Valid )
	{
		//KdPrint(("PDE entry is valid, PTE PFN=%08x\n", Pde->u.Hard.PageFrameNumber));

		if( Pde->u.Hard.LargePage != 0 )
		{
			//
			// This is a large 2M page
			//

			//KdPrint(("! PDE points to large 2M page\n"));

			Pte = Pde;
		}
		else
		{
			//
			// Small 4K page
			//

			// Get its PTE
			Pte  = MiGetPteAddressPae(Pointer);
		}

		//KdPrint(("PTE is at 0x%08x\n", Pte));
		if( Pte->u.Hard.Valid )
		{
			//KdPrint(("PTE entry is valid, PFN=%08x\n", Pte->u.Hard.PageFrameNumber));

			Return = VCS_VALID;
		}
		else
		{
			//
			// PTE is not valid
			//

			pte = *Pte;

			//KdPrint(("Got invalid PTE [%08x%08x]\n", pte.u.Long.HighPart, pte.u.Long.LowPart));

			if( pte.u.Long.LowPart == 0 )
			{
				//KdPrint(("PTE entry is completely invalid (page is not committed or is within VAD tree)\n"));
			}
			else
			{
				if( pte.u.Soft.Prototype == 1 )
				{
					// 					//KdPrint(("PTE entry is not valid, points to prototype PTE. Protection=%x[%s], ProtoAddress=%x\n",
					// 						(ULONG)pte.u.Proto.Protection,
					// 						MiPageProtectionString((UCHAR)pte.u.Proto.Protection),
					// 						(ULONG)pte.u.Proto.ProtoAddress));

					// more accurate check should be performed here for pointed prototype PTE!

					Return = VCS_PROTOTYPE;
				}
				else  // not a prototype PTE
				{
					if( pte.u.Soft.Transition != 0 )
					{
						//
						// This is a transition page.
						//

						// 						//KdPrint(("PTE entry is not valid, points to transition page. PFN=%x, Protection=%x[%s]\n",
						// 							(ULONG)pte.u.Trans.PageFrameNumber,
						// 							(ULONG)pte.u.Trans.Protection,
						// 							MiPageProtectionString((UCHAR)pte.u.Trans.Protection)));

						Return = VCS_TRANSITION;
					}
					else if (pte.u.Soft.PageFileHigh == 0)
					{
						//
						// Demand zero page
						//

						// 						//KdPrint(("PTE entry is not valid, points to demand-zero page. Protection=%x[%s]\n",
						// 							(ULONG)pte.u.Soft.Protection,
						// 							MiPageProtectionString((UCHAR)pte.u.Soft.Protection)));

						Return = VCS_DEMANDZERO;
					}
					else
					{
						//
						// Pagefile PTE
						//

						if( pte.u.Soft.Transition == 0 )
						{
							// 							//KdPrint(("PTE entry is not valid, VA is paged out. PageFile Offset=%08x, Protection=%x[%s]\n",
							// 								(ULONG)pte.u.Soft.PageFileHigh,
							// 								(ULONG)pte.u.Soft.Protection,
							// 								MiPageProtectionString((UCHAR)pte.u.Soft.Protection)));

							Return = VCS_PAGEDOUT;
						}
						else
						{
							//KdPrint(("PTE entry is not valid, Refault\n"));
						}
					}
				}
			}
		}
	}
	else
	{
		//KdPrint(("PDE entry is not valid\n"));
	}

	return Return;
}
VALIDITY_CHECK_STATUS MiIsAddressValidEx(
	IN PVOID Pointer
	)
{
	if( CR4() & PAE_ON ) {
		return MmIsAddressValidExPae(Pointer);
	}
	else {
		return MmIsAddressValidExNotPae(Pointer);
	}
}
BOOL MmIsAddressValidEx(
	IN PVOID Pointer
	)
{
	VALIDITY_CHECK_STATUS MmRet;
	ULONG ulTry;

	if (!ARGUMENT_PRESENT(Pointer) ||
		!Pointer){
		return FALSE;
	}
	/*
	//VCS_TRANSITION、VCS_PAGEDOUT内存居然是这样子~~擦~

	lkd> dd f8ad5ad8
	f8ad5ad8  ???????? ???????? ???????? ????????
	f8ad5ae8  ???????? ???????? ???????? ????????
	f8ad5af8  ???????? ???????? ???????? ????????
	f8ad5b08  ???????? ???????? ???????? ????????
	f8ad5b18  ???????? ???????? ???????? ????????
	f8ad5b28  ???????? ???????? ???????? ????????
	f8ad5b38  ???????? ???????? ???????? ????????
	f8ad5b48  ???????? ???????? ???????? ????????
	*/
	MmRet = MiIsAddressValidEx(Pointer);
	if (MmRet != VCS_VALID){
		return FALSE;
	}
	return TRUE;
}
BOOL MmIsAddressRangeValid(
	IN PVOID Address,
	IN ULONG Size
	)
{
	ULONG Ptr = NULL;
	VALIDITY_CHECK_STATUS MmRet;

	for(Ptr =  (ULONG)Address; Ptr <= (ULONG)Address + Size; Ptr++)
	{
		MmRet = MiIsAddressValidEx(Ptr);
		if (MmRet != VCS_VALID){
			return FALSE;
		}
	}
	return TRUE;
}