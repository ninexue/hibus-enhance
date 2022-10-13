#ifndef __HIBUS_BUSYBOX_HELPER__
#define __HIBUS_BUSYBOX_HELPER__

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Subroutine:  get_usr_id
 * Function:    Get UID according to user name
 * Input:       name[I], user name
 * Output:      UID, or 0xFFFFFFFF for error
 * Description:
 * Date:        2022/07/20
 * ModifyRecord:
 * *************************************************************************/
uid_t get_usr_id(const char *name);

#ifdef __cplusplus
}
#endif

#endif  // __HIBUS_BUSYBOX_HELPER__
