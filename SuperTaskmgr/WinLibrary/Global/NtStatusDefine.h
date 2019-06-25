
// NtStatusDefine.h

#pragma once

#ifndef NT_SUCCESS

//
// NTSTATUS
//

typedef LONG NTSTATUS;
/*lint -save -e624 */  // Don't complain about different typedefs.
typedef NTSTATUS *PNTSTATUS;
/*lint -restore */  // Resume checking for different typedefs.

#if _WIN32_WINNT >= 0x0600
typedef CONST NTSTATUS *PCNTSTATUS;
#endif // _WIN32_WINNT >= 0x0600

//
//  Status values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-------------------------+-------------------------------+
//  |Sev|C|       Facility          |               Code            |
//  +---+-+-------------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//

//
// Generic test for success on any status value (non-negative numbers
// indicate success).
//

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

//
// MessageId: STATUS_INFO_LENGTH_MISMATCH
//
// MessageText:
//
// The specified information record length does not match the length required for the specified information class.
//


#pragma warning (disable: 4005)

#define STATUS_UNSUCCESSFUL              ((NTSTATUS)0xC0000001L)

//
// MessageId: STATUS_NOT_IMPLEMENTED
//
// MessageText:
//
// {Not Implemented}
// The requested operation is not implemented.
//
#define STATUS_NOT_IMPLEMENTED           ((NTSTATUS)0xC0000002L)

//
// MessageId: STATUS_INVALID_INFO_CLASS
//
// MessageText:
//
// {Invalid Parameter}
// The specified information class is not a valid information class for the specified object.
//
#define STATUS_INVALID_INFO_CLASS        ((NTSTATUS)0xC0000003L)    // ntsubauth

//
// MessageId: STATUS_INFO_LENGTH_MISMATCH
//
// MessageText:
//
// The specified information record length does not match the length required for the specified information class.
//
#define STATUS_INFO_LENGTH_MISMATCH      ((NTSTATUS)0xC0000004L)

//
// MessageId: STATUS_ACCESS_VIOLATION
//
// MessageText:
//
// The instruction at 0x%08lx referenced memory at 0x%08lx. The memory could not be %s.
//
#define STATUS_ACCESS_VIOLATION          ((NTSTATUS)0xC0000005L)    // winnt

//
// MessageId: STATUS_IN_PAGE_ERROR
//
// MessageText:
//
// The instruction at 0x%p referenced memory at 0x%p. The required data was not placed into memory because of an I/O error status of 0x%x.
//
#define STATUS_IN_PAGE_ERROR             ((NTSTATUS)0xC0000006L)    // winnt

//
// MessageId: STATUS_PAGEFILE_QUOTA
//
// MessageText:
//
// The pagefile quota for the process has been exhausted.
//
#define STATUS_PAGEFILE_QUOTA            ((NTSTATUS)0xC0000007L)

//
// MessageId: STATUS_INVALID_HANDLE
//
// MessageText:
//
// An invalid HANDLE was specified.
//
#define STATUS_INVALID_HANDLE            ((NTSTATUS)0xC0000008L)    // winnt

//
// MessageId: STATUS_BAD_INITIAL_STACK
//
// MessageText:
//
// An invalid initial stack was specified in a call to NtCreateThread.
//
#define STATUS_BAD_INITIAL_STACK         ((NTSTATUS)0xC0000009L)

//
// MessageId: STATUS_BAD_INITIAL_PC
//
// MessageText:
//
// An invalid initial start address was specified in a call to NtCreateThread.
//
#define STATUS_BAD_INITIAL_PC            ((NTSTATUS)0xC000000AL)

//
// MessageId: STATUS_INVALID_CID
//
// MessageText:
//
// An invalid Client ID was specified.
//
#define STATUS_INVALID_CID               ((NTSTATUS)0xC000000BL)

//
// MessageId: STATUS_TIMER_NOT_CANCELED
//
// MessageText:
//
// An attempt was made to cancel or set a timer that has an associated APC and the subject thread is not the thread that originally set the timer with an associated APC routine.
//
#define STATUS_TIMER_NOT_CANCELED        ((NTSTATUS)0xC000000CL)

//
// MessageId: STATUS_INVALID_PARAMETER
//
// MessageText:
//
// An invalid parameter was passed to a service or function.
//
#define STATUS_INVALID_PARAMETER         ((NTSTATUS)0xC000000DL)    // winnt

//
// MessageId: STATUS_NO_SUCH_DEVICE
//
// MessageText:
//
// A device which does not exist was specified.
//
#define STATUS_NO_SUCH_DEVICE            ((NTSTATUS)0xC000000EL)

//
// MessageId: STATUS_NO_SUCH_FILE
//
// MessageText:
//
// {File Not Found}
// The file %hs does not exist.
//
#define STATUS_NO_SUCH_FILE              ((NTSTATUS)0xC000000FL)

//
// MessageId: STATUS_INVALID_DEVICE_REQUEST
//
// MessageText:
//
// The specified request is not a valid operation for the target device.
//
#define STATUS_INVALID_DEVICE_REQUEST    ((NTSTATUS)0xC0000010L)

//
// MessageId: STATUS_END_OF_FILE
//
// MessageText:
//
// The end-of-file marker has been reached. There is no valid data in the file beyond this marker.
//
#define STATUS_END_OF_FILE               ((NTSTATUS)0xC0000011L)

//
// MessageId: STATUS_WRONG_VOLUME
//
// MessageText:
//
// {Wrong Volume}
// The wrong volume is in the drive.
// Please insert volume %hs into drive %hs.
//
#define STATUS_WRONG_VOLUME              ((NTSTATUS)0xC0000012L)

//
// MessageId: STATUS_NO_MEDIA_IN_DEVICE
//
// MessageText:
//
// {No Disk}
// There is no disk in the drive.
// Please insert a disk into drive %hs.
//
#define STATUS_NO_MEDIA_IN_DEVICE        ((NTSTATUS)0xC0000013L)

//
// MessageId: STATUS_UNRECOGNIZED_MEDIA
//
// MessageText:
//
// {Unknown Disk Format}
// The disk in drive %hs is not formatted properly.
// Please check the disk, and reformat if necessary.
//
#define STATUS_UNRECOGNIZED_MEDIA        ((NTSTATUS)0xC0000014L)

//
// MessageId: STATUS_NONEXISTENT_SECTOR
//
// MessageText:
//
// {Sector Not Found}
// The specified sector does not exist.
//
#define STATUS_NONEXISTENT_SECTOR        ((NTSTATUS)0xC0000015L)

//
// MessageId: STATUS_MORE_PROCESSING_REQUIRED
//
// MessageText:
//
// {Still Busy}
// The specified I/O request packet (IRP) cannot be disposed of because the I/O operation is not complete.
//
#define STATUS_MORE_PROCESSING_REQUIRED  ((NTSTATUS)0xC0000016L)

//
// MessageId: STATUS_NO_MEMORY
//
// MessageText:
//
// {Not Enough Quota}
// Not enough virtual memory or paging file quota is available to complete the specified operation.
//

//
// MessageId: STATUS_NO_MEMORY
//
// MessageText:
//
// {Not Enough Quota}
// Not enough virtual memory or paging file quota is available to complete the specified operation.
//
#define STATUS_NO_MEMORY                 ((NTSTATUS)0xC0000017L)    // winnt

//
// MessageId: STATUS_IMAGE_ALREADY_LOADED
//
// MessageText:
//
// Indicates that the specified image is already loaded.
//
#define STATUS_IMAGE_ALREADY_LOADED      ((NTSTATUS)0xC000010EL)

#pragma warning (default: 4005)

//
// Generic test for information on any status value.
//


#endif


